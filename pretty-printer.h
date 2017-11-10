/* pretty-printer.h  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token-list.h"

#define SPROGRAM			 50	/* all of		TPROGRAM TNAME TSEMI SBLOCK TDOT                         */
#define SBLOCK				 51	/* all of		SBLOCK_1 SCOMPSTAT                                       */
#define SBLOCK_1			 52	/* zero or more	SBLOCK_1_1                                               */
#define SBLOCK_1_1			 53	/* one of		SVARDEC SSUBPROGDEC                                      */
#define SVARDEC				 54	/* all of		TVAR SVARNAMES TCOLON STYPE TSEMI SVARDEC_6              */
#define SVARDEC_6			 55	/* zero or more	SVARDEC_6_1                                              */
#define SVARDEC_6_1			 56	/* all of		SVARNAMES TCOLON STYPE TSEMI                             */
#define SVARNAMES			 57	/* all of		SVARNAME SVARNAMES_1                                     */
#define SVARNAMES_1			 58	/* zero or more	SVARNAMES_1_1                                            */
#define SVARNAMES_1_1		 59	/* all of		TCOMMA SVARNAME                                          */
#define SVARNAME			 60	/* all of		TNAME                                                    */
#define STYPE				 61	/* one of		SSTDTYPE SARRTYPE                                        */
#define SSTDTYPE			 62	/* one of		TINTEGER TBOOLEAN TCHAR                                  */
#define SARRTYPE			 63	/* all of		TARRAY TLSQPAREN TNUMBER TRSQPAREN TOF SSTDTYPE          */
#define SSUBPROGDEC			 64	/* all of		TPROCEDURE SPROCEDURENAME SSUBPROGDEC_3
													TSEMI SSUBPROGDEC_5 SCOMPSTAT TSEMI                  */
#define SSUBPROGDEC_3		 65	/* zero or one	SFORMALPARAM                                             */
#define SSUBPROGDEC_5		 66	/* zero or one	SVARDEC                                                  */
#define SPROCEDURENAME		 67	/* all of		TNAME                                                    */
#define SFORMPARAM			 68	/* all of		TLPAREN SVARNAMES TCOLON STYPE SFORMPARAM_5 TRPAREN      */
#define SFORMPARAM_5		 69	/* zero or more	SFORMPARAM_5_1                                           */
#define SFORMPARAM_5_1		 70	/* all of		TSEMI SVARNAMES TCOLON STYPE                             */
#define SCOMPSTAT			 71	/* all of		TBEGIN SSTAT SCOMPSTAT_3 TEND                            */
#define SCOMPSTAT_3			 72	/* zero or more	SCOMPSTAT_3_1                                            */
#define SCOMPSTAT_3_1		 73	/* all of		TSEMI SSTAT                                              */
#define SSTAT				 74	/* one of		SASSIGNSTAT SCONDSTAT SITERSTAT SEXITSTAT SCALLSTAT
													SRETSTAT SINPUTSTAT SOUTPUTSTAT SCOMPSTAT SEMPTYSTAT */
