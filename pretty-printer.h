/* pretty-printer.h  */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "token-list.h"

#define SPROGRAM			 50	/* serial		TPROGRAM TNAME TSEMI SBLOCK TDOT                         */
#define SBLOCK				 51	/* serial		SBLOCK_1 SCOMPSTAT                                       */
#define SBLOCK_1			 52	/* zero or more	SBLOCK_1_1                                               */
#define SBLOCK_1_1			 53	/* parallel		SVARDEC SSUBPROGDEC                                      */
#define SVARDEC				 54	/* serial		TVAR SVARNAMES TCOLON STYPE TSEMI SVARDEC_6              */
#define SVARDEC_6			 55	/* zero or more	SVARDEC_6_1                                              */
#define SVARDEC_6_1			 56	/* serial		SVARNAMES TCOLON STYPE TSEMI                             */
#define SVARNAMES			 57	/* serial		SVARNAME SVARNAMES_1                                     */
#define SVARNAMES_1			 58	/* zero or more	SVARNAMES_1_1                                            */
#define SVARNAMES_1_1		 59	/* serial		TCOMMA SVARNAME                                          */
#define SVARNAME			 60	/* serial		TNAME                                                    */
#define STYPE				 61	/* parallel		SSTDTYPE SARRTYPE                                        */
#define SSTDTYPE			 62	/* parallel		TINTEGER TBOOLEAN TCHAR                                  */
#define SARRTYPE			 63	/* serial		TARRAY TLSQPAREN TNUMBER TRSQPAREN TOF SSTDTYPE          */
#define SSUBPROGDEC			 64	/* serial		TPROCEDURE SPROCEDURENAME SSUBPROGDEC_3
													TSEMI SSUBPROGDEC_5 SCOMPSTAT TSEMI                  */
#define SSUBPROGDEC_3		 65	/* zero or one	SFORMALPARAM                                             */
#define SSUBPROGDEC_5		 66	/* zero or one	SVARDEC                                                  */
#define SPROCEDURENAME		 67	/* serial		TNAME                                                    */
#define SFORMPARAM			 68	/* serial		TLPAREN SVARNAMES TCOLON STYPE SFORMPARAM_5 TRPAREN      */
#define SFORMPARAM_5		 69	/* zero or more	SFORMPARAM_5_1                                           */
#define SFORMPARAM_5_1		 70	/* serial		TSEMI SVARNAMES TCOLON STYPE                             */
#define SCOMPSTAT			 71	/* serial		TBEGIN SSTAT SCOMPSTAT_3 TEND                            */
#define SCOMPSTAT_3			 72	/* zero or more	SCOMPSTAT_3_1                                            */
#define SCOMPSTAT_3_1		 73	/* serial		TSEMI SSTAT                                              */
#define SSTAT				 74	/* parallel		SASSIGNSTAT SCONDSTAT SITERSTAT SEXITSTAT SCALLSTAT
													SRETSTAT SINPUTSTAT SOUTPUTSTAT SCOMPSTAT SEMPTYSTAT */
