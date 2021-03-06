//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// nodeValuesscan.h
//
// Identification: src/include/parser/executor/nodeValuesscan.h
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//


/*-------------------------------------------------------------------------
 *
 * nodeValuesscan.h
 *
 *
 *
 * Portions Copyright (c) 1996-2015, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/executor/nodeValuesscan.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef NODEVALUESSCAN_H
#define NODEVALUESSCAN_H

#include "parser/nodes/execnodes.h"

extern ValuesScanState *ExecInitValuesScan(ValuesScan *node, EState *estate, int eflags);
extern TupleTableSlot *ExecValuesScan(ValuesScanState *node);
extern void ExecEndValuesScan(ValuesScanState *node);
extern void ExecReScanValuesScan(ValuesScanState *node);


extern TupleTableSlot *ValuesNext(ValuesScanState *node);

#endif   /* NODEVALUESSCAN_H */
