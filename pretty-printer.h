/* pretty-printer.h  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token-list.h"

#define SPROGRAM			 50	/* all of		TPROGRAM TNAME TSEMI SBLOCK TDOT                         */
#define SBLOCK				 51	/* all of		SBLOCK_0 SCOMPSTAT                                       */
#define SBLOCK_0			 52	/* zero or more	SBLOCK_0_0                                               */
#define SBLOCK_0_0			 53	/* one of		SVARDEC SSUBPROGDEC                                      */
#define SVARDEC				 54	/* all of		TVAR SVARNAMES TCOLON STYPE TSEMI SVARDEC_5              */
#define SVARDEC_5			 55	/* zero or more	SVARDEC_5_0                                              */
#define SVARDEC_5_0			 56	/* all of		SVARNAMES TCOLON STYPE TSEMI                             */
#define SVARNAMES			 57	/* all of		SVARNAME SVARNAMES_1                                     */
#define SVARNAMES_1			 58	/* zero or more	SVARNAMES_1_0                                            */
#define SVARNAMES_1_0		 59	/* all of		TCOMMA SVARNAME                                          */
#define SVARNAME			 60	/* all of		TNAME                                                    */
#define STYPE				 61	/* one of		SSTDTYPE SARRTYPE                                        */
#define SSTDTYPE			 62	/* one of		TINTEGER TBOOLEAN TCHAR                                  */
#define SARRTYPE			 63	/* all of		TARRAY TLSQPAREN TNUMBER TRSQPAREN TOF SSTDTYPE          */
#define SSUBPROGDEC			 64	/* all of		TPROCEDURE SPROCEDURENAME SSUBPROGDEC_2
													TSEMI SSUBPROGDEC_4 SCOMPSTAT TSEMI                  */
#define SSUBPROGDEC_2		 65	/* zero or one	SFORMALPARAM                                             */
#define SSUBPROGDEC_4		 66	/* zero or one	SVARDEC                                                  */
#define SPROCEDURENAME		 67	/* all of		TNAME                                                    */
#define SFORMPARAM			 68	/* all of		TLPAREN SVARNAMES TCOLON STYPE SFORMPARAM_5 TRPAREN      */
#define SFORMPARAM_4		 69	/* zero or more	SFORMPARAM_4_0                                           */
#define SFORMPARAM_4_0		 70	/* all of		TSEMI SVARNAMES TCOLON STYPE                             */
#define SCOMPSTAT			 71	/* all of		TBEGIN SSTAT SCOMPSTAT_2 TEND                            */
#define SCOMPSTAT_2			 72	/* zero or more	SCOMPSTAT_2_0                                            */
#define SCOMPSTAT_2_0		 73	/* all of		TSEMI SSTAT                                              */
#define SSTAT				 74	/* one of		SASSIGNSTAT SCONDSTAT SITERSTAT SEXITSTAT SCALLSTAT
													SRETSTAT SINPUTSTAT SOUTPUTSTAT SCOMPSTAT SEMPTYSTAT */
