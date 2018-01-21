#include "compiler.h"


char* get_label(SyntaxTreeNode* node){
	char* hoge;
	if(node->s_elem_it == SVAR){
		return get_label(node->child);
	}
	if(node->s_elem_it == SVARNAME){
		hoge = calloc(sizeof(char), MAXSTRSIZE * 2 + 10);
		VarData* var_data = (VarData*)node->data;
		VarDecData* var_dec_data = (VarDecData*)var_data->data;
		if(var_data->is_declaration == 0){
			VarRefData* var_ref_data = (VarRefData*)var_data->data;
			VarData* var_data = (VarData*)var_ref_data->data;
			var_dec_data = (VarDecData*)var_data->data;
		}
		if(var_dec_data->namespace->name == NULL){
			sprintf(hoge, "$%s%%%%global%%",
					var_dec_data->name);
		}
		else{
			sprintf(hoge, "$%s%%%s",
					var_dec_data->name, var_dec_data->namespace->name);
		}
	}
	else if(node->s_elem_it == SSUBPROGDEC){
		hoge = calloc(sizeof(char), MAXSTRSIZE + 10);
		sprintf(hoge, "%%%s", ((ProcData*)node->data)->name);
	}
	else{
		hoge = calloc(sizeof(char), MAXSTRSIZE + 10);
		sprintf(hoge, "$%s%p", SYNTAXDIC[node->s_elem_it], node);
	}
	return hoge;
}