#define SCONDSTAT			 75	/* serial		TIF SEXPR TTHEN SSTAT SCONDSTAT_5                        */
#define SCONDSTAT_5			 76	/* zero or one	SCONDSTAT_5_1                                            */
#define SCONDSTAT_5_1		 77	/* serial		TELSE SSTAT                                              */
#define SITERSTAT			 78	/* serial		TWHILE SSTAT TDO SSTAT                                   */
#define SEXITSTAT			 79	/* serial	    TBREAK                                                   */
#define SCALLSTAT			 80	/* serial		TCALL SPROCEDURENAME SCALLSTAT_3                         */
#define SCALLSTAT_3			 81	/* zero or one	SCALLSTAT_3_1                                            */
#define SCALLSTAT_3_1		 82	/* serial		TLPAREN SEXPRS TRPAREN                                   */
#define SEXPRS				 83	/* serial		TEXPR SEXPRS_2                                           */
#define SEXPRS_2			 84	/* zero or more	SEXPRS_2_1                                               */
#define SEXPRS_2_1			 85	/* serial		TCOMMA TEXPR                                             */
#define SRETSTAT			 86	/* serial		TRETURN                                                  */
#define SASSIGNSTAT			 87	/* serial		SLEFTPART TASSIGN SEXPR                                  */
#define SLEFTPART			 88	/* serial		SVAR                                                     */
#define SVAR				 89	/* serial		SVARNAME SVAR_2                                          */
#define SVAR_2				 90	/* zero or one	SVAR_2_1                                                 */
#define SVAR_2_1			 91	/* serial		TLSQPAREN SEXPR TRSQPAREN                                */
#define SEXPR				 92	/* serial		SSIMPLEEXPR SEXPR_2                                      */
#define SEXPR_2				 93	/* zero or more	SEXPR_2_1                                                */
#define SEXPR_2_1			 94	/* serial		SRELATOP SSIMPLEEXPR                                     */
#define SSIMPLEEXPR			 95	/* serial		SSIMPLEEXPR_1 STERM SSIMPLEEXPR_3                        */
#define SSIMPLEEXPR_1		 96	/* zero or one	SSIMPLEEXPR_1_1                                          */
#define SSIMPLEEXPR_1_1		 97	/* parallel		TPLUS TMINUS                                             */
#define SSIMPLEEXPR_3		 98	/* zero or more	SSIMPLEEXPR_3_1                                          */
#define SSIMPLEEXPR_3_1		 99	/* serial		SADDOP STERM                                             */
#define STERM				100	/* serial		SFACTOR STERM_2                                          */
#define STERM_2				101	/* zero or more	STERM_2_1                                                */
#define STERM_2_1			102	/* serial		SMULOP SFACTOR                                           */
#define SFACTOR				103	/* parallel		SVAR SCONST SFACTOR_3 SFACTOR_4 SFACTOR_5                */
#define SFACTOR_3			104	/* serial		TLPAREN SEXPR TRPAREN                                    */
#define SFACTOR_4			105	/* serial		TNOT SFACTOR                                             */
#define SFACTOR_5			106	/* serial		SSTDTYPE TLPAREN SEXPR TRPAREN                           */
#define SCONST				107	/* parallel		TNUMBER TFALSE TTURE TSTRING                             */
#define SMULOP				108	/* parallel		TSTAR TDIV TAND                                          */
#define SADDOP				109	/* parallel		TPLUS TMINUS TOR                                         */
#define SRELATOP			110	/* parallel		TEQUAL TNOTEQ TLE TLEEQ TGR TGREQ                        */
#define SINSTAT				111	/* serial		SINSTAT_1 SINSTAT_2                                      */
#define SINSTAT_1			112	/* parallel		TREAD TREADLN                                            */
#define SINSTAT_2			113	/* zero or one	SINSTAT_2_1                                              */
#define SINSTAT_2_1			114	/* serial		TLPAREN SVAR SINSTAT_2_1_3 TRPAREN                       */
#define SINSTAT_2_1_3		115	/* zero or more	SINSTAT_2_1_3_1                                          */
#define SINSTAT_2_1_3_1		116	/* serial		TCOMMA SVAR                                              */
#define SOUTSTAT			117	/* serial		SOUTSTAT_1 SOUTSTAT_2                                    */
#define SOUTSTAT_1			118	/* parallel		TWRITE TWRITELN                                          */
#define SOUTSTAT_2			119	/* zero or one	SOUTSTAT_2_1                                             */
#define SOUTSTAT_2_1		120	/* serial		TLPAREN SOUTFORM SOUTSTAT_2_1_3                          */
#define SOUTSTAT_2_1_3		121	/* zero or more	SOUTSTAT_2_1_3_1                                         */
#define SOUTSTAT_2_1_3_1	122	/* serial		TCOMMA TOURFORM                                          */
#define SOUTFORM			123	/* parallel		SOUTFORM_1 TSTRING                                       */
#define SOUTFORM_1			124	/* serial		SEXPR SOUTFORM_1_2                                       */
#define SOUTFORM_1_2		125	/* zero or one	SOUTFORM_1_2_1                                           */
#define SOUTFORM_1_2_1		126	/* serial		TCOLON TNUMBER                                           */
#define SEMPTYSTAT			127	/* evaluate(SEMPTYSTAT) always returns 1                                 */

#define NUMOFSYNTAX			127

#define SELEMOP_ALL_OF			1
#define SELEMOP_ONE_OF  		2
#define SELEMOP_ZERO_OR_MORE	3
#define SELEMOP_ZERO_OR_ONE		4
#define SELEMOP_SINGLE_TOKEN	5

typedef struct SyntaxElem_{
	int op;
	int childrenNum;
	int children[10];
}SyntaxElem;

int SyntaxElem_parse();
