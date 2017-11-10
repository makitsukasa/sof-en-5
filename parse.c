#include "pretty-printer.h"

/* const */ SyntaxElem syntaxElems[NUMOFSYNTAX + 1];

int token;

int is_indent(int parentSyntaxElemIt, int childIt){
	if(parentSyntaxElemIt == SBLOCK && childIt == 0)
		return 1;

	if(parentSyntaxElemIt == SCOMPSTAT && 
		(childIt == 1 || childIt == 2))
			return 1;

	return 0;
}

void init_parse(void) {
	int i;
	token = scan();

	for(i = 0; i < NUMOFSYNTAX; i++){
		SyntaxElem hoge = {SELEMOP_TERMINATOR, 0, {}};
		syntaxElems[i] = hoge;
	}

	/*
	SyntaxElem sElem_TNAME				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TPROGRAM			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TVAR				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TARRAY				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TOF				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TBEGIN				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TEND				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TIF				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TTHEN				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TELSE				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TPROCEDURE			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TRETURN			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TCALL				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TWHILE				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TDO				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TNOT				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TOR				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TDIV				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TAND				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TCHAR				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TINTEGER			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TBOOLEAN			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TREADLN			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TWRITELN			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TTRUE				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TFALSE				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TNUMBER			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TSTRING			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TPLUS				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TMINUS				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TSTAR				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TEQUAL				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TNOTEQ				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TLE				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TLEEQ				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TGR				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TGREQ				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TLPAREN			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TRPAREN			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TLSQPAREN			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TRSQPAREN			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TASSIGN			= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TDOT				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TCOMMA				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TCOLON				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TSEMI				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TREAD				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TWRITE				= {SELEMOP_TERMINATOR,	0, {}};
	SyntaxElem sElem_TBREAK				= {SELEMOP_TERMINATOR,	0, {}};
	*/
	SyntaxElem sElem_SPROGRAM			= {SELEMOP_ALL_OF,			5, {TPROGRAM, TNAME, TSEMI, SBLOCK, TDOT}};
	SyntaxElem sElem_SBLOCK				= {SELEMOP_ALL_OF,			2, {SBLOCK_1, SCOMPSTAT}};
	SyntaxElem sElem_SBLOCK_1			= {SELEMOP_ZERO_OR_MORE,	1, {SBLOCK_1_1}};
	SyntaxElem sElem_SBLOCK_1_1			= {SELEMOP_ONE_OF,			2, {SVARDEC, SSUBPROGDEC}};
	SyntaxElem sElem_SVARDEC			= {SELEMOP_ALL_OF,			6, {TVAR, SVARNAMES, TCOLON, STYPE, TSEMI, SVARDEC_6}};
	SyntaxElem sElem_SVARDEC_6			= {SELEMOP_ZERO_OR_MORE,	1, {SVARDEC_6_1}};
	SyntaxElem sElem_SVARDEC_6_1		= {SELEMOP_ALL_OF,			4, {SVARNAMES, TCOLON, STYPE, TSEMI}};
	SyntaxElem sElem_SVARNAMES			= {SELEMOP_ALL_OF,			2, {SVARNAME, SVARNAMES_2}};
	SyntaxElem sElem_SVARNAMES_2		= {SELEMOP_ZERO_OR_MORE,	1, {SVARNAMES_2_1}};
	SyntaxElem sElem_SVARNAMES_2_1		= {SELEMOP_ALL_OF,			2, {TCOMMA, SVARNAME}};
	SyntaxElem sElem_SVARNAME			= {SELEMOP_ALL_OF,			1, {TNAME}};
	SyntaxElem sElem_STYPE				= {SELEMOP_ONE_OF,			2, {SSTDTYPE, SARRTYPE}};
	SyntaxElem sElem_SSTDTYPE			= {SELEMOP_ONE_OF,			3, {TINTEGER, TBOOLEAN, TCHAR}};
	SyntaxElem sElem_SARRTYPE			= {SELEMOP_ALL_OF,			6, {TARRAY, TLSQPAREN, TNUMBER, TRSQPAREN, TOF, SSTDTYPE}};
	SyntaxElem sElem_SSUBPROGDEC		= {SELEMOP_ALL_OF,			7, {TPROCEDURE, SPROCEDURENAME, SSUBPROGDEC_3, TSEMI, SSUBPROGDEC_5, SCOMPSTAT, TSEMI}};
	SyntaxElem sElem_SSUBPROGDEC_3		= {SELEMOP_ZERO_OR_ONE,		1, {SFORMPARAM}};
	SyntaxElem sElem_SSUBPROGDEC_5		= {SELEMOP_ZERO_OR_ONE,		1, {SVARDEC}};
	SyntaxElem sElem_SPROCEDURENAME		= {SELEMOP_ALL_OF,			1, {TNAME}};
	SyntaxElem sElem_SFORMPARAM			= {SELEMOP_ALL_OF,			6, {TLPAREN, SVARNAMES, TCOLON, STYPE, SFORMPARAM_5, TRPAREN}};
	SyntaxElem sElem_SFORMPARAM_5		= {SELEMOP_ZERO_OR_MORE,	1, {SFORMPARAM_5_1}};
	SyntaxElem sElem_SFORMPARAM_5_1		= {SELEMOP_ALL_OF,			4, {TSEMI, SVARNAMES, TCOLON, STYPE}};
	SyntaxElem sElem_SCOMPSTAT			= {SELEMOP_ALL_OF,			4, {TBEGIN, SSTAT, SCOMPSTAT_3, TEND}};
	SyntaxElem sElem_SCOMPSTAT_3		= {SELEMOP_ZERO_OR_MORE,	1, {SCOMPSTAT_3_1}};
	SyntaxElem sElem_SCOMPSTAT_3_1		= {SELEMOP_ALL_OF,			2, {TSEMI, SSTAT}};
	SyntaxElem sElem_SSTAT				= {SELEMOP_ONE_OF,			10,{SASSIGNSTAT, SCONDSTAT, SITERSTAT, SEXITSTAT, SCALLSTAT, SRETSTAT, SINSTAT, SOUTSTAT, SCOMPSTAT, SEMPTYSTAT}};
	SyntaxElem sElem_SCONDSTAT			= {SELEMOP_ALL_OF,			5, {TIF, SEXPR, TTHEN, SSTAT, SCONDSTAT_5}};
	SyntaxElem sElem_SCONDSTAT_5		= {SELEMOP_ZERO_OR_ONE,		1, {SCONDSTAT_5_1}};
	SyntaxElem sElem_SCONDSTAT_5_1		= {SELEMOP_ALL_OF,			2, {TELSE, SSTAT}};
	SyntaxElem sElem_SITERSTAT			= {SELEMOP_ALL_OF,			4, {TWHILE, SEXPR, TDO, SSTAT}};
	SyntaxElem sElem_SEXITSTAT			= {SELEMOP_ALL_OF,			1, {TBREAK}};
	SyntaxElem sElem_SCALLSTAT			= {SELEMOP_ALL_OF,			3, {TCALL, SPROCEDURENAME, SCALLSTAT_3}};
	SyntaxElem sElem_SCALLSTAT_3		= {SELEMOP_ZERO_OR_ONE,		1, {SCALLSTAT_3_1}};
	SyntaxElem sElem_SCALLSTAT_3_1		= {SELEMOP_ALL_OF,			3, {TLPAREN, SEXPRS, TRPAREN}};
	SyntaxElem sElem_SEXPRS				= {SELEMOP_ALL_OF,			2, {SEXPR, SEXPRS_2}};
	SyntaxElem sElem_SEXPRS_2			= {SELEMOP_ZERO_OR_MORE,	1, {SEXPRS_2_1}};
	SyntaxElem sElem_SEXPRS_2_1			= {SELEMOP_ALL_OF,			2, {TCOMMA, SEXPR}};
	SyntaxElem sElem_SRETSTAT			= {SELEMOP_ALL_OF,			1, {TRETURN}};
	SyntaxElem sElem_SASSIGNSTAT		= {SELEMOP_ALL_OF,			3, {SLEFTPART, TASSIGN, SEXPR}};
	SyntaxElem sElem_SLEFTPART			= {SELEMOP_ALL_OF,			1, {SVAR}};
	SyntaxElem sElem_SVAR				= {SELEMOP_ALL_OF,			2, {SVARNAME, SVAR_2}};
	SyntaxElem sElem_SVAR_2				= {SELEMOP_ZERO_OR_ONE,		1, {SVAR_2_1}};
	SyntaxElem sElem_SVAR_2_1			= {SELEMOP_ALL_OF,			3, {TLSQPAREN, SEXPR, TRSQPAREN}};
	SyntaxElem sElem_SEXPR				= {SELEMOP_ALL_OF,			2, {SSIMPLEEXPR, SEXPR_2}};
	SyntaxElem sElem_SEXPR_2			= {SELEMOP_ZERO_OR_MORE,	1, {SEXPR_2_1}};
	SyntaxElem sElem_SEXPR_2_1			= {SELEMOP_ALL_OF,			2, {SRELATOP, SSIMPLEEXPR}};
	SyntaxElem sElem_SSIMPLEEXPR		= {SELEMOP_ALL_OF,			3, {SSIMPLEEXPR_1, STERM, SSIMPLEEXPR_3}};
	SyntaxElem sElem_SSIMPLEEXPR_1		= {SELEMOP_ZERO_OR_ONE,		1, {SSIMPLEEXPR_1_1}};
	SyntaxElem sElem_SSIMPLEEXPR_1_1	= {SELEMOP_ONE_OF,			2, {TPLUS, TMINUS}};
	SyntaxElem sElem_SSIMPLEEXPR_3		= {SELEMOP_ZERO_OR_MORE,	1, {SSIMPLEEXPR_3_1}};
	SyntaxElem sElem_SSIMPLEEXPR_3_1	= {SELEMOP_ALL_OF,			2, {SADDOP, STERM}};
	SyntaxElem sElem_STERM				= {SELEMOP_ALL_OF,			2, {SFACTOR, STERM_2}};
	SyntaxElem sElem_STERM_2			= {SELEMOP_ZERO_OR_MORE,	1, {STERM_2_1}};
	SyntaxElem sElem_STERM_2_1			= {SELEMOP_ALL_OF,			2, {SMULOP, SFACTOR}};
	SyntaxElem sElem_SFACTOR			= {SELEMOP_ONE_OF,			5, {SVAR, SCONST, SFACTOR_3, SFACTOR_4, SFACTOR_5}};
	SyntaxElem sElem_SFACTOR_3			= {SELEMOP_ALL_OF,			3, {TLPAREN, SEXPR, TRPAREN}};
	SyntaxElem sElem_SFACTOR_4			= {SELEMOP_ALL_OF,			2, {TNOT, SFACTOR}};
	SyntaxElem sElem_SFACTOR_5			= {SELEMOP_ALL_OF,			4, {SSTDTYPE, TLPAREN, SEXPR, TRPAREN}};
	SyntaxElem sElem_SCONST				= {SELEMOP_ONE_OF,			4, {TNUMBER, TFALSE, TTRUE, TSTRING}};
	SyntaxElem sElem_SMULOP				= {SELEMOP_ONE_OF,			3, {TSTAR, TDIV, TAND}};
	SyntaxElem sElem_SADDOP				= {SELEMOP_ONE_OF,			3, {TPLUS, TMINUS, TOR}};
	SyntaxElem sElem_SRELATOP			= {SELEMOP_ONE_OF,			6, {TEQUAL, TNOTEQ, TLE, TLEEQ, TGR, TGREQ}};
	SyntaxElem sElem_SINSTAT			= {SELEMOP_ALL_OF,			2, {SINSTAT_1, SINSTAT_2}};
	SyntaxElem sElem_SINSTAT_1			= {SELEMOP_ONE_OF,			2, {TREAD, TREADLN}};
	SyntaxElem sElem_SINSTAT_2			= {SELEMOP_ZERO_OR_ONE,		1, {SINSTAT_2_1}};
	SyntaxElem sElem_SINSTAT_2_1		= {SELEMOP_ALL_OF,			4, {TLPAREN, SVAR, SINSTAT_2_1_3, TRPAREN}};
	SyntaxElem sElem_SINSTAT_2_1_3		= {SELEMOP_ZERO_OR_MORE,	1, {SINSTAT_2_1_3_1}};
	SyntaxElem sElem_SINSTAT_2_1_3_1	= {SELEMOP_ALL_OF,			2, {TCOMMA, SVAR}};
	SyntaxElem sElem_SOUTSTAT			= {SELEMOP_ALL_OF,			2, {SOUTSTAT_1, SOUTSTAT_2}};
	SyntaxElem sElem_SOUTSTAT_1			= {SELEMOP_ONE_OF,			2, {TWRITE, TWRITELN}};
	SyntaxElem sElem_SOUTSTAT_2			= {SELEMOP_ZERO_OR_ONE,		1, {SOUTSTAT_2_1}};
	SyntaxElem sElem_SOUTSTAT_2_1		= {SELEMOP_ALL_OF,			4, {TLPAREN, SOUTFORM, SOUTSTAT_2_1_3, TRPAREN}};
	SyntaxElem sElem_SOUTSTAT_2_1_3		= {SELEMOP_ZERO_OR_MORE,	1, {SOUTSTAT_2_1_3_1}};
	SyntaxElem sElem_SOUTSTAT_2_1_3_1	= {SELEMOP_ALL_OF,			2, {TCOMMA, SOUTFORM}};
	SyntaxElem sElem_SOUTFORM			= {SELEMOP_ONE_OF,			2, {SOUTFORM_1, TSTRING}};
	SyntaxElem sElem_SOUTFORM_1			= {SELEMOP_ALL_OF,			2, {SEXPR, SOUTFORM_1_2}};
	SyntaxElem sElem_SOUTFORM_1_2		= {SELEMOP_ZERO_OR_ONE,		1, {SOUTFORM_1_2_1}};
	SyntaxElem sElem_SOUTFORM_1_2_1		= {SELEMOP_ALL_OF,			2, {TCOLON, TNUMBER}};
	SyntaxElem sElem_SEMPTYSTAT			= {SELEMOP_TERMINATOR,		0, {}};
	/*
	syntaxElems[TNAME]				= sElem_TNAME;
	syntaxElems[TPROGRAM]			= sElem_TPROGRAM;
	syntaxElems[TVAR]				= sElem_TVAR;
	syntaxElems[TARRAY]				= sElem_TARRAY;
	syntaxElems[TOF]				= sElem_TOF;
	syntaxElems[TBEGIN]				= sElem_TBEGIN;
	syntaxElems[TEND]				= sElem_TEND;
	syntaxElems[TIF]				= sElem_TIF;
	syntaxElems[TTHEN]				= sElem_TTHEN;
	syntaxElems[TELSE]				= sElem_TELSE;
	syntaxElems[TPROCEDURE]			= sElem_TPROCEDURE;
	syntaxElems[TRETURN]			= sElem_TRETURN;
	syntaxElems[TCALL]				= sElem_TCALL;
	syntaxElems[TWHILE]				= sElem_TWHILE;
	syntaxElems[TDO]				= sElem_TDO;
	syntaxElems[TNOT]				= sElem_TNOT;
	syntaxElems[TOR]				= sElem_TOR;
	syntaxElems[TDIV]				= sElem_TDIV;
	syntaxElems[TAND]				= sElem_TAND;
	syntaxElems[TCHAR]				= sElem_TCHAR;
	syntaxElems[TINTEGER]			= sElem_TINTEGER;
	syntaxElems[TBOOLEAN]			= sElem_TBOOLEAN;
	syntaxElems[TREADLN]			= sElem_TREADLN;
	syntaxElems[TWRITELN]			= sElem_TWRITELN;
	syntaxElems[TTRUE]				= sElem_TTRUE;
	syntaxElems[TFALSE]				= sElem_TFALSE;
	syntaxElems[TNUMBER]			= sElem_TNUMBER;
	syntaxElems[TSTRING]			= sElem_TSTRING;
	syntaxElems[TPLUS]				= sElem_TPLUS;
	syntaxElems[TMINUS]				= sElem_TMINUS;
	syntaxElems[TSTAR]				= sElem_TSTAR;
	syntaxElems[TEQUAL]				= sElem_TEQUAL;
	syntaxElems[TNOTEQ]				= sElem_TNOTEQ;
	syntaxElems[TLE]				= sElem_TLE;
	syntaxElems[TLEEQ]				= sElem_TLEEQ;
	syntaxElems[TGR]				= sElem_TGR;
	syntaxElems[TGREQ]				= sElem_TGREQ;
	syntaxElems[TLPAREN]			= sElem_TLPAREN;
	syntaxElems[TRPAREN]			= sElem_TRPAREN;
	syntaxElems[TLSQPAREN]			= sElem_TLSQPAREN;
	syntaxElems[TRSQPAREN]			= sElem_TRSQPAREN;
	syntaxElems[TASSIGN]			= sElem_TASSIGN;
	syntaxElems[TDOT]				= sElem_TDOT;
	syntaxElems[TCOMMA]				= sElem_TCOMMA;
	syntaxElems[TCOLON]				= sElem_TCOLON;
	syntaxElems[TSEMI]				= sElem_TSEMI;
	syntaxElems[TREAD]				= sElem_TREAD;
	syntaxElems[TWRITE]				= sElem_TWRITE;
	syntaxElems[TBREAK]				= sElem_TBREAK;
	*/
	syntaxElems[SPROGRAM]			= sElem_SPROGRAM;
	syntaxElems[SBLOCK]				= sElem_SBLOCK;
	syntaxElems[SBLOCK_1]			= sElem_SBLOCK_1;
	syntaxElems[SBLOCK_1_1]			= sElem_SBLOCK_1_1;
	syntaxElems[SVARDEC]			= sElem_SVARDEC;
	syntaxElems[SVARDEC_6]			= sElem_SVARDEC_6;
	syntaxElems[SVARDEC_6_1]		= sElem_SVARDEC_6_1;
	syntaxElems[SVARNAMES]			= sElem_SVARNAMES;
	syntaxElems[SVARNAMES_2]		= sElem_SVARNAMES_2;
	syntaxElems[SVARNAMES_2_1]		= sElem_SVARNAMES_2_1;
	syntaxElems[SVARNAME]			= sElem_SVARNAME;
	syntaxElems[STYPE]				= sElem_STYPE;
	syntaxElems[SSTDTYPE]			= sElem_SSTDTYPE;
	syntaxElems[SARRTYPE]			= sElem_SARRTYPE;
	syntaxElems[SSUBPROGDEC]		= sElem_SSUBPROGDEC;
	syntaxElems[SSUBPROGDEC_3]		= sElem_SSUBPROGDEC_3;
	syntaxElems[SSUBPROGDEC_5]		= sElem_SSUBPROGDEC_5;
	syntaxElems[SPROCEDURENAME]		= sElem_SPROCEDURENAME;
	syntaxElems[SFORMPARAM]			= sElem_SFORMPARAM;
	syntaxElems[SFORMPARAM_5]		= sElem_SFORMPARAM_5;
	syntaxElems[SFORMPARAM_5_1]		= sElem_SFORMPARAM_5_1;
	syntaxElems[SCOMPSTAT]			= sElem_SCOMPSTAT;
	syntaxElems[SCOMPSTAT_3]		= sElem_SCOMPSTAT_3;
	syntaxElems[SCOMPSTAT_3_1]		= sElem_SCOMPSTAT_3_1;
	syntaxElems[SSTAT]				= sElem_SSTAT;
	syntaxElems[SCONDSTAT]			= sElem_SCONDSTAT;
	syntaxElems[SCONDSTAT_5]		= sElem_SCONDSTAT_5;
	syntaxElems[SCONDSTAT_5_1]		= sElem_SCONDSTAT_5_1;
	syntaxElems[SITERSTAT]			= sElem_SITERSTAT;
	syntaxElems[SEXITSTAT]			= sElem_SEXITSTAT;
	syntaxElems[SCALLSTAT]			= sElem_SCALLSTAT;
	syntaxElems[SCALLSTAT_3]		= sElem_SCALLSTAT_3;
	syntaxElems[SCALLSTAT_3_1]		= sElem_SCALLSTAT_3_1;
	syntaxElems[SEXPRS]				= sElem_SEXPRS;
	syntaxElems[SEXPRS_2]			= sElem_SEXPRS_2;
	syntaxElems[SEXPRS_2_1]			= sElem_SEXPRS_2_1;
	syntaxElems[SRETSTAT]			= sElem_SRETSTAT;
	syntaxElems[SASSIGNSTAT]		= sElem_SASSIGNSTAT;
	syntaxElems[SLEFTPART]			= sElem_SLEFTPART;
	syntaxElems[SVAR]				= sElem_SVAR;
	syntaxElems[SVAR_2]				= sElem_SVAR_2;
	syntaxElems[SVAR_2_1]			= sElem_SVAR_2_1;
	syntaxElems[SEXPR]				= sElem_SEXPR;
	syntaxElems[SEXPR_2]			= sElem_SEXPR_2;
	syntaxElems[SEXPR_2_1]			= sElem_SEXPR_2_1;
	syntaxElems[SSIMPLEEXPR]		= sElem_SSIMPLEEXPR;
	syntaxElems[SSIMPLEEXPR_1]		= sElem_SSIMPLEEXPR_1;
	syntaxElems[SSIMPLEEXPR_1_1]	= sElem_SSIMPLEEXPR_1_1;
	syntaxElems[SSIMPLEEXPR_3]		= sElem_SSIMPLEEXPR_3;
	syntaxElems[SSIMPLEEXPR_3_1]	= sElem_SSIMPLEEXPR_3_1;
	syntaxElems[STERM]				= sElem_STERM;
	syntaxElems[STERM_2]			= sElem_STERM_2;
	syntaxElems[STERM_2_1]			= sElem_STERM_2_1;
	syntaxElems[SFACTOR]			= sElem_SFACTOR;
	syntaxElems[SFACTOR_3]			= sElem_SFACTOR_3;
	syntaxElems[SFACTOR_4]			= sElem_SFACTOR_4;
	syntaxElems[SFACTOR_5]			= sElem_SFACTOR_5;
	syntaxElems[SCONST]				= sElem_SCONST;
	syntaxElems[SMULOP]				= sElem_SMULOP;
	syntaxElems[SADDOP]				= sElem_SADDOP;
	syntaxElems[SRELATOP]			= sElem_SRELATOP;
	syntaxElems[SINSTAT]			= sElem_SINSTAT;
	syntaxElems[SINSTAT_1]			= sElem_SINSTAT_1;
	syntaxElems[SINSTAT_2]			= sElem_SINSTAT_2;
	syntaxElems[SINSTAT_2_1]		= sElem_SINSTAT_2_1;
	syntaxElems[SINSTAT_2_1_3]		= sElem_SINSTAT_2_1_3;
	syntaxElems[SINSTAT_2_1_3_1]	= sElem_SINSTAT_2_1_3_1;
	syntaxElems[SOUTSTAT]			= sElem_SOUTSTAT;
	syntaxElems[SOUTSTAT_1]			= sElem_SOUTSTAT_1;
	syntaxElems[SOUTSTAT_2]			= sElem_SOUTSTAT_2;
	syntaxElems[SOUTSTAT_2_1]		= sElem_SOUTSTAT_2_1;
	syntaxElems[SOUTSTAT_2_1_3]		= sElem_SOUTSTAT_2_1_3;
	syntaxElems[SOUTSTAT_2_1_3_1]	= sElem_SOUTSTAT_2_1_3_1;
	syntaxElems[SOUTFORM]			= sElem_SOUTFORM;
	syntaxElems[SOUTFORM_1]			= sElem_SOUTFORM_1;
	syntaxElems[SOUTFORM_1_2]		= sElem_SOUTFORM_1_2;
	syntaxElems[SOUTFORM_1_2_1]		= sElem_SOUTFORM_1_2_1;
	syntaxElems[SEMPTYSTAT]			= sElem_SEMPTYSTAT;
}

