#include "pretty-printer.h"

/* const */ SyntaxElem s_elems[NUMOFSYNTAX + 1];

int token;

int is_indent(int parentSyntaxElemIt, int child_it){
	if(parentSyntaxElemIt == SBLOCK && child_it == 0)
		return 1;

	if(parentSyntaxElemIt == SSUBPROGDEC && child_it == 4)
		return 1;

	if(parentSyntaxElemIt == SCOMPSTAT && 
		(child_it == 1 || child_it == 2))
			return 1;

	return 0;
}

SyntaxTreeNode* add_child(SyntaxTreeNode* this, SyntaxTreeNode* youngest_child, int child_it){
	int indent = this->indent_depth + is_indent(this->s_elem_it, child_it);
	SyntaxTreeNode* newborn
		= parse(s_elems[this->s_elem_it].children[child_it], indent);

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
		s_elems[i] = hoge;
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
	SyntaxElem s_elem_SBLOCK			= {SELEMOP_ALL_OF,			2, {SBLOCK_1, SCOMPSTAT}};
	SyntaxElem s_elem_SBLOCK_1			= {SELEMOP_ZERO_OR_MORE,	1, {SBLOCK_1_1}};
	SyntaxElem s_elem_SBLOCK_1_1		= {SELEMOP_ONE_OF,			2, {SVARDEC, SSUBPROGDEC}};
	SyntaxElem s_elem_SVARDEC			= {SELEMOP_ALL_OF,			6, {TVAR, SVARNAMES, TCOLON, STYPE, TSEMI, SVARDEC_6}};
	SyntaxElem s_elem_SVARDEC_6			= {SELEMOP_ZERO_OR_MORE,	1, {SVARDEC_6_1}};
	SyntaxElem s_elem_SVARDEC_6_1		= {SELEMOP_ALL_OF,			4, {SVARNAMES, TCOLON, STYPE, TSEMI}};
	SyntaxElem s_elem_SVARNAMES			= {SELEMOP_ALL_OF,			2, {SVARNAME, SVARNAMES_2}};
	SyntaxElem s_elem_SVARNAMES_2		= {SELEMOP_ZERO_OR_MORE,	1, {SVARNAMES_2_1}};
	SyntaxElem s_elem_SVARNAMES_2_1		= {SELEMOP_ALL_OF,			2, {TCOMMA, SVARNAME}};
	SyntaxElem s_elem_SVARNAME			= {SELEMOP_ALL_OF,			1, {TNAME}};
	SyntaxElem s_elem_STYPE				= {SELEMOP_ONE_OF,			2, {SSTDTYPE, SARRTYPE}};
	SyntaxElem s_elem_SSTDTYPE			= {SELEMOP_ONE_OF,			3, {TINTEGER, TBOOLEAN, TCHAR}};
	SyntaxElem s_elem_SARRTYPE			= {SELEMOP_ALL_OF,			6, {TARRAY, TLSQPAREN, TNUMBER, TRSQPAREN, TOF, SSTDTYPE}};
	SyntaxElem s_elem_SSUBPROGDEC		= {SELEMOP_ALL_OF,			7, {TPROCEDURE, SPROCEDURENAME, SSUBPROGDEC_3, TSEMI, SSUBPROGDEC_5, SCOMPSTAT, TSEMI}};
	SyntaxElem s_elem_SSUBPROGDEC_3		= {SELEMOP_ZERO_OR_ONE,		1, {SFORMPARAM}};
	SyntaxElem s_elem_SSUBPROGDEC_5		= {SELEMOP_ZERO_OR_ONE,		1, {SVARDEC}};
	SyntaxElem s_elem_SPROCEDURENAME	= {SELEMOP_ALL_OF,			1, {TNAME}};
	SyntaxElem s_elem_SFORMPARAM		= {SELEMOP_ALL_OF,			6, {TLPAREN, SVARNAMES, TCOLON, STYPE, SFORMPARAM_5, TRPAREN}};
	SyntaxElem s_elem_SFORMPARAM_5		= {SELEMOP_ZERO_OR_MORE,	1, {SFORMPARAM_5_1}};
	SyntaxElem s_elem_SFORMPARAM_5_1	= {SELEMOP_ALL_OF,			4, {TSEMI, SVARNAMES, TCOLON, STYPE}};
	SyntaxElem s_elem_SCOMPSTAT			= {SELEMOP_ALL_OF,			4, {TBEGIN, SSTAT, SCOMPSTAT_3, TEND}};
	SyntaxElem s_elem_SCOMPSTAT_3		= {SELEMOP_ZERO_OR_MORE,	1, {SCOMPSTAT_3_1}};
	SyntaxElem s_elem_SCOMPSTAT_3_1		= {SELEMOP_ALL_OF,			2, {TSEMI, SSTAT}};
	SyntaxElem s_elem_SSTAT				= {SELEMOP_ONE_OF,			10, {SASSIGNSTAT, SCONDSTAT, SITERSTAT, SEXITSTAT, SCALLSTAT, SRETSTAT, SINSTAT, SOUTSTAT, SCOMPSTAT, SEMPTYSTAT}};
	SyntaxElem s_elem_SCONDSTAT			= {SELEMOP_ALL_OF,			5, {TIF, SEXPR, TTHEN, SSTAT, SCONDSTAT_5}};
	SyntaxElem s_elem_SCONDSTAT_5		= {SELEMOP_ZERO_OR_ONE,		1, {SCONDSTAT_5_1}};
	SyntaxElem s_elem_SCONDSTAT_5_1		= {SELEMOP_ALL_OF,			2, {TELSE, SSTAT}};
	SyntaxElem s_elem_SITERSTAT			= {SELEMOP_ALL_OF,			4, {TWHILE, SEXPR, TDO, SSTAT}};
	SyntaxElem s_elem_SEXITSTAT			= {SELEMOP_ALL_OF,			1, {TBREAK}};
	SyntaxElem s_elem_SCALLSTAT			= {SELEMOP_ALL_OF,			3, {TCALL, SPROCEDURENAME, SCALLSTAT_3}};
	SyntaxElem s_elem_SCALLSTAT_3		= {SELEMOP_ZERO_OR_ONE,		1, {SCALLSTAT_3_1}};
	SyntaxElem s_elem_SCALLSTAT_3_1		= {SELEMOP_ALL_OF,			3, {TLPAREN, SEXPRS, TRPAREN}};
	SyntaxElem s_elem_SEXPRS			= {SELEMOP_ALL_OF,			2, {SEXPR, SEXPRS_2}};
	SyntaxElem s_elem_SEXPRS_2			= {SELEMOP_ZERO_OR_MORE,	1, {SEXPRS_2_1}};
	SyntaxElem s_elem_SEXPRS_2_1		= {SELEMOP_ALL_OF,			2, {TCOMMA, SEXPR}};
	SyntaxElem s_elem_SRETSTAT			= {SELEMOP_ALL_OF,			1, {TRETURN}};
	SyntaxElem s_elem_SASSIGNSTAT		= {SELEMOP_ALL_OF,			3, {SLEFTPART, TASSIGN, SEXPR}};
	SyntaxElem s_elem_SLEFTPART			= {SELEMOP_ALL_OF,			1, {SVAR}};
	SyntaxElem s_elem_SVAR				= {SELEMOP_ALL_OF,			2, {SVARNAME, SVAR_2}};
	SyntaxElem s_elem_SVAR_2			= {SELEMOP_ZERO_OR_ONE,		1, {SVAR_2_1}};
	SyntaxElem s_elem_SVAR_2_1			= {SELEMOP_ALL_OF,			3, {TLSQPAREN, SEXPR, TRSQPAREN}};
	SyntaxElem s_elem_SEXPR				= {SELEMOP_ALL_OF,			2, {SSIMPLEEXPR, SEXPR_2}};
	SyntaxElem s_elem_SEXPR_2			= {SELEMOP_ZERO_OR_MORE,	1, {SEXPR_2_1}};
	SyntaxElem s_elem_SEXPR_2_1			= {SELEMOP_ALL_OF,			2, {SRELATOP, SSIMPLEEXPR}};
	SyntaxElem s_elem_SSIMPLEEXPR		= {SELEMOP_ALL_OF,			3, {SSIMPLEEXPR_1, STERM, SSIMPLEEXPR_3}};
	SyntaxElem s_elem_SSIMPLEEXPR_1		= {SELEMOP_ZERO_OR_ONE,		1, {SSIMPLEEXPR_1_1}};
	SyntaxElem s_elem_SSIMPLEEXPR_1_1	= {SELEMOP_ONE_OF,			2, {TPLUS, TMINUS}};
	SyntaxElem s_elem_SSIMPLEEXPR_3		= {SELEMOP_ZERO_OR_MORE,	1, {SSIMPLEEXPR_3_1}};
	SyntaxElem s_elem_SSIMPLEEXPR_3_1	= {SELEMOP_ALL_OF,			2, {SADDOP, STERM}};
	SyntaxElem s_elem_STERM				= {SELEMOP_ALL_OF,			2, {SFACTOR, STERM_2}};
	SyntaxElem s_elem_STERM_2			= {SELEMOP_ZERO_OR_MORE,	1, {STERM_2_1}};
	SyntaxElem s_elem_STERM_2_1			= {SELEMOP_ALL_OF,			2, {SMULOP, SFACTOR}};
	SyntaxElem s_elem_SFACTOR			= {SELEMOP_ONE_OF,			5, {SVAR, SCONST, SFACTOR_3, SFACTOR_4, SFACTOR_5}};
	SyntaxElem s_elem_SFACTOR_3			= {SELEMOP_ALL_OF,			3, {TLPAREN, SEXPR, TRPAREN}};
	SyntaxElem s_elem_SFACTOR_4			= {SELEMOP_ALL_OF,			2, {TNOT, SFACTOR}};
	SyntaxElem s_elem_SFACTOR_5			= {SELEMOP_ALL_OF,			4, {SSTDTYPE, TLPAREN, SEXPR, TRPAREN}};
	SyntaxElem s_elem_SCONST			= {SELEMOP_ONE_OF,			4, {TNUMBER, TFALSE, TTRUE, TSTRING}};
	SyntaxElem s_elem_SMULOP			= {SELEMOP_ONE_OF,			3, {TSTAR, TDIV, TAND}};
	SyntaxElem s_elem_SADDOP			= {SELEMOP_ONE_OF,			3, {TPLUS, TMINUS, TOR}};
	SyntaxElem s_elem_SRELATOP			= {SELEMOP_ONE_OF,			6, {TEQUAL, TNOTEQ, TLE, TLEEQ, TGR, TGREQ}};
	SyntaxElem s_elem_SINSTAT			= {SELEMOP_ALL_OF,			2, {SINSTAT_1, SINSTAT_2}};
	SyntaxElem s_elem_SINSTAT_1			= {SELEMOP_ONE_OF,			2, {TREAD, TREADLN}};
	SyntaxElem s_elem_SINSTAT_2			= {SELEMOP_ZERO_OR_ONE,		1, {SINSTAT_2_1}};
	SyntaxElem s_elem_SINSTAT_2_1		= {SELEMOP_ALL_OF,			4, {TLPAREN, SVAR, SINSTAT_2_1_3, TRPAREN}};
	SyntaxElem s_elem_SINSTAT_2_1_3		= {SELEMOP_ZERO_OR_MORE,	1, {SINSTAT_2_1_3_1}};
	SyntaxElem s_elem_SINSTAT_2_1_3_1	= {SELEMOP_ALL_OF,			2, {TCOMMA, SVAR}};
	SyntaxElem s_elem_SOUTSTAT			= {SELEMOP_ALL_OF,			2, {SOUTSTAT_1, SOUTSTAT_2}};
	SyntaxElem s_elem_SOUTSTAT_1		= {SELEMOP_ONE_OF,			2, {TWRITE, TWRITELN}};
	SyntaxElem s_elem_SOUTSTAT_2		= {SELEMOP_ZERO_OR_ONE,		1, {SOUTSTAT_2_1}};
	SyntaxElem s_elem_SOUTSTAT_2_1		= {SELEMOP_ALL_OF,			4, {TLPAREN, SOUTFORM, SOUTSTAT_2_1_3, TRPAREN}};
	SyntaxElem s_elem_SOUTSTAT_2_1_3	= {SELEMOP_ZERO_OR_MORE,	1, {SOUTSTAT_2_1_3_1}};
	SyntaxElem s_elem_SOUTSTAT_2_1_3_1	= {SELEMOP_ALL_OF,			2, {TCOMMA, SOUTFORM}};
	SyntaxElem s_elem_SOUTFORM			= {SELEMOP_ONE_OF,			2, {SOUTFORM_1, TSTRING}};
	SyntaxElem s_elem_SOUTFORM_1		= {SELEMOP_ALL_OF,			2, {SEXPR, SOUTFORM_1_2}};
	SyntaxElem s_elem_SOUTFORM_1_2		= {SELEMOP_ZERO_OR_ONE,		1, {SOUTFORM_1_2_1}};
	SyntaxElem s_elem_SOUTFORM_1_2_1	= {SELEMOP_ALL_OF,			2, {TCOLON, TNUMBER}};
	SyntaxElem s_elem_SEMPTYSTAT		= {SELEMOP_TERMINATOR,		0, {}};
	/*
	s_elems[TNAME]				= s_elem_TNAME;
	s_elems[TPROGRAM]			= s_elem_TPROGRAM;
	s_elems[TVAR]				= s_elem_TVAR;
	s_elems[TARRAY]				= s_elem_TARRAY;
	s_elems[TOF]				= s_elem_TOF;
	s_elems[TBEGIN]				= s_elem_TBEGIN;
	s_elems[TEND]				= s_elem_TEND;
	s_elems[TIF]				= s_elem_TIF;
	s_elems[TTHEN]				= s_elem_TTHEN;
	s_elems[TELSE]				= s_elem_TELSE;
	s_elems[TPROCEDURE]			= s_elem_TPROCEDURE;
	s_elems[TRETURN]			= s_elem_TRETURN;
	s_elems[TCALL]				= s_elem_TCALL;
	s_elems[TWHILE]				= s_elem_TWHILE;
	s_elems[TDO]				= s_elem_TDO;
	s_elems[TNOT]				= s_elem_TNOT;
	s_elems[TOR]				= s_elem_TOR;
	s_elems[TDIV]				= s_elem_TDIV;
	s_elems[TAND]				= s_elem_TAND;
	s_elems[TCHAR]				= s_elem_TCHAR;
	s_elems[TINTEGER]			= s_elem_TINTEGER;
	s_elems[TBOOLEAN]			= s_elem_TBOOLEAN;
	s_elems[TREADLN]			= s_elem_TREADLN;
	s_elems[TWRITELN]			= s_elem_TWRITELN;
	s_elems[TTRUE]				= s_elem_TTRUE;
	s_elems[TFALSE]				= s_elem_TFALSE;
	s_elems[TNUMBER]			= s_elem_TNUMBER;
	s_elems[TSTRING]			= s_elem_TSTRING;
	s_elems[TPLUS]				= s_elem_TPLUS;
	s_elems[TMINUS]				= s_elem_TMINUS;
	s_elems[TSTAR]				= s_elem_TSTAR;
	s_elems[TEQUAL]				= s_elem_TEQUAL;
	s_elems[TNOTEQ]				= s_elem_TNOTEQ;
	s_elems[TLE]				= s_elem_TLE;
	s_elems[TLEEQ]				= s_elem_TLEEQ;
	s_elems[TGR]				= s_elem_TGR;
	s_elems[TGREQ]				= s_elem_TGREQ;
	s_elems[TLPAREN]			= s_elem_TLPAREN;
	s_elems[TRPAREN]			= s_elem_TRPAREN;
	s_elems[TLSQPAREN]			= s_elem_TLSQPAREN;
	s_elems[TRSQPAREN]			= s_elem_TRSQPAREN;
	s_elems[TASSIGN]			= s_elem_TASSIGN;
	s_elems[TDOT]				= s_elem_TDOT;
	s_elems[TCOMMA]				= s_elem_TCOMMA;
	s_elems[TCOLON]				= s_elem_TCOLON;
	s_elems[TSEMI]				= s_elem_TSEMI;
	s_elems[TREAD]				= s_elem_TREAD;
	s_elems[TWRITE]				= s_elem_TWRITE;
	s_elems[TBREAK]				= s_elem_TBREAK;
	*/
	s_elems[SPROGRAM]			= s_elem_SPROGRAM;
	s_elems[SBLOCK]				= s_elem_SBLOCK;
	s_elems[SBLOCK_1]			= s_elem_SBLOCK_1;
	s_elems[SBLOCK_1_1]			= s_elem_SBLOCK_1_1;
	s_elems[SVARDEC]			= s_elem_SVARDEC;
	s_elems[SVARDEC_6]			= s_elem_SVARDEC_6;
	s_elems[SVARDEC_6_1]		= s_elem_SVARDEC_6_1;
	s_elems[SVARNAMES]			= s_elem_SVARNAMES;
	s_elems[SVARNAMES_2]		= s_elem_SVARNAMES_2;
	s_elems[SVARNAMES_2_1]		= s_elem_SVARNAMES_2_1;
	s_elems[SVARNAME]			= s_elem_SVARNAME;
	s_elems[STYPE]				= s_elem_STYPE;
	s_elems[SSTDTYPE]			= s_elem_SSTDTYPE;
	s_elems[SARRTYPE]			= s_elem_SARRTYPE;
	s_elems[SSUBPROGDEC]		= s_elem_SSUBPROGDEC;
	s_elems[SSUBPROGDEC_3]		= s_elem_SSUBPROGDEC_3;
	s_elems[SSUBPROGDEC_5]		= s_elem_SSUBPROGDEC_5;
	s_elems[SPROCEDURENAME]		= s_elem_SPROCEDURENAME;
	s_elems[SFORMPARAM]			= s_elem_SFORMPARAM;
	s_elems[SFORMPARAM_5]		= s_elem_SFORMPARAM_5;
	s_elems[SFORMPARAM_5_1]		= s_elem_SFORMPARAM_5_1;
	s_elems[SCOMPSTAT]			= s_elem_SCOMPSTAT;
	s_elems[SCOMPSTAT_3]		= s_elem_SCOMPSTAT_3;
	s_elems[SCOMPSTAT_3_1]		= s_elem_SCOMPSTAT_3_1;
	s_elems[SSTAT]				= s_elem_SSTAT;
	s_elems[SCONDSTAT]			= s_elem_SCONDSTAT;
	s_elems[SCONDSTAT_5]		= s_elem_SCONDSTAT_5;
	s_elems[SCONDSTAT_5_1]		= s_elem_SCONDSTAT_5_1;
	s_elems[SITERSTAT]			= s_elem_SITERSTAT;
	s_elems[SEXITSTAT]			= s_elem_SEXITSTAT;
	s_elems[SCALLSTAT]			= s_elem_SCALLSTAT;
	s_elems[SCALLSTAT_3]		= s_elem_SCALLSTAT_3;
	s_elems[SCALLSTAT_3_1]		= s_elem_SCALLSTAT_3_1;
	s_elems[SEXPRS]				= s_elem_SEXPRS;
	s_elems[SEXPRS_2]			= s_elem_SEXPRS_2;
	s_elems[SEXPRS_2_1]			= s_elem_SEXPRS_2_1;
	s_elems[SRETSTAT]			= s_elem_SRETSTAT;
	s_elems[SASSIGNSTAT]		= s_elem_SASSIGNSTAT;
	s_elems[SLEFTPART]			= s_elem_SLEFTPART;
	s_elems[SVAR]				= s_elem_SVAR;
	s_elems[SVAR_2]				= s_elem_SVAR_2;
	s_elems[SVAR_2_1]			= s_elem_SVAR_2_1;
	s_elems[SEXPR]				= s_elem_SEXPR;
	s_elems[SEXPR_2]			= s_elem_SEXPR_2;
	s_elems[SEXPR_2_1]			= s_elem_SEXPR_2_1;
	s_elems[SSIMPLEEXPR]		= s_elem_SSIMPLEEXPR;
	s_elems[SSIMPLEEXPR_1]		= s_elem_SSIMPLEEXPR_1;
	s_elems[SSIMPLEEXPR_1_1]	= s_elem_SSIMPLEEXPR_1_1;
	s_elems[SSIMPLEEXPR_3]		= s_elem_SSIMPLEEXPR_3;
	s_elems[SSIMPLEEXPR_3_1]	= s_elem_SSIMPLEEXPR_3_1;
	s_elems[STERM]				= s_elem_STERM;
	s_elems[STERM_2]			= s_elem_STERM_2;
	s_elems[STERM_2_1]			= s_elem_STERM_2_1;
	s_elems[SFACTOR]			= s_elem_SFACTOR;
	s_elems[SFACTOR_3]			= s_elem_SFACTOR_3;
	s_elems[SFACTOR_4]			= s_elem_SFACTOR_4;
	s_elems[SFACTOR_5]			= s_elem_SFACTOR_5;
	s_elems[SCONST]				= s_elem_SCONST;
	s_elems[SMULOP]				= s_elem_SMULOP;
	s_elems[SADDOP]				= s_elem_SADDOP;
	s_elems[SRELATOP]			= s_elem_SRELATOP;
	s_elems[SINSTAT]			= s_elem_SINSTAT;
	s_elems[SINSTAT_1]			= s_elem_SINSTAT_1;
	s_elems[SINSTAT_2]			= s_elem_SINSTAT_2;
	s_elems[SINSTAT_2_1]		= s_elem_SINSTAT_2_1;
	s_elems[SINSTAT_2_1_3]		= s_elem_SINSTAT_2_1_3;
	s_elems[SINSTAT_2_1_3_1]	= s_elem_SINSTAT_2_1_3_1;
	s_elems[SOUTSTAT]			= s_elem_SOUTSTAT;
	s_elems[SOUTSTAT_1]			= s_elem_SOUTSTAT_1;
	s_elems[SOUTSTAT_2]			= s_elem_SOUTSTAT_2;
	s_elems[SOUTSTAT_2_1]		= s_elem_SOUTSTAT_2_1;
	s_elems[SOUTSTAT_2_1_3]		= s_elem_SOUTSTAT_2_1_3;
	s_elems[SOUTSTAT_2_1_3_1]	= s_elem_SOUTSTAT_2_1_3_1;
	s_elems[SOUTFORM]			= s_elem_SOUTFORM;
	s_elems[SOUTFORM_1]			= s_elem_SOUTFORM_1;
	s_elems[SOUTFORM_1_2]		= s_elem_SOUTFORM_1_2;
	s_elems[SOUTFORM_1_2_1]		= s_elem_SOUTFORM_1_2_1;
	s_elems[SEMPTYSTAT]			= s_elem_SEMPTYSTAT;
}