void generate_assm(SyntaxTreeNode* node){

	if(node == NULL) return;

	if(node->parse_result != PARSERESULT_MATCH){
		generate_assm(node->child);
		generate_assm(node->brother);
		return;
	}

	/*printf("\t\tgen assm %s\n", SYNTAXDIC[node->s_elem_it]);*/

	switch(node->s_elem_it){
	case SPROGRAM:{
		/* SPROGRAM
		 *  L>TPROGRAM->TNAME->TSEMI->SBLOCK->TDOT
		 */
		SyntaxTreeNode* node_SBLOCK = node->child->brother->brother->brother;

		iw_comment	("start SPROGRAM");
		iw_START	(get_label(node));
		iw_LAD		("", "gr0", "ZERO");
		iw_CALL		("", get_label(node_SBLOCK));
		iw_CALL		("", "FLUSH");
		iw_SVC		("", "0");
		iw_comment	("end   SPROGRAM");

		generate_assm(node_SBLOCK);
		break;
	}

	case SVARNAME:{
		VarData* var_data = (VarData*)node->data;
		if(var_data->is_declaration){
			VarDecData* var_dec_data = (VarDecData*)var_data->data;
				/*
				char name[MAXSTRSIZE];
				Type type;
				int is_param;
				int line;
				struct ProcedureData_* namespace;
				struct VarData_* ref_head;
				struct VarData_* ref_tail;
				*/
			int size = var_dec_data->type.array_size;
			if(size == 0) size = 1;

			iw_DS(get_label(node), size);
		}
		else{
			/*VarRefData var_ref_data = (VarRefData*)var_data->data;*/

		}
		break;
	}

	case SSUBPROGDEC:{
		/* SSUBPROGDEC
		 *  L>TPROCEDURE->SPROCEDURENAME->SSUBPROGDEC_2->TSEMI->SSUBPROGDEC_4->SCOMPSTAT->TSEMI
		 *                                 L> SFORMPARAM         L> SVARDEC
		 */

		SyntaxTreeNode* node_SSUBPROGDEC_2 = node->child->brother->brother;
		SyntaxTreeNode* node_SSUBPROGDEC_4 = node_SSUBPROGDEC_2->brother->brother;
		SyntaxTreeNode* node_SCOMPSTAT = node_SSUBPROGDEC_4->brother;
		iw_comment	("start SSUBPROGDEC");
		generate_assm(node_SSUBPROGDEC_2->child);
		generate_assm(node_SSUBPROGDEC_4->child);
		iw_label(get_label(node));
		generate_assm(node_SCOMPSTAT->child);
		iw_RET("");
		iw_comment	("end   SSUBPROGDEC");
		break;
	}

	case SCONDSTAT:{
		/* SCONDSTAT
		 *  L> TIF -> (*)SEXPR -> TTHEN -> SSTAT -> SCONDSTAT_4
		 *	                                         L> SCONDSTAT_4_0
		 *                                               L> TELSE -> SSTAT
		 */
		break;
	}

	case SITERSTAT:{
		/* SITERSTAT
		 *  L> TWHILE -> (*)SEXPR -> TDO -> SSTAT
		 */
		break;
	}

	case SEXITSTAT:{
		break;
	}

	case SCALLSTAT:{
		/* SCALLSTAT
		 *  L> TCALL -> SPROCNAME -> SCALLSTAT_2
		 *                             L> SCALLSTAT_2_0
		 *                                 L> TLPAREN -> SEXPRS -> TRPAREN
		 *                                                L> SEXPR -> SEXPRS_1
		 *                                                             L> SEXPRS_1_0
		 *                                                                 L> TCOMMA -> SEXPR
		 */
		
		/* just 0 argument */
		/*if(node_SCALLSTAT_2->parse_result == PARSERESULT_EMPTY){

		}*/

		/* 1 or more arguments */
		/*else*/{

		}

		break;

	}

	case SEXPRS:{
		break;
	}

	case SRETSTAT:{
		iw_RET("");
		break;
	}

	case SASSIGNSTAT:{
		break;
	}

	case SVAR:{
		/* pattern 1 hoge[piyo] */
		/* SVAR
		 *  L> SVARNAME -> SVAR_1
		 *                  L> SVAR_1_0
		 *                      L> TLSQPAREN -> (*)SEXPR -> TRSQPAREN
		 */

		/* pattern 2 hoge */
		/* SVAR
		 *  L> SVARNAME -> SVAR_1
		 *                  L> SVAR_1_0(EMPTY)
		 */

		SyntaxTreeNode* node_SVARNAME = node->child;
		SyntaxTreeNode* node_SVAR_1_0 = node->child->brother->child;

		/* pattern 1 */
		if(node_SVAR_1_0->parse_result == PARSERESULT_MATCH){
			SyntaxTreeNode* node_SEXPR = node_SVAR_1_0->child->brother;
			generate_assm	(node_SEXPR);		/* gr1 = ans */
			iw_LAD			("", "gr2", "gr1");	/* gr2 = gr1 */
			iw_LAD_3		("", "gr1", get_label(node_SVARNAME), "gr2");
		}
		/* pattern 2 */
		else/* if(node_SVAR_1_0->parse_result == PARSERESULT_EMPTY) */{
			iw_LAD			("", "gr1", get_label(node_SVARNAME));
		}
		break;
	}


	case SEXPR:{
		/* SEXPR
		 *  L> SSIMPLEEXPR -> SEXPR_1
		 *                     L> SEXPR_1_0
		 *                         L> SRELATOP -> SSIMPLEEXPR
		 *                             L>TEQUAL->TNOTEQ->TLE->TLEEQ->TGR->TGREQ
		 */

		SyntaxTreeNode* node_SSIMPLEEXPR = node->child;
		SyntaxTreeNode* node_SEXPR_1_0 = node->child->brother->child;

		generate_assm(node_SSIMPLEEXPR); /* gr1 = ans */

		while(node_SEXPR_1_0->parse_result != PARSERESULT_DIFFERENCE){
			SyntaxTreeNode* node_rel_op = node_SEXPR_1_0->child->child;
			node_SSIMPLEEXPR = node_SEXPR_1_0->child->brother;

			while(node_rel_op->parse_result != PARSERESULT_MATCH){
				node_rel_op = node_rel_op->brother;
			}

			iw_PUSH			("", "0", "gr1");	/* stack.push(gr1) */
			generate_assm	(node_SSIMPLEEXPR);	/* gr1 = ans */
			iw_LAD			("", "gr2", "gr1");	/* gr2 = gr1 */
			iw_POP			("", "gr1");		/* gr1 = stack.pop() */
			/* gr1 = calc(gr1, gr2) */
			switch(node_rel_op->s_elem_it){
			case TEQUAL:	break;
			case TNOTEQ:	break;
			case TLE:		break;
			case TLEEQ:		break;
			case TGR:		break;
			case TGREQ:		break;
			}

			node_SEXPR_1_0 = node_SEXPR_1_0->brother;
		}

		/* now gr1 is answer */
		break;
	}

	case SSIMPLEEXPR:{
		/* SSIMPLEEXPR
		 *  L> SSIMPLEEXPR_0 -> STERM -> SSIMPLEEXPR_2
		 *      L> SSIMPLEEXPR_0_0        |
		 *          L> TPLUS -> TMINUS    |
		 *                                L> SSIMPLEEXPR_2_0
		 *                                    L> SADDOP -> STERM
		 *                                        L> TPLUS->TMINUS->TOR
		 */

		SyntaxTreeNode* node_TPLUS = node->child->child->child;
		SyntaxTreeNode* node_STERM = node->child->brother;
		SyntaxTreeNode* node_SSIMPLEEXPR_2_0 = node_STERM->brother->child;

		generate_assm(node_STERM); /* gr1 = ans */

		if(node_TPLUS->parse_result != PARSERESULT_MATCH){
			SyntaxTreeNode* node_TMINUS = node_TPLUS->brother;
			if(node_TMINUS != NULL &&
					node_TMINUS->parse_result == PARSERESULT_MATCH){
				/* gr1 *= -1 */
				iw_LAD		("", "gr2", "gr1");	/* gr2 = gr1 */
				iw_SUBA		("", "gr1", "gr2");	/* gr1 -= gr2 (gr1 = 0) */
				iw_SUBA		("", "gr1", "gr2");	/* gr1 -= gr2 */
			}
		}

		while(node_SSIMPLEEXPR_2_0->parse_result != PARSERESULT_DIFFERENCE){
			SyntaxTreeNode* node_add_op = node_SSIMPLEEXPR_2_0->child->child;
			node_STERM = node_SSIMPLEEXPR_2_0->child->brother;

			while(node_add_op->parse_result != PARSERESULT_MATCH){
				node_add_op = node_add_op->brother;
			}

			iw_PUSH			("", "0", "gr1");	/* stack.push(gr1) */
			generate_assm	(node_STERM);		/* gr1 = ans */
			iw_LAD			("", "gr2", "gr1");	/* gr2 = gr1 */
			iw_POP			("", "gr1");		/* gr1 = stack.pop() */
			/* gr1 = calc(gr1, gr2) */
			switch(node_add_op->s_elem_it){
			case TPLUS:		break;
			case TMINUS:	break;
			case TOR:		break;
			}

			node_SSIMPLEEXPR_2_0 = node_SSIMPLEEXPR_2_0->brother;
		}

		/* now gr1 is answer */
		break;
	}

	case STERM:{
		/* STERM
		 *  L> SFACTOR -> STERM_1
		 *                 L> STERM_1_0
		 *                     L> SMULOP -> SFACTOR
		 *                         L> TSTAR->TDIV->TAND
		 */
		SyntaxTreeNode* node_SFACTOR = node->child;
		SyntaxTreeNode* node_STERM_1_0 = node_SFACTOR->brother->child;

		generate_assm(node_SFACTOR); /* gr1 = ans */

		while(node_STERM_1_0->parse_result != PARSERESULT_DIFFERENCE){
			SyntaxTreeNode* node_add_op = node_STERM_1_0->child->child;
			node_SFACTOR = node_STERM_1_0->child->brother;

			while(node_add_op->parse_result != PARSERESULT_MATCH){
				node_add_op = node_add_op->brother;
			}

			iw_PUSH			("", "0", "gr1");	/* stack.push(gr1) */
			generate_assm	(node_SFACTOR);		/* gr1 = ans */
			iw_LAD			("", "gr2", "gr1");	/* gr2 = gr1 */
			iw_POP			("", "gr1");		/* gr1 = stack.pop() */
			/* gr1 = calc(gr1, gr2) */
			switch(node_add_op->s_elem_it){
			case TSTAR:		break;
			case TDIV:		break;
			case TAND:		break;
			}

			node_STERM_1_0 = node_STERM_1_0->brother;
		}

		/* now gr1 is answer */
		break;
	}

	case SFACTOR:{
		/* SFACTOR
		 *  L> SVAR -> SCONST -> SFACTOR_2 -> SFACTOR_3 -> SFACTOR_4
		 */
		SyntaxTreeNode* child = node->child;
		while(child->parse_result != PARSERESULT_MATCH){
			child = child->brother;
		}

		generate_assm(child);
		break;
	}

	case SFACTOR_2:{
		/* SFACTOR_2
		 *  L> TLPAREN -> SEXPR -> TRPAREN
		 */
		generate_assm(node->child->brother);
		break;
	}

	case SFACTOR_3:{
		/* SFACTOR_3
		 *  L> TNOT -> SFACTOR
		 */
		/* not A <=> A xor FALSE */
		generate_assm	(node->child->brother);	/* gr1 = ans */
		iw_LAD			("", "gr2", "FALSE");	/* gr2 = FALSE */
		iw_XOR			("", "gr1", "gr2");		/* gr1 = gr1 xor gr2 */
		break;
	}

	case SFACTOR_4:{
		/* SFACTOR_4
		 *  L> SSTDTYPE -> TLPAREN -> SEXPR -> TRPAREN
		 */
		generate_assm	(node->child->brother->brother);	/* gr1 = ans */
		break;
	}

	case SCONST:{
		ConstData* data = (ConstData*)node->data;
		iw_DC(get_label(node), data->val);
		iw_LD("", "gr1", get_label(node));
		break;
	}

	case SINSTAT:{
		/* SINSTAT
		 *  L>SINSTAT_0--------->SINSTAT_1
		 *     L>TREAD->TREADLN   L>SINSTAT_1_0
		 */
		SyntaxTreeNode* node_SINSTAT_1 = node->child->brother;
		SyntaxTreeNode* node_TREAD = node->child->child;
		iw_comment("start SINSTAT");
		generate_assm(node_SINSTAT_1);
		if(node_TREAD->parse_result != PARSERESULT_MATCH){
			/* no read yes readln */
			iw_CALL	("", "READSKIPTONEWLINE");
		}
		iw_comment("end   SINSTAT");
		break;
	}

	case SINSTAT_1_0:{
		/* SINSTAT_1_0
		 *  L> TLPAREN -> (*)SVAR -> SINSTAT_1_0_2 -> TRPAREN
		 *                            L> SINSTAT_1_0_2_0
		 *                                L> TCOMMA -> (*)SVAR
		 */

		SyntaxTreeNode* node_SVAR = node->child->brother;
		Type* type_node_SVAR = (Type*)node_SVAR->data;
		SyntaxTreeNode* node_SINSTAT_1_0_2_0 = node_SVAR->brother->child;

		iw_LD	("", "gr1", get_label(node_SVAR));
		if(type_node_SVAR->stdtype == TINTEGER){
			iw_CALL("", "READINT");
		}
		else{
			iw_CALL("", "READCHAR");
		}

		generate_assm(node_SINSTAT_1_0_2_0);
		break;
	}

	case SINSTAT_1_0_2_0:{
		/* SINSTAT_1_0_2_0
		 *  L> TCOMMA -> (*)SVAR
		 */

		SyntaxTreeNode* node_SVAR = node->child->brother;
		Type* type_node_SVAR = (Type*)node_SVAR->data;
		iw_LD	("", "gr1", get_label(node_SVAR));
		if(type_node_SVAR->stdtype == TINTEGER){
			iw_CALL("", "READINT");
		}
		else{
			iw_CALL("", "READCHAR");
		}
		generate_assm(node->brother);
		break;
	}

	case SOUTSTAT:{
		/* SOUTSTAT
		 *  L>SOUTSTAT_0->SOUTSTAT_1
		 *     L>TWRITE->TWRITELN   
		 */
		SyntaxTreeNode* node_SOUTSTAT_1 = node->child->brother;
		SyntaxTreeNode* node_TWRITE = node->child->child;

		iw_comment	("start SOUTSTAT");
		generate_assm(node_SOUTSTAT_1);
		if(node_TWRITE->parse_result != PARSERESULT_MATCH){
			/* no write yes writeln */
			iw_CALL	("", "WRITENEWLINE");
		}
		iw_comment	("end   SOUTSTAT");
		break;
	}

	case SOUTFORM:{
		/* SOUTFORM
		 *  L> SOUTFORM_0 -> TSTRING
		 *      L> (*)SEXPR -> SOUTFORM_0_1
		 *                      L> SOUTFORM_0_1_0
		 *                          L> TCOLON -> TNUMBER
		 */		
		if(node->child->parse_result == PARSERESULT_MATCH){
			/* SOUTFORM_0 is matched */
			SyntaxTreeNode* node_SEXPR = node->child->child;
			SyntaxTreeNode* node_SOUTFORM_0_1 = node_SEXPR->brother;
			Type* type_node_SEXPR = (Type*)node_SEXPR->data;
			generate_assm(node_SEXPR);	/* gr1 <-  */
			if(node_SOUTFORM_0_1->parse_result == PARSERESULT_MATCH){
				SyntaxTreeNode* node_TNUMBER = node_SOUTFORM_0_1->child->child->brother;
				iw_LAD	("", "gr2", node_TNUMBER->string_attr);
			}
			else{
				iw_LD	("", "gr2", "gr0");
			}
			switch(type_node_SEXPR->stdtype){
			case TINTEGER:
				iw_CALL	("", "WRITEINT");
				break;
			case TSTRING:
				iw_CALL	("", "WRITECHAR");
				break;
			case TBOOLEAN:
				iw_CALL	("", "WRITEBOOL");
				break;
			}
		}
		else{
			/* TSTRING is matched */
			SyntaxTreeNode* node_TSTRING = node->child->brother;
			iw_DC_str	(get_label(node_TSTRING), node_TSTRING->string_attr);
			iw_LAD		("", "gr1", get_label(node_TSTRING));
			iw_LD		("", "gr2", "gr0");
			iw_CALL		("", "WRITESTR");
		}
		generate_assm(node->brother);
		break;
	}

	default:{
		generate_assm(node->child);
		generate_assm(node->brother);
		break;
	}
	}

	return;

}

