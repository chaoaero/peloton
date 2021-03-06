//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// nodeBitmapHeapscan.h
//
// Identification: src/include/parser/executor/nodeBitmapHeapscan.h
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//


/*-------------------------------------------------------------------------
 *
 * nodeBitmapHeapscan.h
 *
 *
 *
 * Portions Copyright (c) 1996-2015, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/executor/nodeBitmapHeapscan.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEBITMAPHEAPSCAN_H
#define NODEBITMAPHEAPSCAN_H

#include "parser/nodes/execnodes.h"

extern BitmapHeapScanState *ExecInitBitmapHeapScan(BitmapHeapScan *node, EState *estate, int eflags);
extern TupleTableSlot *ExecBitmapHeapScan(BitmapHeapScanState *node);
extern void ExecEndBitmapHeapScan(BitmapHeapScanState *node);
extern void ExecReScanBitmapHeapScan(BitmapHeapScanState *node);

#endif   /* NODEBITMAPHEAPSCAN_H */