#define SCONDSTAT			 75	/* all of		TIF SEXPR TTHEN SSTAT SCONDSTAT_5                        */
#define SCONDSTAT_5			 76	/* zero or one	SCONDSTAT_5_1                                            */
#define SCONDSTAT_5_1		 77	/* all of		TELSE SSTAT                                              */
#define SITERSTAT			 78	/* all of		TWHILE SSTAT TDO SSTAT                                   */
#define SEXITSTAT			 79	/* serial	    TBREAK                                                   */
#define SCALLSTAT			 80	/* all of		TCALL SPROCEDURENAME SCALLSTAT_3                         */
#define SCALLSTAT_3			 81	/* zero or one	SCALLSTAT_3_1                                            */
#define SCALLSTAT_3_1		 82	/* all of		TLPAREN SEXPRS TRPAREN                                   */
#define SEXPRS				 83	/* all of		TEXPR SEXPRS_2                                           */
#define SEXPRS_2			 84	/* zero or more	SEXPRS_2_1                                               */
#define SEXPRS_2_1			 85	/* all of		TCOMMA TEXPR                                             */
#define SRETSTAT			 86	/* all of		TRETURN                                                  */
#define SASSIGNSTAT			 87	/* all of		SLEFTPART TASSIGN SEXPR                                  */
#define SLEFTPART			 88	/* all of		SVAR                                                     */
#define SVAR				 89	/* all of		SVARNAME SVAR_2                                          */
#define SVAR_2				 90	/* zero or one	SVAR_2_1                                                 */
#define SVAR_2_1			 91	/* all of		TLSQPAREN SEXPR TRSQPAREN                                */
#define SEXPR				 92	/* all of		SSIMPLEEXPR SEXPR_2                                      */
#define SEXPR_2				 93	/* zero or more	SEXPR_2_1                                                */
#define SEXPR_2_1			 94	/* all of		SRELATOP SSIMPLEEXPR                                     */
#define SSIMPLEEXPR			 95	/* all of		SSIMPLEEXPR_1 STERM SSIMPLEEXPR_3                        */
#define SSIMPLEEXPR_1		 96	/* zero or one	SSIMPLEEXPR_1_1                                          */
#define SSIMPLEEXPR_1_1		 97	/* one of		TPLUS TMINUS                                             */
#define SSIMPLEEXPR_3		 98	/* zero or more	SSIMPLEEXPR_3_1                                          */
#define SSIMPLEEXPR_3_1		 99	/* all of		SADDOP STERM                                             */
#define STERM				100	/* all of		SFACTOR STERM_2                                          */
#define STERM_2				101	/* zero or more	STERM_2_1                                                */
#define STERM_2_1			102	/* all of		SMULOP SFACTOR                                           */
#define SFACTOR				103	/* one of		SVAR SCONST SFACTOR_3 SFACTOR_4 SFACTOR_5                */
#define SFACTOR_3			104	/* all of		TLPAREN SEXPR TRPAREN                                    */
#define SFACTOR_4			105	/* all of		TNOT SFACTOR                                             */
#define SFACTOR_5			106	/* all of		SSTDTYPE TLPAREN SEXPR TRPAREN                           */
#define SCONST				107	/* one of		TNUMBER TFALSE TTURE TSTRING                             */
#define SMULOP				108	/* one of		TSTAR TDIV TAND                                          */
#define SADDOP				109	/* one of		TPLUS TMINUS TOR                                         */
#define SRELATOP			110	/* one of		TEQUAL TNOTEQ TLE TLEEQ TGR TGREQ                        */
#define SINSTAT				111	/* all of		SINSTAT_1 SINSTAT_2                                      */
#define SINSTAT_1			112	/* one of		TREAD TREADLN                                            */
#define SINSTAT_2			113	/* zero or one	SINSTAT_2_1                                              */
#define SINSTAT_2_1			114	/* all of		TLPAREN SVAR SINSTAT_2_1_3 TRPAREN                       */
#define SINSTAT_2_1_3		115	/* zero or more	SINSTAT_2_1_3_1                                          */
#define SINSTAT_2_1_3_1		116	/* all of		TCOMMA SVAR                                              */
#define SOUTSTAT			117	/* all of		SOUTSTAT_1 SOUTSTAT_2                                    */
#define SOUTSTAT_1			118	/* one of		TWRITE TWRITELN                                          */
#define SOUTSTAT_2			119	/* zero or one	SOUTSTAT_2_1                                             */
#define SOUTSTAT_2_1		120	/* all of		TLPAREN SOUTFORM SOUTSTAT_2_1_3 TRPAREN                  */
#define SOUTSTAT_2_1_3		121	/* zero or more	SOUTSTAT_2_1_3_1                                         */
#define SOUTSTAT_2_1_3_1	122	/* all of		TCOMMA TOURFORM                                          */
#define SOUTFORM			123	/* one of		SOUTFORM_1 TSTRING                                       */
#define SOUTFORM_1			124	/* all of		SEXPR SOUTFORM_1_2                                       */
#define SOUTFORM_1_2		125	/* zero or one	SOUTFORM_1_2_1                                           */
#define SOUTFORM_1_2_1		126	/* all of		TCOLON TNUMBER                                           */
#define SEMPTYSTAT			127

#define NUMOFSYNTAX			127

typedef struct SyntaxElem_{
	int op;
	int childrenNum;
	int children[20];
} SyntaxElem;

typedef struct SyntaxTreeNode_{
	int sElemIt;
	char string_attr[MAXSTRSIZE];
	int indent_depth;
	int iter_depth;
	int parse_result;
	struct SyntaxTreeNode_* brother;
	struct SyntaxTreeNode_* child;
} SyntaxTreeNode;

#define SELEMOP_ALL_OF			1
#define SELEMOP_ONE_OF			2
#define SELEMOP_ZERO_OR_MORE	3
#define SELEMOP_ZERO_OR_ONE		4
#define SELEMOP_TERMINATOR		5

extern const char* SYNTAXDIC[NUMOFSYNTAX + 1];

extern void debug_tree(SyntaxTreeNode* node);
extern void print_tree(SyntaxTreeNode* node);
extern void free_tree(SyntaxTreeNode* node);
extern SyntaxTreeNode* malloc_tree_node();

/* parse.c */
#define PARSERESULT_MATCH		1
#define PARSERESULT_EMPTY		2
#define PARSERESULT_NOTMATCH	0
extern int parse_without_tree(int sElemIt, int depth);
extern SyntaxTreeNode* parse(int sElemIt, int indent_depth);
extern int is_indent(int parentSyntaxElemIt, int childIt);
extern void init_parse(void);

/* preformat.c */
extern void preformat_tree(SyntaxTreeNode* node);
