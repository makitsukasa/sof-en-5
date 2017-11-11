#include "pretty-printer.h"

/* const */ SyntaxElem sElems[NUMOFSYNTAX + 1];

int token;

int is_indent(int parentSyntaxElemIt, int childIt){
	if(parentSyntaxElemIt == SBLOCK && childIt == 0)
		return 1;

	if(parentSyntaxElemIt == SSUBPROGDEC && childIt == 4)
		return 1;

	if(parentSyntaxElemIt == SCOMPSTAT && 
		(childIt == 1 || childIt == 2))
			return 1;

	return 0;
}

SyntaxTreeNode* add_child(SyntaxTreeNode* this, SyntaxTreeNode* youngest_child, int childIt){
	int indent = this->indent_depth + is_indent(this->sElemIt, childIt);
	SyntaxTreeNode* newborn
		= parse(sElems[this->sElemIt].children[childIt], indent);

	if(youngest_child == NULL){
		this->child = newborn;
	}
	else{
		youngest_child->brother = newborn;
	}
	return newborn;
}

void init_parse(void) {
	int i;
	token = scan();

	for(i = 0; i < NUMOFSYNTAX; i++){
		SyntaxElem hoge = {SELEMOP_TERMINATOR, 0, {}};
		sElems[i] = hoge;
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
	sElems[TNAME]				= sElem_TNAME;
	sElems[TPROGRAM]			= sElem_TPROGRAM;
	sElems[TVAR]				= sElem_TVAR;
	sElems[TARRAY]				= sElem_TARRAY;
	sElems[TOF]					= sElem_TOF;
	sElems[TBEGIN]				= sElem_TBEGIN;
	sElems[TEND]				= sElem_TEND;
	sElems[TIF]					= sElem_TIF;
	sElems[TTHEN]				= sElem_TTHEN;
	sElems[TELSE]				= sElem_TELSE;
	sElems[TPROCEDURE]			= sElem_TPROCEDURE;
	sElems[TRETURN]				= sElem_TRETURN;
	sElems[TCALL]				= sElem_TCALL;
	sElems[TWHILE]				= sElem_TWHILE;
	sElems[TDO]					= sElem_TDO;
	sElems[TNOT]				= sElem_TNOT;
	sElems[TOR]					= sElem_TOR;
	sElems[TDIV]				= sElem_TDIV;
	sElems[TAND]				= sElem_TAND;
	sElems[TCHAR]				= sElem_TCHAR;
	sElems[TINTEGER]			= sElem_TINTEGER;
	sElems[TBOOLEAN]			= sElem_TBOOLEAN;
	sElems[TREADLN]				= sElem_TREADLN;
	sElems[TWRITELN]			= sElem_TWRITELN;
	sElems[TTRUE]				= sElem_TTRUE;
	sElems[TFALSE]				= sElem_TFALSE;
	sElems[TNUMBER]				= sElem_TNUMBER;
	sElems[TSTRING]				= sElem_TSTRING;
	sElems[TPLUS]				= sElem_TPLUS;
	sElems[TMINUS]				= sElem_TMINUS;
	sElems[TSTAR]				= sElem_TSTAR;
	sElems[TEQUAL]				= sElem_TEQUAL;
	sElems[TNOTEQ]				= sElem_TNOTEQ;
	sElems[TLE]					= sElem_TLE;
	sElems[TLEEQ]				= sElem_TLEEQ;
	sElems[TGR]					= sElem_TGR;
	sElems[TGREQ]				= sElem_TGREQ;
	sElems[TLPAREN]				= sElem_TLPAREN;
	sElems[TRPAREN]				= sElem_TRPAREN;
	sElems[TLSQPAREN]			= sElem_TLSQPAREN;
	sElems[TRSQPAREN]			= sElem_TRSQPAREN;
	sElems[TASSIGN]				= sElem_TASSIGN;
	sElems[TDOT]				= sElem_TDOT;
	sElems[TCOMMA]				= sElem_TCOMMA;
	sElems[TCOLON]				= sElem_TCOLON;
	sElems[TSEMI]				= sElem_TSEMI;
	sElems[TREAD]				= sElem_TREAD;
	sElems[TWRITE]				= sElem_TWRITE;
	sElems[TBREAK]				= sElem_TBREAK;
	*/
	sElems[SPROGRAM]			= sElem_SPROGRAM;
	sElems[SBLOCK]				= sElem_SBLOCK;
	sElems[SBLOCK_1]			= sElem_SBLOCK_1;
	sElems[SBLOCK_1_1]			= sElem_SBLOCK_1_1;
	sElems[SVARDEC]				= sElem_SVARDEC;
	sElems[SVARDEC_6]			= sElem_SVARDEC_6;
	sElems[SVARDEC_6_1]			= sElem_SVARDEC_6_1;
	sElems[SVARNAMES]			= sElem_SVARNAMES;
	sElems[SVARNAMES_2]			= sElem_SVARNAMES_2;
	sElems[SVARNAMES_2_1]		= sElem_SVARNAMES_2_1;
	sElems[SVARNAME]			= sElem_SVARNAME;
	sElems[STYPE]				= sElem_STYPE;
	sElems[SSTDTYPE]			= sElem_SSTDTYPE;
	sElems[SARRTYPE]			= sElem_SARRTYPE;
	sElems[SSUBPROGDEC]			= sElem_SSUBPROGDEC;
	sElems[SSUBPROGDEC_3]		= sElem_SSUBPROGDEC_3;
	sElems[SSUBPROGDEC_5]		= sElem_SSUBPROGDEC_5;
	sElems[SPROCEDURENAME]		= sElem_SPROCEDURENAME;
	sElems[SFORMPARAM]			= sElem_SFORMPARAM;
	sElems[SFORMPARAM_5]		= sElem_SFORMPARAM_5;
	sElems[SFORMPARAM_5_1]		= sElem_SFORMPARAM_5_1;
	sElems[SCOMPSTAT]			= sElem_SCOMPSTAT;
	sElems[SCOMPSTAT_3]			= sElem_SCOMPSTAT_3;
	sElems[SCOMPSTAT_3_1]		= sElem_SCOMPSTAT_3_1;
	sElems[SSTAT]				= sElem_SSTAT;
	sElems[SCONDSTAT]			= sElem_SCONDSTAT;
	sElems[SCONDSTAT_5]			= sElem_SCONDSTAT_5;
	sElems[SCONDSTAT_5_1]		= sElem_SCONDSTAT_5_1;
	sElems[SITERSTAT]			= sElem_SITERSTAT;
	sElems[SEXITSTAT]			= sElem_SEXITSTAT;
	sElems[SCALLSTAT]			= sElem_SCALLSTAT;
	sElems[SCALLSTAT_3]			= sElem_SCALLSTAT_3;
	sElems[SCALLSTAT_3_1]		= sElem_SCALLSTAT_3_1;
	sElems[SEXPRS]				= sElem_SEXPRS;
	sElems[SEXPRS_2]			= sElem_SEXPRS_2;
	sElems[SEXPRS_2_1]			= sElem_SEXPRS_2_1;
	sElems[SRETSTAT]			= sElem_SRETSTAT;
	sElems[SASSIGNSTAT]			= sElem_SASSIGNSTAT;
	sElems[SLEFTPART]			= sElem_SLEFTPART;
	sElems[SVAR]				= sElem_SVAR;
	sElems[SVAR_2]				= sElem_SVAR_2;
	sElems[SVAR_2_1]			= sElem_SVAR_2_1;
	sElems[SEXPR]				= sElem_SEXPR;
	sElems[SEXPR_2]				= sElem_SEXPR_2;
	sElems[SEXPR_2_1]			= sElem_SEXPR_2_1;
	sElems[SSIMPLEEXPR]			= sElem_SSIMPLEEXPR;
	sElems[SSIMPLEEXPR_1]		= sElem_SSIMPLEEXPR_1;
	sElems[SSIMPLEEXPR_1_1]		= sElem_SSIMPLEEXPR_1_1;
	sElems[SSIMPLEEXPR_3]		= sElem_SSIMPLEEXPR_3;
	sElems[SSIMPLEEXPR_3_1]		= sElem_SSIMPLEEXPR_3_1;
	sElems[STERM]				= sElem_STERM;
	sElems[STERM_2]				= sElem_STERM_2;
	sElems[STERM_2_1]			= sElem_STERM_2_1;
	sElems[SFACTOR]				= sElem_SFACTOR;
	sElems[SFACTOR_3]			= sElem_SFACTOR_3;
	sElems[SFACTOR_4]			= sElem_SFACTOR_4;
	sElems[SFACTOR_5]			= sElem_SFACTOR_5;
	sElems[SCONST]				= sElem_SCONST;
	sElems[SMULOP]				= sElem_SMULOP;
	sElems[SADDOP]				= sElem_SADDOP;
	sElems[SRELATOP]			= sElem_SRELATOP;
	sElems[SINSTAT]				= sElem_SINSTAT;
	sElems[SINSTAT_1]			= sElem_SINSTAT_1;
	sElems[SINSTAT_2]			= sElem_SINSTAT_2;
	sElems[SINSTAT_2_1]			= sElem_SINSTAT_2_1;
	sElems[SINSTAT_2_1_3]		= sElem_SINSTAT_2_1_3;
	sElems[SINSTAT_2_1_3_1]		= sElem_SINSTAT_2_1_3_1;
	sElems[SOUTSTAT]			= sElem_SOUTSTAT;
	sElems[SOUTSTAT_1]			= sElem_SOUTSTAT_1;
	sElems[SOUTSTAT_2]			= sElem_SOUTSTAT_2;
	sElems[SOUTSTAT_2_1]		= sElem_SOUTSTAT_2_1;
	sElems[SOUTSTAT_2_1_3]		= sElem_SOUTSTAT_2_1_3;
	sElems[SOUTSTAT_2_1_3_1]	= sElem_SOUTSTAT_2_1_3_1;
	sElems[SOUTFORM]			= sElem_SOUTFORM;
	sElems[SOUTFORM_1]			= sElem_SOUTFORM_1;
	sElems[SOUTFORM_1_2]		= sElem_SOUTFORM_1_2;
	sElems[SOUTFORM_1_2_1]		= sElem_SOUTFORM_1_2_1;
	sElems[SEMPTYSTAT]			= sElem_SEMPTYSTAT;
}

SyntaxTreeNode* parse(int sElemIt, int indent_depth){
	int i;
	SyntaxElem sElem = sElems[sElemIt];
	SyntaxTreeNode* youngest_child = NULL;
	SyntaxTreeNode* this = malloc_tree_node();
	this->sElemIt = sElemIt;
	this->indent_depth = indent_depth;

	switch(sElem.op){
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
			int indent = this->indent_depth + is_indent(this->sElemIt, i);
			SyntaxTreeNode* newborn
				= parse(sElems[this->sElemIt].children[i], indent);

			if(youngest_child == NULL){
				this->child = newborn;
			}
			else{
				youngest_child->brother = newborn;
			}
			youngest_child = newborn;

			switch(youngest_child->parse_result){

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
			int indent = this->indent_depth + is_indent(this->sElemIt, i);
			SyntaxTreeNode* newborn
				= parse(sElems[this->sElemIt].children[i], indent);

			if(youngest_child == NULL){
				this->child = newborn;
			}
			else{
				youngest_child->brother = newborn;
			}
			youngest_child = newborn;

			switch(youngest_child->parse_result){

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
			int indent = this->indent_depth + is_indent(this->sElemIt, 0);
			SyntaxTreeNode* newborn
				= parse(sElems[this->sElemIt].children[0], indent);

			if(youngest_child == NULL){
				this->child = newborn;
			}
			else{
				youngest_child->brother = newborn;
			}
			youngest_child = newborn;

			if(youngest_child->parse_result != PARSERESULT_MATCH){
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
	return NULL;
}
