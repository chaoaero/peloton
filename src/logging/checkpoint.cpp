//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// checkpoint.cpp
//
// Identification: src/logging/checkpoint.cpp
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//


#include "common/pool.h"
#include "logging/checkpoint.h"
#include "logging/logging_util.h"
#include "logging/checkpoint/simple_checkpoint.h"
#include "logging/log_manager.h"
#include "logging/checkpoint_manager.h"
#include "logging/backend_logger.h"
#include "storage/tile.h"
#include "storage/database.h"
#include "storage/tile_group.h"
#include "storage/tuple.h"

namespace peloton {
namespace logging {

//===--------------------------------------------------------------------===//
// Checkpoint
//===--------------------------------------------------------------------===//
/**
 * @brief MainLoop
 */
void Checkpoint::MainLoop(void) {
  auto &checkpoint_manager = CheckpointManager::GetInstance();

  /////////////////////////////////////////////////////////////////////
  // STANDBY MODE
  /////////////////////////////////////////////////////////////////////
  LOG_TRACE("Checkpoint Standby Mode");

  // Standby before we need to do RECOVERY
  checkpoint_manager.WaitForModeTransition(CHECKPOINT_STATUS_STANDBY, false);

  // Do recovery if we can, otherwise terminate
  switch (checkpoint_manager.GetCheckpointStatus()) {
    case CHECKPOINT_STATUS_RECOVERY: {
      LOG_TRACE("Checkpoint Recovery Mode");
      /////////////////////////////////////////////////////////////////////
      // RECOVERY MODE
      /////////////////////////////////////////////////////////////////////

      // First, do recovery if needed
      DoRecovery();
      LOG_TRACE("Checkpoint DoRecovery Done");
      checkpoint_status = CHECKPOINT_STATUS_DONE_RECOVERY;
      break;
    }

    case CHECKPOINT_STATUS_CHECKPOINTING: {
      LOG_TRACE("Checkpoint Checkpointing Mode");
    } break;

    default:
      break;
  }

  checkpoint_manager.SetCheckpointStatus(CHECKPOINT_STATUS_DONE_RECOVERY);
  checkpoint_manager.WaitForModeTransition(CHECKPOINT_STATUS_CHECKPOINTING,
                                           true);

  /////////////////////////////////////////////////////////////////////
  // CHECKPOINTING MODE
  /////////////////////////////////////////////////////////////////////
  // Periodically, wake up and do checkpointing
  while (checkpoint_manager.GetCheckpointStatus() ==
         CHECKPOINT_STATUS_CHECKPOINTING) {
    checkpoint_status = CHECKPOINT_STATUS_CHECKPOINTING;
    sleep(checkpoint_interval_);
    DoCheckpoint();
  }
}

std::string Checkpoint::ConcatFileName(std::string checkpoint_dir,
                                       int version) {
  return checkpoint_dir + "/" + FILE_PREFIX + std::to_string(version) +
         FILE_SUFFIX;
}

void Checkpoint::InitDirectory() {
  auto success = LoggingUtil::CreateDirectory(checkpoint_dir.c_str(), 0700);
  if (success) {
    LOG_TRACE("Checkpoint directory is: %s", checkpoint_dir.c_str());
  } else {
    LOG_ERROR("Failed to create checkpoint directory");
  }
}

std::unique_ptr<Checkpoint> Checkpoint::GetCheckpoint(
    CheckpointType checkpoint_type, bool disable_file_access) {
  if (checkpoint_type == CHECKPOINT_TYPE_NORMAL) {
    std::unique_ptr<Checkpoint> checkpoint(
        new SimpleCheckpoint(disable_file_access));
    return std::move(checkpoint);
  }
  return std::move(std::unique_ptr<Checkpoint>(nullptr));
}

void Checkpoint::RecoverTuple(storage::Tuple *tuple, storage::DataTable *table,
                              ItemPointer target_location, cid_t commit_id) {
  auto tile_group_id = target_location.block;
  auto tuple_slot = target_location.offset;

  LOG_TRACE("Recover tuple from checkpoint (%u, %u)", tile_group_id,
            tuple_slot);

  auto &manager = catalog::Manager::GetInstance();
  auto tile_group = manager.GetTileGroup(tile_group_id);

  // Create new tile group if table doesn't already have that tile group
  if (tile_group == nullptr) {
    table->AddTileGroupWithOidForRecovery(tile_group_id);
    tile_group = manager.GetTileGroup(tile_group_id);
  }

  // Do the insert!
  auto inserted_tuple_slot =
      tile_group->InsertTupleFromCheckpoint(tuple_slot, tuple, commit_id);

  if (inserted_tuple_slot == INVALID_OID) {
    // TODO: We need to abort on failure!
  } else {
    // TODO this is not thread safe
    table->SetNumberOfTuples(table->GetNumberOfTuples() + 1);
  }
}

}  // namespace logging
}  // namespace peloton