SyntaxTreeNode* parse(int s_elem_it, int indent_depth){
	int i;
	SyntaxElem sElem = s_elems[s_elem_it];
	SyntaxTreeNode* youngest_child = NULL;
	SyntaxTreeNode* this = malloc_tree_node();
	this->s_elem_it = s_elem_it;
	this->indent_depth = indent_depth;

	switch(sElem.op){
	/* check 1. to meet TERMINATOR */
	case SELEMOP_TERMINATOR:
		/* empry stat	: return empty */
		if(s_elem_it == SEMPTYSTAT){
			this->parse_result = PARSERESULT_EMPTY;
			return this;
		}
		/* other		: compare token and elem */
		else if(token == s_elem_it){
			this->parse_result = PARSERESULT_MATCH;
			this->s_elem_it = s_elem_it;
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
		for(i = 0; i < sElem.children_num; i++){
			youngest_child = add_child(this, youngest_child, i);

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
		for(i = 0; i < sElem.children_num; i++){
			youngest_child = add_child(this, youngest_child, i);

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
			youngest_child = add_child(this, youngest_child, 0);

			if(youngest_child->parse_result != PARSERESULT_MATCH){
				break;
			}
			this->parse_result = PARSERESULT_MATCH;
		}

		return this;

	/* check 5. to meet the condition ZERO OR ONE time */
	case SELEMOP_ZERO_OR_ONE:
		youngest_child = add_child(this, youngest_child, 0);

		if(youngest_child->parse_result == PARSERESULT_MATCH)
			this->parse_result = PARSERESULT_MATCH;
		else
			this->parse_result = PARSERESULT_EMPTY;

		return this;
	}

	/* nobody can come here just for debug */
	return NULL;
}