int main(int nc, char *np[]){
	SyntaxTreeNode *node_SPROGRAM;
	/*CrossRefRecord *record_head;*/

	if(nc < 2) {
		printf("File name id not given.\n");
		exit(-1);
	}

	if(init_scan(np[1]) < 0) {
		printf("File %s can not open.\n", np[1]);
		exit(-1);
	}

	init_parse();

	node_SPROGRAM = parse(SPROGRAM, 0);

	if(node_SPROGRAM->parse_result != PARSERESULT_MATCH){
		printf("syntax error found.\ndetail:\n");
		find_error_tree(node_SPROGRAM);
		
		free_tree(node_SPROGRAM);
		end_scan();
		return -1;
	}

	/*decide_printed_line_num_tree(node_SPROGRAM);*/

	fill_node_data_prepare(node_SPROGRAM);

	if(!fill_node_data(node_SPROGRAM, NULL, NULL)){
		/*debug_tree(node_SPROGRAM);
		debug_variable(node_SPROGRAM);*/
		free_tree(node_SPROGRAM);
		end_scan();
		return -1;
	}

	/*debug_tree(node_SPROGRAM);
	debug_variable(node_SPROGRAM);*/

	if(!check_type(node_SPROGRAM)){
		printf("error found.\n");
		free_tree(node_SPROGRAM);
		end_scan();
		return -1;
	}

	/*debug_tree(node_SPROGRAM);*/

	fp = stdout;

	generate_assm(node_SPROGRAM);

	if(0){
		fprintf(fp, "\n\
EOVF\n\
	CALL WRITELINE\n\
	LAD gr1, EOVF1\n\
	LD gr2, gr0\n\
	CALL WRITESTR\n\
	CALL WRITELINE\n\
	SVC 1 ; overflow error stop\n\
EOVF1 DC '***** Run-Time Error : Overflow *****'\n\
E0DIV\n\
	JNZ EOVF\n\
	CALL WRITELINE\n\
	LAD gr1, E0DIV1\n\
	LD gr2, gr0\n\
	CALL WRITESTR\n\
	CALL WRITELINE\n\
	SVC 2 ; 0-divide error stop\n\
E0DIV1 DC '***** Run-Time Error : Zero-Divide *****'\n\
EROV\n\
	CALL WRITELINE\n\
	LAD gr1, EROV1\n\
	LD gr2, gr0\n\
	CALL WRITESTR\n\
	CALL WRITELINE\n\
	SVC 3 ; range-over error stop\n\
EROV1 DC '***** Run-Time Error : Range-Over in ArrayIndex *****'\n\
WRITECHAR\n\
; gr1 の値（文字）を gr2 のけた数で出力する．\n\
; gr2 が 0 なら必要最小限の桁数で出力する\n\
	RPUSH\n\
	LD gr6, SPACE\n\
	LD gr7, OBUFSIZE\n\
WC1\n\
	SUBA gr2, ONE ; while(--c > 0) {\n\
	JZE WC2\n\
	JMI WC2\n\
	ST gr6, OBUF,gr7 ; *p++ = ' ';\n\
	CALL BOVFCHECK\n\
	JUMP WC1 ; }\n\
WC2\n\
	ST gr1, OBUF,gr7 ; *p++ = gr1;\n\
	CALL BOVFCHECK\n\
	ST gr7, OBUFSIZE\n\
	RPOP\n\
	RET\n\
WRITESTR\n\
; gr1 が指す文字列を gr2 のけた数で出力する．\n\
; gr2 が 0 なら必要最小限の桁数で出力する\n\
	RPUSH\n\
	LD gr6, gr1 ; p = gr1;\n\
WS1\n\
	LD gr4, 0,gr6 ; while(*p != '¥0') {\n\
	JZE WS2\n\
	ADDA gr6, ONE ; p++;\n\
	SUBA gr2, ONE ; c- ;\n\
	JUMP WS1 ; }\n\
WS2\n\
	LD gr7, OBUFSIZE ; q = OBUFSIZE;\n\
	LD gr5, SPACE\n\
WS3\n\
	SUBA gr2, ONE ; while(--c >= 0) {\n\
	JMI WS4\n\
	ST gr5, OBUF,gr7 ; *q++ = ' ';\n\
	CALL BOVFCHECK\n\
	JUMP WS3 ; }\n\
WS4\n\
	LD gr4, 0,gr1 ; while(*gr1 != '¥0') {\n\
	JZE WS5\n\
	ST gr4, OBUF,gr7 ; *q++ = *gr1++;\n\
	ADDA gr1, ONE\n\
	CALL BOVFCHECK\n\
	JUMP WS4 ; }\n\
WS5\n\
	ST gr7, OBUFSIZE ; OBUFSIZE = q;\n\
	RPOP\n\
	RET\n\
BOVFCHECK\n\
	ADDA gr7, ONE\n\
	CPA gr7, BOVFLEVEL\n\
	JMI BOVF1\n\
	CALL WRITELINE\n\
	LD gr7, OBUFSIZE\n\
BOVF1\n\
	RET\n\
BOVFLEVEL DC 256\n\
WRITEINT\n\
; gr1 の値（整数）を gr2 のけた数で出力する．\n\
; gr2 が 0 なら必要最小限の桁数で出力する\n\
	RPUSH\n\
	LD gr7, gr0 ; flag = 0;\n\
	CPA gr1, gr0 ; if(gr1>=0) goto WI1;\n\
	JPL WI1\n\
	JZE WI1\n\
	LD gr4, gr0 ; gr1= - gr1;\n\
	SUBA gr4, gr1\n\
	CPA gr4, gr1\n\
	JZE WI6\n\
	LD gr1, gr4\n\
	LD gr7, ONE ; flag = 1;\n\
WI1\n\
	LD gr6, SIX ; p = INTBUF+6;\n\
	ST gr0, INTBUF,gr6 ; *p = '¥0';\n\
	SUBA gr6, ONE ; p- ;\n\
	CPA gr1, gr0 ; if(gr1 == 0)\n\
	JNZ WI2\n\
	LD gr4, ZERO ; *p = '0';\n\
	ST gr4, INTBUF,gr6\n\
	JUMP WI5 ; }\n\
WI2 ; else {\n\
	CPA gr1, gr0 ; while(gr1 != 0) {\n\
	JZE WI3\n\
	LD gr5, gr1 ; gr5 = gr1 - (gr1 / 10) * 10;\n\
	DIVA gr1, TEN ; gr1 /= 10;\n\
	LD gr4, gr1\n\
	MULA gr4, TEN\n\
	SUBA gr5, gr4\n\
	ADDA gr5, ZERO ; gr5 += '0';\n\
	ST gr5, INTBUF,gr6 ; *p = gr5;\n\
	SUBA gr6, ONE ; p- ;\n\
	JUMP WI2 ; }\n\
WI3\n\
	CPA gr7, gr0 ; if(flag != 0) {\n\
	JZE WI4\n\
	LD gr4, MINUS ; *p = '-';\n\
	ST gr4, INTBUF,gr6\n\
	JUMP WI5 ; }\n\
WI4\n\
	ADDA gr6, ONE ; else p++;\n\
	; }\n\
WI5\n\
	LAD gr1, INTBUF,gr6 ; gr1 = p;\n\
	CALL WRITESTR ; WRITESTR();\n\
	RPOP\n\
	RET\n\
WI6\n\
	LAD gr1, MMINT\n\
	CALL WRITESTR ; WRITESTR();\n\
	RPOP\n\
	RET\n\
MMINT DC '-32768'\n\
WRITEBOOL\n\
; gr1 の値（真理値）が 0 なら'FALSE'を\n\
; 0 以外なら'TRUE'を gr2 のけた数で出力する．\n\
; gr2 が 0 なら必要最小限の桁数で出力する\n\
	RPUSH\n\
	CPA gr1, gr0 ; if(gr1 != 0)\n\
	JZE WB1\n\
	LAD gr1, WBTRUE ; gr1 = \"TRUE\";\n\
	JUMP WB2\n\
WB1 ; else\n\
	LAD gr1, WBFALSE ; gr1 = \"FALSE\";\n\
WB2\n\
	CALL WRITESTR ; WRITESTR();\n\
	RPOP\n\
	RET\n\
WBTRUE DC 'TRUE'\n\
WBFALSE DC 'FALSE'\n\
WRITENEWLINE\n\
; 改行を出力する\n\
	RPUSH\n\
	LD gr7, OBUFSIZE\n\
	LD gr6, NEWLINE\n\
	ST gr6, OBUF,gr7\n\
	ADDA gr7, ONE\n\
	ST gr7, OBUFSIZE\n\
	OUT OBUF, OBUFSIZE\n\
	ST gr0, OBUFSIZE\n\
	RPOP\n\
	RET\n\
FLUSH\n\
	RPUSH\n\
	LD gr7, OBUFSIZE\n\
	JZE FL1\n\
	CALL WRITELINE\n\
FL1\n\
	RPOP\n\
	RET\n\
READCHAR\n\
; gr1 が指す番地に文字一つを読み込む\n\
	RPUSH\n\
	LD gr5, RPBBUF ; if(RPBBUF != '¥0') {\n\
	JZE RC0\n\
	ST gr5, 0,gr1 ; *gr1 = RPBBUF;\n\
	ST gr0, RPBBUF ; RPBBUF = '¥0'\n\
	JUMP RC3 ; return; }\n\
RC0\n\
	LD gr7, INP ; inp = INP;\n\
	LD gr6, IBUFSIZE ; if(IBUFSIZE == 0) {\n\
	JNZ RC1\n\
	IN IBUF, IBUFSIZE ; IN();\n\
	LD gr7, gr0 ; inp = 0; }\n\
RC1\n\
	CPA gr7, IBUFSIZE ; if(inp == IBUFSIZE) {\n\
	JNZ RC2\n\
	LD gr5, NEWLINE ; *gr1 = '¥n';\n\
	ST gr5, 0,gr1\n\
	ST gr0, IBUFSIZE ; IBUFSIZE = INP = 0;\n\
	ST gr0, INP\n\
	JUMP RC3 ; }\n\
RC2 ; else {\n\
	LD gr5, IBUF,gr7 ; *gr1 = *inp++;\n\
	ADDA gr7, ONE\n\
	ST gr5, 0,gr1\n\
	ST gr7, INP ; INP = inp;\n\
RC3 ; }\n\
	RPOP\n\
	RET\n\
READINT\n\
; gr1 が指す番地に整数値一つを読み込む\n\
RPUSH\n\
RI1 ; do {\n\
	CALL READCHAR ; ch = READCHAR();\n\
	LD gr7, 0,gr1\n\
	CPA gr7, SPACE ; } while(ch==' ' || ch=='¥t' || ch=='¥n');\n\
	JZE RI1\n\
	CPA gr7, TAB\n\
	JZE RI1\n\
	CPA gr7, NEWLINE\n\
	JZE RI1\n\
	LD gr5, ONE ; flag = 1\n\
	CPA gr7, MINUS ; if(ch == '-') {\n\
	JNZ RI4\n\
	LD gr5, gr0 ; flag = 0;\n\
	CALL READCHAR ; ch = READCHAR();\n\
	LD gr7, 0,gr1\n\
RI4 ; }\n\
	LD gr6, gr0 ; v = 0;\n\
RI2\n\
	CPA gr7, ZERO ; while('0' <= ch && ch <= '9') {\n\
	JMI RI3\n\
	CPA gr7, NINE\n\
	JPL RI3\n\
	MULA gr6, TEN ; v = v*10+ch-'0';\n\
	ADDA gr6, gr7\n\
	SUBA gr6, ZERO\n\
	CALL READCHAR ; ch = READSCHAR();\n\
	LD gr7, 0,gr1\n\
	JUMP RI2 ; }\n\
RI3\n\
	ST gr7, RPBBUF ; ReadPushBack();\n\
	ST gr6, 0,gr1 ; *gr1 = v;\n\
	CPA gr5, gr0 ; if(flag == 0) {\n\
	JNZ RI5\n\
	SUBA gr5, gr6 ; *gr1 = -v;\n\
	ST gr5, 0,gr1\n\
RI5 ; }\n\
	RPOP\n\
	RET\n\
READSKIPTONEWLINE\n\
; 入力を改行コードまで（改行コードも含む）読み飛ばす\n\
	ST gr0, IBUFSIZE\n\
	ST gr0, INP\n\
	ST gr0, RPBBUF\n\
	RET\n\
FALSE DC 0\n\
TRUE DC 1\n\
ZERO DC 0\n\
ONE DC 1\n\
SIX DC 6\n\
TEN DC 10\n\
SPACE DC #0020 ; ' '\n\
MINUS DC #002D ; '-'\n\
TAB DC #0009 ; '¥t'\n\
ZERO DC #0030 ; '0'\n\
NINE DC #0039 ; '9'\n\
NEWLINE DC #000A ; '¥n'\n\
INTBUF DS 8\n\
OBUFSIZE DC 0\n\
IBUFSIZE DC 0\n\
INP DC 0\n\
OBUF DS 257\n\
IBUF DS 257\n\
RPBBUF DC 0\n\
	END\n");
	}

	free_tree(node_SPROGRAM);
	end_scan();


	return 0;
}

