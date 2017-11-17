#include "pretty-printer.h"

/* const */ SyntaxElem s_elem_array[NUMOFSYNTAX + 1];
int token;

void init_parse(void) {
	int i;
	token = scan();

	for(i = 0; i < NUMOFSYNTAX; i++){
		SyntaxElem hoge = {SELEMOP_TERMINATOR, 0, {}};
		s_elem_array[i] = hoge;
	}

	/*
	SyntaxElem s_elem_TNAME				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TPROGRAM			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TVAR				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TARRAY			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TOF				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TBEGIN			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TEND				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TIF				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TTHEN				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TELSE				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TPROCEDURE		= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TRETURN			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TCALL				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TWHILE			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TDO				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TNOT				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TOR				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TDIV				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TAND				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TCHAR				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TINTEGER			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TBOOLEAN			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TREADLN			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TWRITELN			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TTRUE				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TFALSE			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TNUMBER			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TSTRING			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TPLUS				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TMINUS			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TSTAR				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TEQUAL			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TNOTEQ			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TLE				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TLEEQ				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TGR				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TGREQ				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TLPAREN			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TRPAREN			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TLSQPAREN			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TRSQPAREN			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TASSIGN			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TDOT				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TCOMMA			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TCOLON			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TSEMI				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TREAD				= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TWRITE			= {SELEMOP_TERMINATOR,		0, {}};
	SyntaxElem s_elem_TBREAK			= {SELEMOP_TERMINATOR,		0, {}};
	*/
	SyntaxElem s_elem_SPROGRAM			= {SELEMOP_ALL_OF,			5, {TPROGRAM, TNAME, TSEMI, SBLOCK, TDOT}};
	SyntaxElem s_elem_SBLOCK			= {SELEMOP_ALL_OF,			2, {SBLOCK_0, SCOMPSTAT}};
	SyntaxElem s_elem_SBLOCK_0			= {SELEMOP_ZERO_OR_MORE,	1, {SBLOCK_0_0}};
	SyntaxElem s_elem_SBLOCK_0_0		= {SELEMOP_ONE_OF,			2, {SVARDEC, SSUBPROGDEC}};
	SyntaxElem s_elem_SVARDEC			= {SELEMOP_ALL_OF,			6, {TVAR, SVARNAMES, TCOLON, STYPE, TSEMI, SVARDEC_5}};
	SyntaxElem s_elem_SVARDEC_5			= {SELEMOP_ZERO_OR_MORE,	1, {SVARDEC_5_0}};
	SyntaxElem s_elem_SVARDEC_5_0		= {SELEMOP_ALL_OF,			4, {SVARNAMES, TCOLON, STYPE, TSEMI}};
	SyntaxElem s_elem_SVARNAMES			= {SELEMOP_ALL_OF,			2, {SVARNAME, SVARNAMES_1}};
	SyntaxElem s_elem_SVARNAMES_1		= {SELEMOP_ZERO_OR_MORE,	1, {SVARNAMES_1_0}};
	SyntaxElem s_elem_SVARNAMES_1_0		= {SELEMOP_ALL_OF,			2, {TCOMMA, SVARNAME}};
	SyntaxElem s_elem_SVARNAME			= {SELEMOP_ALL_OF,			1, {TNAME}};
	SyntaxElem s_elem_STYPE				= {SELEMOP_ONE_OF,			2, {SSTDTYPE, SARRTYPE}};
	SyntaxElem s_elem_SSTDTYPE			= {SELEMOP_ONE_OF,			3, {TINTEGER, TBOOLEAN, TCHAR}};
	SyntaxElem s_elem_SARRTYPE			= {SELEMOP_ALL_OF,			6, {TARRAY, TLSQPAREN, TNUMBER, TRSQPAREN, TOF, SSTDTYPE}};
	SyntaxElem s_elem_SSUBPROGDEC		= {SELEMOP_ALL_OF,			7, {TPROCEDURE, SPROCEDURENAME, SSUBPROGDEC_2, TSEMI, SSUBPROGDEC_4, SCOMPSTAT, TSEMI}};
	SyntaxElem s_elem_SSUBPROGDEC_2		= {SELEMOP_ZERO_OR_ONE,		1, {SFORMPARAM}};
	SyntaxElem s_elem_SSUBPROGDEC_4		= {SELEMOP_ZERO_OR_ONE,		1, {SVARDEC}};
	SyntaxElem s_elem_SPROCEDURENAME	= {SELEMOP_ALL_OF,			1, {TNAME}};
	SyntaxElem s_elem_SFORMPARAM		= {SELEMOP_ALL_OF,			6, {TLPAREN, SVARNAMES, TCOLON, STYPE, SFORMPARAM_4, TRPAREN}};
	SyntaxElem s_elem_SFORMPARAM_4		= {SELEMOP_ZERO_OR_MORE,	1, {SFORMPARAM_4_0}};
	SyntaxElem s_elem_SFORMPARAM_4_0	= {SELEMOP_ALL_OF,			4, {TSEMI, SVARNAMES, TCOLON, STYPE}};
	SyntaxElem s_elem_SCOMPSTAT			= {SELEMOP_ALL_OF,			4, {TBEGIN, SSTAT, SCOMPSTAT_2, TEND}};
	SyntaxElem s_elem_SCOMPSTAT_2		= {SELEMOP_ZERO_OR_MORE,	1, {SCOMPSTAT_2_0}};
	SyntaxElem s_elem_SCOMPSTAT_2_0		= {SELEMOP_ALL_OF,			2, {TSEMI, SSTAT}};
	SyntaxElem s_elem_SSTAT				= {SELEMOP_ONE_OF,			10,{SASSIGNSTAT, SCONDSTAT, SITERSTAT, SEXITSTAT, SCALLSTAT, SRETSTAT, SINSTAT, SOUTSTAT, SCOMPSTAT, SEMPTYSTAT}};
	SyntaxElem s_elem_SCONDSTAT			= {SELEMOP_ALL_OF,			5, {TIF, SEXPR, TTHEN, SSTAT, SCONDSTAT_4}};
	SyntaxElem s_elem_SCONDSTAT_4		= {SELEMOP_ZERO_OR_ONE,		1, {SCONDSTAT_4_0}};
	SyntaxElem s_elem_SCONDSTAT_4_0		= {SELEMOP_ALL_OF,			2, {TELSE, SSTAT}};
	SyntaxElem s_elem_SITERSTAT			= {SELEMOP_ALL_OF,			4, {TWHILE, SEXPR, TDO, SSTAT}};
	SyntaxElem s_elem_SEXITSTAT			= {SELEMOP_ALL_OF,			1, {TBREAK}};
	SyntaxElem s_elem_SCALLSTAT			= {SELEMOP_ALL_OF,			3, {TCALL, SPROCEDURENAME, SCALLSTAT_2}};
	SyntaxElem s_elem_SCALLSTAT_2		= {SELEMOP_ZERO_OR_ONE,		1, {SCALLSTAT_2_0}};
	SyntaxElem s_elem_SCALLSTAT_2_0		= {SELEMOP_ALL_OF,			3, {TLPAREN, SEXPRS, TRPAREN}};
	SyntaxElem s_elem_SEXPRS			= {SELEMOP_ALL_OF,			2, {SEXPR, SEXPRS_1}};
	SyntaxElem s_elem_SEXPRS_1			= {SELEMOP_ZERO_OR_MORE,	1, {SEXPRS_1_0}};
	SyntaxElem s_elem_SEXPRS_1_0		= {SELEMOP_ALL_OF,			2, {TCOMMA, SEXPR}};
	SyntaxElem s_elem_SRETSTAT			= {SELEMOP_ALL_OF,			1, {TRETURN}};
	SyntaxElem s_elem_SASSIGNSTAT		= {SELEMOP_ALL_OF,			3, {SLEFTPART, TASSIGN, SEXPR}};
	SyntaxElem s_elem_SLEFTPART			= {SELEMOP_ALL_OF,			1, {SVAR}};
	SyntaxElem s_elem_SVAR				= {SELEMOP_ALL_OF,			2, {SVARNAME, SVAR_1}};
	SyntaxElem s_elem_SVAR_1			= {SELEMOP_ZERO_OR_ONE,		1, {SVAR_1_0}};
	SyntaxElem s_elem_SVAR_1_0			= {SELEMOP_ALL_OF,			3, {TLSQPAREN, SEXPR, TRSQPAREN}};
	SyntaxElem s_elem_SEXPR				= {SELEMOP_ALL_OF,			2, {SSIMPLEEXPR, SEXPR_1}};
	SyntaxElem s_elem_SEXPR_1			= {SELEMOP_ZERO_OR_MORE,	1, {SEXPR_1_0}};
	SyntaxElem s_elem_SEXPR_1_0			= {SELEMOP_ALL_OF,			2, {SRELATOP, SSIMPLEEXPR}};
	SyntaxElem s_elem_SSIMPLEEXPR		= {SELEMOP_ALL_OF,			3, {SSIMPLEEXPR_0, STERM, SSIMPLEEXPR_2}};
	SyntaxElem s_elem_SSIMPLEEXPR_0		= {SELEMOP_ZERO_OR_ONE,		1, {SSIMPLEEXPR_0_0}};
	SyntaxElem s_elem_SSIMPLEEXPR_0_0	= {SELEMOP_ONE_OF,			2, {TPLUS, TMINUS}};
	SyntaxElem s_elem_SSIMPLEEXPR_2		= {SELEMOP_ZERO_OR_MORE,	1, {SSIMPLEEXPR_2_0}};
	SyntaxElem s_elem_SSIMPLEEXPR_2_0	= {SELEMOP_ALL_OF,			2, {SADDOP, STERM}};
	SyntaxElem s_elem_STERM				= {SELEMOP_ALL_OF,			2, {SFACTOR, STERM_1}};
	SyntaxElem s_elem_STERM_1			= {SELEMOP_ZERO_OR_MORE,	1, {STERM_1_0}};
	SyntaxElem s_elem_STERM_1_0			= {SELEMOP_ALL_OF,			2, {SMULOP, SFACTOR}};
	SyntaxElem s_elem_SFACTOR			= {SELEMOP_ONE_OF,			5, {SVAR, SCONST, SFACTOR_2, SFACTOR_3, SFACTOR_4}};
	SyntaxElem s_elem_SFACTOR_2			= {SELEMOP_ALL_OF,			3, {TLPAREN, SEXPR, TRPAREN}};
	SyntaxElem s_elem_SFACTOR_3			= {SELEMOP_ALL_OF,			2, {TNOT, SFACTOR}};
	SyntaxElem s_elem_SFACTOR_4			= {SELEMOP_ALL_OF,			4, {SSTDTYPE, TLPAREN, SEXPR, TRPAREN}};
	SyntaxElem s_elem_SCONST			= {SELEMOP_ONE_OF,			4, {TNUMBER, TFALSE, TTRUE, TSTRING}};
	SyntaxElem s_elem_SMULOP			= {SELEMOP_ONE_OF,			3, {TSTAR, TDIV, TAND}};
	SyntaxElem s_elem_SADDOP			= {SELEMOP_ONE_OF,			3, {TPLUS, TMINUS, TOR}};
	SyntaxElem s_elem_SRELATOP			= {SELEMOP_ONE_OF,			6, {TEQUAL, TNOTEQ, TLE, TLEEQ, TGR, TGREQ}};
	SyntaxElem s_elem_SINSTAT			= {SELEMOP_ALL_OF,			2, {SINSTAT_0, SINSTAT_1}};
	SyntaxElem s_elem_SINSTAT_0			= {SELEMOP_ONE_OF,			2, {TREAD, TREADLN}};
	SyntaxElem s_elem_SINSTAT_1			= {SELEMOP_ZERO_OR_ONE,		1, {SINSTAT_1_0}};
	SyntaxElem s_elem_SINSTAT_1_0		= {SELEMOP_ALL_OF,			4, {TLPAREN, SVAR, SINSTAT_1_0_2, TRPAREN}};
	SyntaxElem s_elem_SINSTAT_1_0_2		= {SELEMOP_ZERO_OR_MORE,	1, {SINSTAT_1_0_2_0}};
	SyntaxElem s_elem_SINSTAT_1_0_2_0	= {SELEMOP_ALL_OF,			2, {TCOMMA, SVAR}};
	SyntaxElem s_elem_SOUTSTAT			= {SELEMOP_ALL_OF,			2, {SOUTSTAT_0, SOUTSTAT_1}};
	SyntaxElem s_elem_SOUTSTAT_0		= {SELEMOP_ONE_OF,			2, {TWRITE, TWRITELN}};
	SyntaxElem s_elem_SOUTSTAT_1		= {SELEMOP_ZERO_OR_ONE,		1, {SOUTSTAT_1_0}};
	SyntaxElem s_elem_SOUTSTAT_1_0		= {SELEMOP_ALL_OF,			4, {TLPAREN, SOUTFORM, SOUTSTAT_1_0_2, TRPAREN}};
	SyntaxElem s_elem_SOUTSTAT_1_0_2	= {SELEMOP_ZERO_OR_MORE,	1, {SOUTSTAT_1_0_2_0}};
	SyntaxElem s_elem_SOUTSTAT_1_0_2_0	= {SELEMOP_ALL_OF,			2, {TCOMMA, SOUTFORM}};
	SyntaxElem s_elem_SOUTFORM			= {SELEMOP_ONE_OF,			2, {SOUTFORM_0, TSTRING}};
	SyntaxElem s_elem_SOUTFORM_0		= {SELEMOP_ALL_OF,			2, {SEXPR, SOUTFORM_0_1}};
	SyntaxElem s_elem_SOUTFORM_0_1		= {SELEMOP_ZERO_OR_ONE,		1, {SOUTFORM_0_1_0}};
	SyntaxElem s_elem_SOUTFORM_0_1_0	= {SELEMOP_ALL_OF,			2, {TCOLON, TNUMBER}};
	SyntaxElem s_elem_SEMPTYSTAT		= {SELEMOP_TERMINATOR,		0, {}};
	/*
	s_elem_array[TNAME]				= s_elem_TNAME;
	s_elem_array[TPROGRAM]			= s_elem_TPROGRAM;
	s_elem_array[TVAR]				= s_elem_TVAR;
	s_elem_array[TARRAY]			= s_elem_TARRAY;
	s_elem_array[TOF]				= s_elem_TOF;
	s_elem_array[TBEGIN]			= s_elem_TBEGIN;
	s_elem_array[TEND]				= s_elem_TEND;
	s_elem_array[TIF]				= s_elem_TIF;
	s_elem_array[TTHEN]				= s_elem_TTHEN;
	s_elem_array[TELSE]				= s_elem_TELSE;
	s_elem_array[TPROCEDURE]		= s_elem_TPROCEDURE;
	s_elem_array[TRETURN]			= s_elem_TRETURN;
	s_elem_array[TCALL]				= s_elem_TCALL;
	s_elem_array[TWHILE]			= s_elem_TWHILE;
	s_elem_array[TDO]				= s_elem_TDO;
	s_elem_array[TNOT]				= s_elem_TNOT;
	s_elem_array[TOR]				= s_elem_TOR;
	s_elem_array[TDIV]				= s_elem_TDIV;
	s_elem_array[TAND]				= s_elem_TAND;
	s_elem_array[TCHAR]				= s_elem_TCHAR;
	s_elem_array[TINTEGER]			= s_elem_TINTEGER;
	s_elem_array[TBOOLEAN]			= s_elem_TBOOLEAN;
	s_elem_array[TREADLN]			= s_elem_TREADLN;
	s_elem_array[TWRITELN]			= s_elem_TWRITELN;
	s_elem_array[TTRUE]				= s_elem_TTRUE;
	s_elem_array[TFALSE]			= s_elem_TFALSE;
	s_elem_array[TNUMBER]			= s_elem_TNUMBER;
	s_elem_array[TSTRING]			= s_elem_TSTRING;
	s_elem_array[TPLUS]				= s_elem_TPLUS;
	s_elem_array[TMINUS]			= s_elem_TMINUS;
	s_elem_array[TSTAR]				= s_elem_TSTAR;
	s_elem_array[TEQUAL]			= s_elem_TEQUAL;
	s_elem_array[TNOTEQ]			= s_elem_TNOTEQ;
	s_elem_array[TLE]				= s_elem_TLE;
	s_elem_array[TLEEQ]				= s_elem_TLEEQ;
	s_elem_array[TGR]				= s_elem_TGR;
	s_elem_array[TGREQ]				= s_elem_TGREQ;
	s_elem_array[TLPAREN]			= s_elem_TLPAREN;
	s_elem_array[TRPAREN]			= s_elem_TRPAREN;
	s_elem_array[TLSQPAREN]			= s_elem_TLSQPAREN;
	s_elem_array[TRSQPAREN]			= s_elem_TRSQPAREN;
	s_elem_array[TASSIGN]			= s_elem_TASSIGN;
	s_elem_array[TDOT]				= s_elem_TDOT;
	s_elem_array[TCOMMA]			= s_elem_TCOMMA;
	s_elem_array[TCOLON]			= s_elem_TCOLON;
	s_elem_array[TSEMI]				= s_elem_TSEMI;
	s_elem_array[TREAD]				= s_elem_TREAD;
	s_elem_array[TWRITE]			= s_elem_TWRITE;
	s_elem_array[TBREAK]			= s_elem_TBREAK;
	*/
	s_elem_array[SPROGRAM]			= s_elem_SPROGRAM;
	s_elem_array[SBLOCK]			= s_elem_SBLOCK;
	s_elem_array[SBLOCK_0]			= s_elem_SBLOCK_0;
	s_elem_array[SBLOCK_0_0]		= s_elem_SBLOCK_0_0;
	s_elem_array[SVARDEC]			= s_elem_SVARDEC;
	s_elem_array[SVARDEC_5]			= s_elem_SVARDEC_5;
	s_elem_array[SVARDEC_5_0]		= s_elem_SVARDEC_5_0;
	s_elem_array[SVARNAMES]			= s_elem_SVARNAMES;
	s_elem_array[SVARNAMES_1]		= s_elem_SVARNAMES_1;
	s_elem_array[SVARNAMES_1_0]		= s_elem_SVARNAMES_1_0;
	s_elem_array[SVARNAME]			= s_elem_SVARNAME;
	s_elem_array[STYPE]				= s_elem_STYPE;
	s_elem_array[SSTDTYPE]			= s_elem_SSTDTYPE;
	s_elem_array[SARRTYPE]			= s_elem_SARRTYPE;
	s_elem_array[SSUBPROGDEC]		= s_elem_SSUBPROGDEC;
	s_elem_array[SSUBPROGDEC_2]		= s_elem_SSUBPROGDEC_2;
	s_elem_array[SSUBPROGDEC_4]		= s_elem_SSUBPROGDEC_4;
	s_elem_array[SPROCEDURENAME]	= s_elem_SPROCEDURENAME;
	s_elem_array[SFORMPARAM]		= s_elem_SFORMPARAM;
	s_elem_array[SFORMPARAM_4]		= s_elem_SFORMPARAM_4;
	s_elem_array[SFORMPARAM_4_0]	= s_elem_SFORMPARAM_4_0;
	s_elem_array[SCOMPSTAT]			= s_elem_SCOMPSTAT;
	s_elem_array[SCOMPSTAT_2]		= s_elem_SCOMPSTAT_2;
	s_elem_array[SCOMPSTAT_2_0]		= s_elem_SCOMPSTAT_2_0;
	s_elem_array[SSTAT]				= s_elem_SSTAT;
	s_elem_array[SCONDSTAT]			= s_elem_SCONDSTAT;
	s_elem_array[SCONDSTAT_4]		= s_elem_SCONDSTAT_4;
	s_elem_array[SCONDSTAT_4_0]		= s_elem_SCONDSTAT_4_0;
	s_elem_array[SITERSTAT]			= s_elem_SITERSTAT;
	s_elem_array[SEXITSTAT]			= s_elem_SEXITSTAT;
	s_elem_array[SCALLSTAT]			= s_elem_SCALLSTAT;
	s_elem_array[SCALLSTAT_2]		= s_elem_SCALLSTAT_2;
	s_elem_array[SCALLSTAT_2_0]		= s_elem_SCALLSTAT_2_0;
	s_elem_array[SEXPRS]			= s_elem_SEXPRS;
	s_elem_array[SEXPRS_1]			= s_elem_SEXPRS_1;
	s_elem_array[SEXPRS_1_0]		= s_elem_SEXPRS_1_0;
	s_elem_array[SRETSTAT]			= s_elem_SRETSTAT;
	s_elem_array[SASSIGNSTAT]		= s_elem_SASSIGNSTAT;
	s_elem_array[SLEFTPART]			= s_elem_SLEFTPART;
	s_elem_array[SVAR]				= s_elem_SVAR;
	s_elem_array[SVAR_1]			= s_elem_SVAR_1;
	s_elem_array[SVAR_1_0]			= s_elem_SVAR_1_0;
	s_elem_array[SEXPR]				= s_elem_SEXPR;
	s_elem_array[SEXPR_1]			= s_elem_SEXPR_1;
	s_elem_array[SEXPR_1_0]			= s_elem_SEXPR_1_0;
	s_elem_array[SSIMPLEEXPR]		= s_elem_SSIMPLEEXPR;
	s_elem_array[SSIMPLEEXPR_0]		= s_elem_SSIMPLEEXPR_0;
	s_elem_array[SSIMPLEEXPR_0_0]	= s_elem_SSIMPLEEXPR_0_0;
	s_elem_array[SSIMPLEEXPR_2]		= s_elem_SSIMPLEEXPR_2;
	s_elem_array[SSIMPLEEXPR_2_0]	= s_elem_SSIMPLEEXPR_2_0;
	s_elem_array[STERM]				= s_elem_STERM;
	s_elem_array[STERM_1]			= s_elem_STERM_1;
	s_elem_array[STERM_1_0]			= s_elem_STERM_1_0;
	s_elem_array[SFACTOR]			= s_elem_SFACTOR;
	s_elem_array[SFACTOR_2]			= s_elem_SFACTOR_2;
	s_elem_array[SFACTOR_3]			= s_elem_SFACTOR_3;
	s_elem_array[SFACTOR_4]			= s_elem_SFACTOR_4;
	s_elem_array[SCONST]			= s_elem_SCONST;
	s_elem_array[SMULOP]			= s_elem_SMULOP;
	s_elem_array[SADDOP]			= s_elem_SADDOP;
	s_elem_array[SRELATOP]			= s_elem_SRELATOP;
	s_elem_array[SINSTAT]			= s_elem_SINSTAT;
	s_elem_array[SINSTAT_0]			= s_elem_SINSTAT_0;
	s_elem_array[SINSTAT_1]			= s_elem_SINSTAT_1;
	s_elem_array[SINSTAT_1_0]		= s_elem_SINSTAT_1_0;
	s_elem_array[SINSTAT_1_0_2]		= s_elem_SINSTAT_1_0_2;
	s_elem_array[SINSTAT_1_0_2_0]	= s_elem_SINSTAT_1_0_2_0;
	s_elem_array[SOUTSTAT]			= s_elem_SOUTSTAT;
	s_elem_array[SOUTSTAT_0]		= s_elem_SOUTSTAT_0;
	s_elem_array[SOUTSTAT_1]		= s_elem_SOUTSTAT_1;
	s_elem_array[SOUTSTAT_1_0]		= s_elem_SOUTSTAT_1_0;
	s_elem_array[SOUTSTAT_1_0_2]	= s_elem_SOUTSTAT_1_0_2;
	s_elem_array[SOUTSTAT_1_0_2_0]	= s_elem_SOUTSTAT_1_0_2_0;
	s_elem_array[SOUTFORM]			= s_elem_SOUTFORM;
	s_elem_array[SOUTFORM_0]		= s_elem_SOUTFORM_0;
	s_elem_array[SOUTFORM_0_1]		= s_elem_SOUTFORM_0_1;
	s_elem_array[SOUTFORM_0_1_0]	= s_elem_SOUTFORM_0_1_0;
	s_elem_array[SEMPTYSTAT]		= s_elem_SEMPTYSTAT;
}

