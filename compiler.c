#include "compiler.h"

void generate_assm(SyntaxTreeNode* node){

	if(node == NULL) return;


	if(node->parse_result != PARSERESULT_MATCH){
		generate_assm(node->child);
		generate_assm(node->brother);
		return;
	}

	/*printf("\t\tgen assm %s\n", SYNTAXDIC[node->s_elem_it]);
	fflush(stdout);*/

	switch(node->s_elem_it){
	case SPROGRAM:{
		/* SPROGRAM
		 *  L>TPROGRAM->TNAME->TSEMI->SBLOCK->TDOT
		 */
		SyntaxTreeNode* node_SBLOCK = node->child->brother->brother->brother;

		iw_comment	("start SPROGRAM");
		iw_START	(get_label(node));
		iw_LAD		("", gr0, "0");
		iw_CALL		("", get_label(node_SBLOCK));
		iw_CALL		("", "FLUSH");
		iw_SVC		("", "0");
		generate_assm(node_SBLOCK);
		break;
	}

	case SBLOCK:{
		/* SBLOCK
		 *  L>SBLOCK_0->SCOMPSTAT
		 */
		SyntaxTreeNode* node_SBLOCK_0 = node->child;
		SyntaxTreeNode* node_SCOMPSTAT = node_SBLOCK_0->brother;
		node_SBLOCK_0->brother = NULL;
		generate_assm	(node_SBLOCK_0);
		iw_comment		("start SCOMPSTAT under SBLOCK");
		iw_label		(get_label(node));
		generate_assm	(node_SCOMPSTAT);
		iw_RET			("");
		iw_comment		("end   SCOMPSTAT under SBLOCK");
		node_SBLOCK_0->brother = node_SCOMPSTAT;
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

			/*iw_DS(get_label(node), size);*/
			add_footer_ds(get_label(node), size);
		}
		else{
			/*VarRefData var_ref_data = (VarRefData*)var_data->data;*/
			iw_LD	("", gr1, get_label(node));
		}
		generate_assm(node->brother);
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
		ProcData* proc_data = (ProcData*)node->data;
		VarData* var_data = proc_data->var_data_tail;

		iw_comment		("start SSUBPROGDEC");
		generate_assm	(node_SSUBPROGDEC_2->child);
		generate_assm	(node_SSUBPROGDEC_4->child);
		iw_label		(get_label(node));

		/* pop address of argument (not address of parameter) reverse order */
		iw_POP			("", gr7);
		while(var_data != NULL){
			VarDecData* var_dec_data = (VarDecData*)var_data->data;
			if(var_dec_data->is_param){
				iw_POP		("", gr1);
				iw_ST		("", gr1, get_var_label(var_data));
			}
			var_data = var_data->prev;
		}
		iw_PUSH_3		("", "0", gr7);

		generate_assm	(node_SCOMPSTAT->child);
		iw_RET			("");
		iw_comment		("end   SSUBPROGDEC");
		break;
	}

	case SCONDSTAT:{
		/* SCONDSTAT
		 *  L> TIF -> (*)SEXPR -> TTHEN -> SSTAT -> SCONDSTAT_4
		 *	                                         L> SCONDSTAT_4_0
		 *                                               L> TELSE -> SSTAT
		 */

		SyntaxTreeNode* node_SEXPR = node->child->brother;
		SyntaxTreeNode* node_SSTAT = node_SEXPR->brother->brother;
		SyntaxTreeNode* node_SCONDSTAT_4 = node_SSTAT->brother;

		iw_comment		("start SCONDSTAT");
		generate_assm	(node_SEXPR);
		iw_CPA			("", gr1, gr0);
		/* if then */
		if(node_SCONDSTAT_4->parse_result != PARSERESULT_MATCH){
			iw_JZE			("", get_end_label(node));
			generate_assm	(node_SSTAT->child);

		}
		/* if then else */
		else{
			SyntaxTreeNode* node_TELSE = node_SCONDSTAT_4->child->child;
			SyntaxTreeNode* node_TSTAT_in_else = node_TELSE->brother;
			iw_JZE			("", get_label(node_TELSE));
			generate_assm	(node_SSTAT->child);
			iw_JUMP			("", get_end_label(node));
			iw_label		(get_label(node_TELSE));
			generate_assm	(node_TSTAT_in_else->child);
		}
		iw_label		(get_end_label(node));
		iw_comment		("end   SCONDSTAT");
		break;
	}

	case SITERSTAT:{
		/* SITERSTAT
		 *  L> TWHILE -> (*)SEXPR -> TDO -> SSTAT
		 */

		SyntaxTreeNode* node_SEXPR = node->child->brother;
		SyntaxTreeNode* node_SSTAT = node_SEXPR->brother->brother;

		iw_comment		("start SITERSTAT");
		iw_label		(get_label(node));
		generate_assm	(node_SEXPR);
		iw_CPA			("", gr1, gr0);
		iw_JZE			("", get_end_label(node));
		generate_assm	(node_SSTAT->child);
		iw_JUMP			("", get_label(node));
		iw_label		(get_end_label(node));
		iw_comment		("end   SITERSTAT");
		break;
	}

	case SEXITSTAT:{
		ExitData* exit_data = (ExitData*) node->data;
		SyntaxTreeNode* node_SITERSTAT = exit_data->node_SITERSTAT;
		iw_JUMP("", get_end_label(node_SITERSTAT));
		break;
	}

	case SCALLSTAT:{
		/* SCALLSTAT
		 *  L>TCALL->SPROCNAME->SCALLSTAT_2
		 *                       L>SCALLSTAT_2_0
		 *                          L>TLPAREN->SEXPRS->TRPAREN
		 *                                      L>SEXPR->SEXPRS_1
		 *                                                L>SEXPRS_1_0
		 *                                                   L>TCOMMA->SEXPR
		 */
		/* call func(x, y)
		 *
		 *  - push(x)
		 *  - push(y)
		 *  - call func
		 *  - x = pop()
		 *  - y = pop()
		 */

		ProcCallData* call_data = (ProcCallData*) node->data;
		SyntaxTreeNode* node_SCALLSTAT_2 = node->child->brother->brother;

		iw_comment("start SCALLSTAT");
		
		/* 1 or more arguments */
		if(node_SCALLSTAT_2->parse_result == PARSERESULT_MATCH){
			SyntaxTreeNode* node_SEXPR = node_SCALLSTAT_2->child->child->brother->child;
			SyntaxTreeNode* node_SEXPRS_1_0 = node_SEXPR->brother->child;
			Type* type_node_SEXPR = (Type*)node_SEXPR->data;
			iw_comment		("start before call 1st arg");
			generate_assm	(node_SEXPR);		/* gr1 <- val */
			if(type_node_SEXPR->can_assign){
				SyntaxTreeNode* node_SVAR =
						node_SEXPR->child->child->brother->child->child;
				SyntaxTreeNode* node_SVAR_1_0 = 
						node_SVAR->child->brother->child;

				if(node_SVAR_1_0->parse_result == PARSERESULT_MATCH){
					iw_PUSH_by_indexed_label("", get_label(node_SVAR), gr3);
				}
				else{
					iw_PUSH_by_label	("", get_label(node_SVAR));
				}
			}
			else{
				/*iw_DC				(get_label(node_SEXPR), 0);*/
				add_footer_dc_num	(get_label(node_SEXPR), 0);
				iw_ST				("", gr1, get_label(node_SEXPR));
				iw_PUSH_by_label	("", get_label(node_SEXPR));
			}
			iw_comment	("end   before call 1st arg");

			if(node_SEXPR->brother->parse_result == PARSERESULT_MATCH){
				iw_comment("start before call 2nd arg or later");
				while(node_SEXPRS_1_0 != NULL &&
						node_SEXPRS_1_0->parse_result == PARSERESULT_MATCH){
					node_SEXPR = node_SEXPRS_1_0->child->brother;
					type_node_SEXPR = (Type*)node_SEXPR->data;
					generate_assm(node_SEXPR);		/* gr1 <- val, gr2 -< index */
					if(type_node_SEXPR->can_assign){
						SyntaxTreeNode* node_SVAR =
								node_SEXPR->child->child->brother->child->child;
						SyntaxTreeNode* node_SVAR_1_0 = 
								node_SVAR->child->brother->child;

						if(node_SVAR_1_0->parse_result == PARSERESULT_MATCH){
							iw_PUSH_by_indexed_label("", get_label(node_SVAR), gr3);
						}
						else{
							iw_PUSH_by_label	("", get_label(node_SVAR));
						}
					}
					else{
						/*iw_DC	(get_label(node_SEXPR), 0);*/
						add_footer_dc_num	(get_label(node_SEXPR), 0);
						iw_ST				("", gr1, get_label(node_SEXPR));
						iw_PUSH_by_label	("", get_label(node_SEXPR));
					}
					node_SEXPRS_1_0 = node_SEXPRS_1_0->brother;
				}
				iw_comment	("end   before call 2nd arg or later");
			}

		}

		iw_CALL		("", get_proc_label(call_data->proc_data));
		iw_comment	("end   SCALLSTAT");

		break;

	}

	case SRETSTAT:{
		iw_RET("");
		break;
	}

	case SASSIGNSTAT:{
		/* SASSIGNSTAT
		 *  L> SLEFTPART -> TASSIGN -> SEXPR
		 *      L> SVAR
		 */
				/* var pattern 1 hoge[piyo] */
				/* SVAR
				 *  L> SVARNAME -> SVAR_1
				 *                  L> SVAR_1_0
				 *                      L>TLSQPAREN->SEXPR index->TRSQPAREN
				 */
				/* var pattern 2 hoge (check if it is parameter or not) */
				/* SVAR
				 *  L> SVARNAME -> SVAR_1
				 *                  L> SVAR_1_0(EMPTY)
				 */

		SyntaxTreeNode* node_SEXPR = node->child->brother->brother;

		SyntaxTreeNode* node_SVAR = node->child->child;
		SyntaxTreeNode* node_SVAR_1_0 = node_SVAR->child->brother->child;
		VarData* var_data = (VarData*)node_SVAR->child->data;
		VarRefData* var_ref_data = (VarRefData*)var_data->data;
		VarData* var_data_2 = (VarData*)var_ref_data->data;
		VarDecData* var_dec_data = (VarDecData*)var_data_2->data;

		iw_comment		("start assign");
		generate_assm	(node_SEXPR);
		/* pattern 1 */
		if(node_SVAR_1_0->parse_result == PARSERESULT_MATCH){
			SyntaxTreeNode* node_index = node_SVAR_1_0->child->brother;
			iw_PUSH_3		("", "0", gr1);
			generate_assm	(node_index);		/* gr1 = val */
			iw_LD			("", gr3, gr1);		/* gr2 = gr1 */

			/* if gr3 > 32768 then jump EOVF */
			iw_JOV			("", "EOVF");
			/* if gr3 >= array_size then jump EOVF */
			iw_LAD_num		("", gr2, var_dec_data->type.array_size);
			iw_CPA			("", gr3, gr2);
			iw_JMI			("", get_end_label(node));
			iw_JUMP			("", "EROV");

			iw_POP			(get_end_label(node), gr1);
			iw_ST_3			("", gr1, get_label(node_SVAR), gr3);
			iw_LD			("", gr2, get_label(node_SVAR));
		}
		/* pattern 2-1 hoge (parameter) */
		else if(var_dec_data->is_param){
			iw_PUSH_3		("", "0", gr1);
			iw_LD			("", gr2, get_label(node_SVAR));
			iw_POP			("", gr1);
			iw_ST_3			("", gr1, "0", gr2);
		}
		/* pattern 2-2 hoge (not parameter) */
		else{
			iw_ST			("", gr1, get_label(node_SVAR));
		}
		iw_comment		("end   assign");
		break;
	}

	case SVAR:{
		/* pattern 1 hoge[piyo] */
		/* SVAR
		 *  L> SVARNAME -> SVAR_1
		 *                  L> SVAR_1_0
		 *                      L> TLSQPAREN -> (*)SEXPR -> TRSQPAREN
		 */

		/* pattern 2 hoge (check if it is parameter or not) */
		/* SVAR
		 *  L> SVARNAME -> SVAR_1
		 *                  L> SVAR_1_0(EMPTY)
		 */

		SyntaxTreeNode* node_SVARNAME = node->child;
		SyntaxTreeNode* node_SVAR_1_0 = node->child->brother->child;
		VarData* var_data = (VarData*)node_SVARNAME->data;
		VarRefData* var_ref_data = (VarRefData*)var_data->data;
		VarData* var_data_2 = (VarData*)var_ref_data->data;
		VarDecData* var_dec_data = (VarDecData*)var_data_2->data;

		/* pattern 1 */
		if(node_SVAR_1_0->parse_result == PARSERESULT_MATCH){
			SyntaxTreeNode* node_SEXPR = node_SVAR_1_0->child->brother;
			generate_assm	(node_SEXPR);		/* gr1 = val */
			iw_LD			("", gr3, gr1);		/* gr2 = gr1 */
			/* if gr3 > 32768 then jump EOVF */
			iw_JOV			("", "EOVF");
			/* if gr3 >= array_size then jump EOVF */
			iw_LAD_num		("", gr2, var_dec_data->type.array_size);
			iw_CPA			("", gr3, gr2);
			iw_JMI			("", get_end_label(node));
			iw_JUMP			("", "EROV");
			iw_LD_3			(get_end_label(node),
								gr1, get_label(node_SVARNAME), gr3);
			iw_LAD			("", gr2, get_label(node_SVARNAME));
		}
		/* pattern 2-1 hoge (parameter) */
		else if(var_dec_data->is_param){
			/* gr1 <- address of argument */
			iw_LD			("", gr2, get_label(node_SVARNAME));
			iw_LD_3			("", gr1, "0", gr2);
		}
		/* pattern 2-2 hoge (not parameter) */
		else{
			iw_LD			("", gr1, get_label(node_SVARNAME));
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

		generate_assm	(node_SSIMPLEEXPR); /* gr1 = ans */

		while(node_SEXPR_1_0->parse_result != PARSERESULT_DIFFERENCE){
			SyntaxTreeNode* node_rel_op = node_SEXPR_1_0->child->child;
			node_SSIMPLEEXPR = node_SEXPR_1_0->child->brother;

			while(node_rel_op->parse_result != PARSERESULT_MATCH){
				node_rel_op = node_rel_op->brother;
			}

			iw_PUSH_3		("", "0", gr1);	/* stack.push(gr1) */
			generate_assm	(node_SSIMPLEEXPR);	/* gr1 = ans */
			iw_LD			("", gr2, gr1);	/* gr2 = gr1 */
			iw_POP			("", gr1);		/* gr1 = stack.pop() */
			/* gr1 = calc(gr1, gr2) */
			switch(node_rel_op->s_elem_it){
			case TEQUAL:iw_EQUAL("", get_end_label(node));	break;
			case TNOTEQ:iw_NOTEQ("", get_end_label(node));	break;
			case TLE:	iw_LEA	("", get_end_label(node));	break;
			case TLEEQ:	iw_LEEQA("", get_end_label(node));	break;
			case TGR:	iw_GRA	("", get_end_label(node));	break;
			case TGREQ:	iw_GREQA("", get_end_label(node));	break;
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
				iw_LD	("", gr2, gr1);	/* gr2 = gr1 */
				iw_SUBA	("", gr1, gr2);	/* gr1 -= gr2 (gr1 = 0) */
				iw_SUBA	("", gr1, gr2);	/* gr1 -= gr2 */
			}
		}

		while(node_SSIMPLEEXPR_2_0->parse_result != PARSERESULT_DIFFERENCE){
			SyntaxTreeNode* node_add_op = node_SSIMPLEEXPR_2_0->child->child;
			node_STERM = node_SSIMPLEEXPR_2_0->child->brother;

			while(node_add_op->parse_result != PARSERESULT_MATCH){
				node_add_op = node_add_op->brother;
			}

			iw_PUSH_3		("", "0", gr1);	/* stack.push(gr1) */
			generate_assm	(node_STERM);		/* gr1 = ans */
			iw_LD			("", gr2, gr1);	/* gr2 = gr1 */
			iw_POP			("", gr1);		/* gr1 = stack.pop() */
			/* gr1 = calc(gr1, gr2) */
			switch(node_add_op->s_elem_it){
			case TPLUS:	iw_ADDA	("", gr1, gr2);	break;
			case TMINUS:iw_SUBA	("", gr1, gr2);	break;
			case TOR:	iw_OR	("", gr1, gr2);	break;
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

			iw_PUSH_3		("", "0", gr1);	/* stack.push(gr1) */
			generate_assm	(node_SFACTOR);		/* gr1 = ans */
			iw_LD			("", gr2, gr1);	/* gr2 = gr1 */
			iw_POP			("", gr1);		/* gr1 = stack.pop() */
			/* gr1 = calc(gr1, gr2) */
			switch(node_add_op->s_elem_it){
			case TSTAR:	iw_MULA	("", gr1, gr2);	break;
			case TDIV:	iw_DIVA	("", gr1, gr2);	break;
			case TAND:	iw_AND	("", gr1, gr2);	break;
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
		generate_assm	(child);
		break;
	}

	case SFACTOR_2:{
		/* SFACTOR_2
		 *  L> TLPAREN -> SEXPR -> TRPAREN
		 */
		generate_assm	(node->child->brother);
		break;
	}

	case SFACTOR_3:{
		/* SFACTOR_3
		 *  L> TNOT -> SFACTOR
		 */
		/* not A <=> A xor TRUE */
		generate_assm	(node->child->brother);	/* gr1 = ans */
		iw_LD			("", gr2, "TRUE");	/* gr2 = FALSE */
		iw_XOR			("", gr1, gr2);		/* gr1 = gr1 xor gr2 */
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
		ConstData* const_data = (ConstData*)node->data;
		/*
		iw_DC	(get_label(node), const_data->val);
		iw_LD	("", gr1, get_label(node));
		*/
		add_footer_dc_num	(get_label(node), const_data->val);
		iw_LD				("", gr1, get_label(node));
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

		VarData* var_data = (VarData*)node_SVAR->child->data;
		VarRefData* var_ref_data = (VarRefData*)var_data->data;
		VarData* var_data_2 = (VarData*)var_ref_data->data;
		VarDecData* var_dec_data = (VarDecData*)var_data_2->data;

		/* parameter */
		if(var_dec_data->is_param){
			/* gr1 <- address of argument */
			iw_LD	("", gr1, get_label(node_SVAR));
		}
		/* not parameter */
		else{
			iw_LAD	("", gr1, get_label(node_SVAR));
		}

		if(type_node_SVAR->stdtype == TINTEGER){
			iw_CALL	("", "READINT");
		}
		else{
			iw_CALL	("", "READCHAR");
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

		VarData* var_data = (VarData*)node_SVAR->child->data;
		VarRefData* var_ref_data = (VarRefData*)var_data->data;
		VarData* var_data_2 = (VarData*)var_ref_data->data;
		VarDecData* var_dec_data = (VarDecData*)var_data_2->data;

		/* parameter */
		if(var_dec_data->is_param){
			/* gr1 <- address of argument */
			iw_LD	("", gr1, get_label(node_SVAR));
		}
		/* not parameter */
		else{
			iw_LAD	("", gr1, get_label(node_SVAR));
		}

		if(type_node_SVAR->stdtype == TINTEGER){
			iw_CALL	("", "READINT");
		}
		else{
			iw_CALL	("", "READCHAR");
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
			generate_assm(node_SEXPR);	/* gr1 <- expr */
			if(node_SOUTFORM_0_1->parse_result == PARSERESULT_MATCH){
				SyntaxTreeNode* node_TNUMBER = 
						node_SOUTFORM_0_1->child->child->brother;
				iw_LAD	("", gr2, node_TNUMBER->string_attr);
			}
			else{
				iw_LD	("", gr2, gr0);
			}
			switch(type_node_SEXPR->stdtype){
			case TINTEGER:
				iw_CALL	("", "WRITEINT");
				break;
			case TBOOLEAN:
				iw_CALL	("", "WRITEBOOL");
				break;
			case TCHAR:
				iw_CALL	("", "WRITECHAR");
				break;
			}
		}
		else{
			/* TSTRING is matched */
			SyntaxTreeNode* node_TSTRING = node->child->brother;
			add_footer_dc_str(get_label(node_TSTRING), node_TSTRING->string_attr);
			/*iw_DC_str	(get_label(node_TSTRING), node_TSTRING->string_attr);*/
			iw_LAD		("", gr1, get_label(node_TSTRING));
			iw_LD		("", gr2, gr0);
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
	int strlen_np1;
	char output_file_name[MAXSTRSIZE];
	/*CrossRefRecord *record_head;*/

	if(nc < 2){
		printf("missing input file name\n");
		exit(-1);
	}

	strlen_np1 = strlen(np[1]);

	if(strlen_np1 >= MAXSTRSIZE){
		printf("too long input file name\n");
		exit(-1);
	}

	if(	np[1][strlen_np1 - 4] != '.' || 
		np[1][strlen_np1 - 3] != 'm' || 
		np[1][strlen_np1 - 2] != 'p' || 
		np[1][strlen_np1 - 1] != 'l'){
		printf("input file must has the .mpl extension\n");
		exit(-1);
	}

	if(init_scan(np[1]) < 0){
		printf("can not open input file %s\n", np[1]);
		exit(-1);
	}

	strcpy(output_file_name, np[1]);
	output_file_name[strlen_np1 - 4] = '.';
	output_file_name[strlen_np1 - 3] = 'c';
	output_file_name[strlen_np1 - 2] = 's';
	output_file_name[strlen_np1 - 1] = 'l';
	output_file = fopen(output_file_name, "w");
	if(output_file == NULL){
		printf("output file can not open\n");
		exit(-1);
	}
	/*output_file = stdout;*/

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

	if(!fill_node_data(node_SPROGRAM, NULL, NULL, NULL)){
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

	dc_data_head = dc_data_tail = NULL;
	/*debug_tree(node_SPROGRAM);
	debug_variable(node_SPROGRAM);*/

	generate_assm(node_SPROGRAM);

	fprintf(output_file, FOOTER_LIB);
	print_footer_dc();
	iw_END("");

	free_tree(node_SPROGRAM);
	end_scan();


	return 0;
}