SyntaxTreeNode* parse(int sElemIt, int indent_depth){
	int i;
	SyntaxElem sElem = syntaxElems[sElemIt];

	SyntaxTreeNode* this = malloc_tree_node();
	this->sElemIt = sElemIt;
	this->indent_depth = indent_depth;

	switch(sElem.op){
	SyntaxTreeNode* newest_child = NULL;

	/* check 1. to meet TERMINATOR */
	case SELEMOP_TERMINATOR:
		/* empry stat	: return empty */
		if(sElemIt == SEMPTYSTAT){
			this->parse_result = PARSERESULT_EMPTY;
			return this;
		}
		/* other		: compare token and elem */
		else if(token == sElemIt){
			this->parse_result = PARSERESULT_MATCH;
			this->sElemIt = sElemIt;
			strcpy(this->string_attr, string_attr);
			token = scan();
			return this;
		}
		else {
			this->parse_result = PARSERESULT_NOTMATCH;
			return this;
		}

	/* check 2. to meet ALL OF the conditions */
	case SELEMOP_ALL_OF:
		for(i = 0; i < sElem.childrenNum; i++){
			int indent = indent_depth + is_indent(sElemIt, i);
			SyntaxTreeNode* child = parse(sElem.children[i], indent);

			if(this->child == NULL){
				this->child = child;
				newest_child = this->child;
			}
			else{
				newest_child->brother = child;
				newest_child = newest_child->brother;
			}

			switch(child->parse_result){

			/* one of children is not match : NOT MATCH */
			case PARSERESULT_NOTMATCH:
				this->parse_result = PARSERESULT_NOTMATCH;
				return this;

			/* one of children is empty : continue */
			case PARSERESULT_EMPTY:
				break;

			/* one of children is match : schedule return MATCH and continue */
			case PARSERESULT_MATCH:
				this->parse_result = PARSERESULT_MATCH;
				break;
			}
		}
		return this;

	/* check 3. to meet ONE OF the conditions */
	case SELEMOP_ONE_OF:
		for(i = 0; i < sElem.childrenNum; i++){
			int indent = indent_depth + is_indent(sElemIt, i);
			SyntaxTreeNode* child = parse(sElem.children[i], indent);

			if(this->child == NULL){
				this->child = child;
				newest_child = this->child;
			}
			else{
				newest_child->brother = child;
				newest_child = newest_child->brother;
			}

			switch(child->parse_result){

			/* one of children is match : MATCH */
			case PARSERESULT_MATCH:				
				this->parse_result = PARSERESULT_MATCH;
				return this;

			/* one of children is empty : schedule return MATCH and continue */
			case PARSERESULT_EMPTY:
				this->parse_result = PARSERESULT_EMPTY;
				break;

			/* one of children is not match : continue */
			}
		}
		return this;

	/* check 4. to meet the condition ZERO OR MORE times */
	case SELEMOP_ZERO_OR_MORE:
		/* match 1 or more time : return MATCH */
		/* other : return EMPTY */
		this->parse_result = PARSERESULT_EMPTY;
		while(1){
			int indent = indent_depth + is_indent(sElemIt, 0);
			SyntaxTreeNode* child = parse(sElem.children[0], indent);
			if(this->child == NULL){
				this->child = child;
				newest_child = this->child;
			}
			else{
				newest_child->brother = child;
				newest_child = newest_child->brother;
			}
			if(!child->parse_result){
				break;
			}
			this->parse_result = PARSERESULT_MATCH;
		}
		return this;

	/* check 5. to meet the condition ZERO OR ONE time */
	case SELEMOP_ZERO_OR_ONE:
		this->child = parse(sElem.children[0], indent_depth + is_indent(sElemIt, 0));
		if(this->child->parse_result) this->parse_result = PARSERESULT_MATCH;
		else this->parse_result = PARSERESULT_EMPTY;
		return this;
	}

	/* nobody can come here just for debug */
	return 0;
}