/* returns added child */
SyntaxTreeNode* add_child(SyntaxTreeNode* this, SyntaxTreeNode* youngest_child, int child_it){
	int iter = this->iter_depth + (this->s_elem_it == SITERSTAT ? 1 : 0);
	SyntaxTreeNode* newborn
		= parse(s_elem_array[this->s_elem_it].children[child_it], iter);

	newborn->parent = this;

	if(youngest_child == NULL){
		this->child = newborn;
	}
	else{
		youngest_child->brother = newborn;
	}
	return newborn;
}

SyntaxTreeNode* parse(int s_elem_it, int iter_depth){
	int i;
	SyntaxTreeNode* youngest_child = NULL;
	SyntaxTreeNode* this = malloc_tree_node();
	this->s_elem_it = s_elem_it;
	this->line_num = line_num;
	this->iter_depth = iter_depth;

	switch(s_elem_array[this->s_elem_it].op){

	/* check 1. to meet TERMINATOR */
	case SELEMOP_TERMINATOR:
		/* empry stat		: return empty */
		if(s_elem_it == SEMPTYSTAT){
			this->parse_result = PARSERESULT_EMPTY;
			return this;
		}
		/* token == elem	: return match */
		else if(token == s_elem_it){
			this->s_elem_it = s_elem_it;
			strcpy(this->string_attr, string_attr);
			
			/* 'break' token must be in iter stat */
			if(token == TBREAK && this->iter_depth <= 0){
				this->parse_result = PARSERESULT_ACCIDENT;
			}
			else{
				this->parse_result = PARSERESULT_MATCH;
				token = scan();
			}
			return this;
		}
		/* token != elem	: return difference */
		else{
			this->parse_result = PARSERESULT_DIFFERENCE;
			return this;
		}

	/* check 2. to meet ALL OF the conditions */
	case SELEMOP_ALL_OF:
		for(i = 0; i < s_elem_array[this->s_elem_it].children_num; i++){
			youngest_child = add_child(this, youngest_child, i);

			switch(youngest_child->parse_result){

			/* one of children is accident : return ACCIDENT */
			case PARSERESULT_ACCIDENT:
				this->parse_result = PARSERESULT_ACCIDENT;
				return this;

			/* one of children is not matched */
			case PARSERESULT_DIFFERENCE:
				if(this->child == youngest_child){
					this->parse_result = PARSERESULT_DIFFERENCE;
				}
				/* now i parse LL(1) syntax rule,
				 * first child is matched but another child is not matched
				 * => an accident happened, stop parsing and
				 */
				else{
					this->parse_result = PARSERESULT_ACCIDENT;
				}
				return this;

			/* one of children is matched : schedule return MATCH and continue */
			case PARSERESULT_MATCH:
				this->parse_result = PARSERESULT_MATCH;
				break;

			/* one of children is empty : continue */
			/*
			case PARSERESULT_EMPTY:
				break;
			*/
			}
		}
		return this;

	/* check 3. to meet ONE OF the conditions */
	case SELEMOP_ONE_OF:
		for(i = 0; i < s_elem_array[this->s_elem_it].children_num; i++){
			youngest_child = add_child(this, youngest_child, i);

			switch(youngest_child->parse_result){

			/* one of children is accident : return ACCIDENT */
			case PARSERESULT_ACCIDENT:
				this->parse_result = PARSERESULT_ACCIDENT;
				return this;

			/* one of children is matched : MATCH */
			case PARSERESULT_MATCH:				
				this->parse_result = PARSERESULT_MATCH;
				return this;

			/* one of children is empty : schedule return MATCH and continue */
			case PARSERESULT_EMPTY:
				this->parse_result = PARSERESULT_EMPTY;
				break;

			/* one of children is not matched : continue */
			/*
			case PARSERESULT_DIFFERENCE:
				break;
			*/
			}
		}
		return this;

	/* check 4. to meet the condition ZERO OR MORE times */
	case SELEMOP_ZERO_OR_MORE:
		/* match 1 or more time : MATCH */
		/* other : EMPTY */
		this->parse_result = PARSERESULT_EMPTY;
		while(1){
			youngest_child = add_child(this, youngest_child, 0);

			if(youngest_child->parse_result == PARSERESULT_ACCIDENT){
				this->parse_result = PARSERESULT_ACCIDENT;
				break;
			}
			else if(youngest_child->parse_result != PARSERESULT_MATCH){
				break;
			}

			this->parse_result = PARSERESULT_MATCH;
		}
		return this;

	/* check 5. to meet the condition ZERO OR ONE time */
	case SELEMOP_ZERO_OR_ONE:
		youngest_child = add_child(this, youngest_child, 0);

		/* match 1 time : MATCH */
		if(youngest_child->parse_result == PARSERESULT_ACCIDENT){
			this->parse_result = PARSERESULT_ACCIDENT;
		}
		else if(youngest_child->parse_result == PARSERESULT_MATCH){
			this->parse_result = PARSERESULT_MATCH;
		}
		/* match 0 time : EMPTY */
		else{
			this->parse_result = PARSERESULT_EMPTY;
		}

		return this;
	}

	printf("nobody can come here just for compile\n");
	exit(-1);
}
