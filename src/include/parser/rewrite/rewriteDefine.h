//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// rewriteDefine.h
//
// Identification: src/include/parser/rewrite/rewriteDefine.h
//
// Copyright (c) 2015-16, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//


/*-------------------------------------------------------------------------
 *
 * rewriteDefine.h
 *
 *
 *
 * Portions Copyright (c) 1996-2015, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/rewrite/rewriteDefine.h
 *
 *-------------------------------------------------------------------------
 */
#ifndef REWRITEDEFINE_H
#define REWRITEDEFINE_H

#include "parser/catalog/objectaddress.h"
#include "parser/nodes/parsenodes.h"
#include "parser/utils/relcache.h"

#define RULE_FIRES_ON_ORIGIN	'O'
#define RULE_FIRES_ALWAYS		'A'
#define RULE_FIRES_ON_REPLICA	'R'
#define RULE_DISABLED			'D'

extern ObjectAddress DefineRule(RuleStmt *stmt, const char *queryString);

extern ObjectAddress DefineQueryRewrite(char *rulename,
				   Oid event_relid,
				   Node *event_qual,
				   CmdType event_type,
				   bool is_instead,
				   bool replace,
				   List *action);

extern ObjectAddress RenameRewriteRule(RangeVar *relation, const char *oldName,
				  const char *newName);

extern void setRuleCheckAsUser(Node *node, Oid userid);

extern void EnableDisableRule(Relation rel, const char *rulename,
				  char fires_when);

#endif   /* REWRITEDEFINE_H */