#define SCONDSTAT			 75	/* all of		TIF SEXPR TTHEN SSTAT SCONDSTAT_4                        */
#define SCONDSTAT_4			 76	/* zero or one	SCONDSTAT_4_0                                            */
#define SCONDSTAT_4_0		 77	/* all of		TELSE SSTAT                                              */
#define SITERSTAT			 78	/* all of		TWHILE SSTAT TDO SSTAT                                   */
#define SEXITSTAT			 79	/* serial	    TBREAK                                                   */
#define SCALLSTAT			 80	/* all of		TCALL SPROCEDURENAME SCALLSTAT_2                         */
#define SCALLSTAT_2			 81	/* zero or one	SCALLSTAT_2_0                                            */
#define SCALLSTAT_2_0		 82	/* all of		TLPAREN SEXPRS TRPAREN                                   */
#define SEXPRS				 83	/* all of		TEXPR SEXPRS_2                                           */
#define SEXPRS_1			 84	/* zero or more	SEXPRS_1_0                                               */
#define SEXPRS_1_0			 85	/* all of		TCOMMA TEXPR                                             */
#define SRETSTAT			 86	/* all of		TRETURN                                                  */
#define SASSIGNSTAT			 87	/* all of		SLEFTPART TASSIGN SEXPR                                  */
#define SLEFTPART			 88	/* all of		SVAR                                                     */
#define SVAR				 89	/* all of		SVARNAME SVAR_1                                          */
#define SVAR_1				 90	/* zero or one	SVAR_1_0                                                 */
#define SVAR_1_0			 91	/* all of		TLSQPAREN SEXPR TRSQPAREN                                */
#define SEXPR				 92	/* all of		SSIMPLEEXPR SEXPR_1                                      */
#define SEXPR_1				 93	/* zero or more	SEXPR_1_0                                                */
#define SEXPR_1_0			 94	/* all of		SRELATOP SSIMPLEEXPR                                     */
#define SSIMPLEEXPR			 95	/* all of		SSIMPLEEXPR_0 STERM SSIMPLEEXPR_2                        */
#define SSIMPLEEXPR_0		 96	/* zero or one	SSIMPLEEXPR_0_0                                          */
#define SSIMPLEEXPR_0_0		 97	/* one of		TPLUS TMINUS                                             */
#define SSIMPLEEXPR_2		 98	/* zero or more	SSIMPLEEXPR_2_0                                          */
#define SSIMPLEEXPR_2_0		 99	/* all of		SADDOP STERM                                             */
#define STERM				100	/* all of		SFACTOR STERM_1                                          */
#define STERM_1				101	/* zero or more	STERM_1_0                                                */
#define STERM_1_0			102	/* all of		SMULOP SFACTOR                                           */
#define SFACTOR				103	/* one of		SVAR SCONST SFACTOR_2 SFACTOR_3 SFACTOR_4                */
#define SFACTOR_2			104	/* all of		TLPAREN SEXPR TRPAREN                                    */
#define SFACTOR_3			105	/* all of		TNOT SFACTOR                                             */
#define SFACTOR_4			106	/* all of		SSTDTYPE TLPAREN SEXPR TRPAREN                           */
#define SCONST				107	/* one of		TNUMBER TFALSE TTURE TSTRING                             */
#define SMULOP				108	/* one of		TSTAR TDIV TAND                                          */
#define SADDOP				109	/* one of		TPLUS TMINUS TOR                                         */
#define SRELATOP			110	/* one of		TEQUAL TNOTEQ TLE TLEEQ TGR TGREQ                        */
#define SINSTAT				111	/* all of		SINSTAT_0 SINSTAT_1                                      */
#define SINSTAT_0			112	/* one of		TREAD TREADLN                                            */
#define SINSTAT_1			113	/* zero or one	SINSTAT_1_0                                              */
#define SINSTAT_1_0			114	/* all of		TLPAREN SVAR SINSTAT_1_0_2 TRPAREN                       */
#define SINSTAT_1_0_2		115	/* zero or more	SINSTAT_1_0_2_0                                          */
#define SINSTAT_1_0_2_0		116	/* all of		TCOMMA SVAR                                              */
#define SOUTSTAT			117	/* all of		SOUTSTAT_0 SOUTSTAT_1                                    */
#define SOUTSTAT_0			118	/* one of		TWRITE TWRITELN                                          */
#define SOUTSTAT_1			119	/* zero or one	SOUTSTAT_1_0                                             */
#define SOUTSTAT_1_0		120	/* all of		TLPAREN SOUTFORM SOUTSTAT_1_0_2 TRPAREN                  */
#define SOUTSTAT_1_0_2		121	/* zero or more	SOUTSTAT_1_0_2_0                                         */
#define SOUTSTAT_1_0_2_0	122	/* all of		TCOMMA TOURFORM                                          */
#define SOUTFORM			123	/* one of		SOUTFORM_0 TSTRING                                       */
#define SOUTFORM_0			124	/* all of		SEXPR SOUTFORM_0_1                                       */
#define SOUTFORM_0_1		125	/* zero or one	SOUTFORM_0_1_0                                           */
#define SOUTFORM_0_1_0		126	/* all of		TCOLON TNUMBER                                           */
#define SEMPTYSTAT			127

#define NUMOFSYNTAX			127

typedef struct SyntaxElem_{
	int op;
	int children_num;
	int children[20];
} SyntaxElem;

typedef struct SyntaxTreeNode_{
	int s_elem_it;
	char string_attr[MAXSTRSIZE];
	int indent_depth;
	int iter_depth;
	int line_num;
	int parse_result;
	struct SyntaxTreeNode_* parent;
	struct SyntaxTreeNode_* brother;
	struct SyntaxTreeNode_* child;
} SyntaxTreeNode;

#define SELEMOP_ALL_OF			1
#define SELEMOP_ONE_OF			2
#define SELEMOP_ZERO_OR_MORE	3
#define SELEMOP_ZERO_OR_ONE		4
#define SELEMOP_TERMINATOR		5

extern const char* SYNTAXDIC[NUMOFSYNTAX + 1];

extern SyntaxTreeNode* malloc_tree_node();
extern void free_tree(SyntaxTreeNode* node);
extern void print_tree(SyntaxTreeNode* node);

/* parse.c */
#define PARSERESULT_MATCH		1
#define PARSERESULT_DIFFERENCE	0
#define PARSERESULT_EMPTY		2
#define PARSERESULT_ACCIDENT	3
extern SyntaxElem s_elem_array[NUMOFSYNTAX + 1];
extern void init_parse(void);
extern SyntaxTreeNode* parse(int s_elem_it, int iter_depth);
