#include "compiler.h"


char* get_label(SyntaxTreeNode* node){
	char* hoge;
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
			sprintf(hoge, "%%%s$%s",
					"%%grobal%%", var_dec_data->name);
		}
		else{
			sprintf(hoge, "%%%s$%s",
					var_dec_data->namespace->name, var_dec_data->name);
		}
	}
	else if(node->s_elem_it == SSUBPROGDEC){
		hoge = calloc(sizeof(char), MAXSTRSIZE + 10);
		sprintf(hoge, "%s%s", SYNTAXDIC[SSUBPROGDEC], ((ProcData*)node->data)->name);
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

		iw_START	(get_label(node));
		iw_LAD		("", "gr0", "0");
		iw_CALL		("", get_label(node_SBLOCK));
		iw_CALL		("", "FLUSH");
		iw_SVC		("", "0");

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

			iw_DS		(get_label(node), size);
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
		generate_assm(node_SSUBPROGDEC_2);
		generate_assm(node_SSUBPROGDEC_4);
		iw_label(get_label(node));
		generate_assm(node_SCOMPSTAT);
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

		/* pattern 1 */
		/*if(node->child->brother->child->parse_result == PARSERESULT_MATCH){

		}*/
		/* pattern 2 */
		/*else*//* if(node->child->brother->child->parse_result == PARSERESULT_EMPTY) */{

		}
		break;
	}


	case SEXPR:{
		/* SEXPR
		 *  L> SSIMPLEEXPR -> SEXPR_1
		 *                     L> SEXPR_1_0
		 *                         L> SRELATOP -> SSIMPLEEXPR
		 */
		/* operator has left-to-right associativity */

		break;
	}

	case SSIMPLEEXPR:{
		/* SSIMPLEEXPR
		 *  L> SSIMPLEEXPR_0 -> STERM -> SSIMPLEEXPR_2
		 *      L> SSIMPLEEXPR_0_0        |
		 *          L> TPLUS -> TMINUS    |
		 *                                L> SSIMPLEEXPR_2_0
		 *                                    L> SADDOP -> STERM
		 */	

		break;
	}

	case STERM:{
		/* STERM
		 *  L> SFACTOR -> STERM_1
		 *                 L> STERM_1_0
		 *                     L> SMULOP -> SFACTOR
		 */
		/* operator has left-to-right associativity */

	}

	case SFACTOR:{
		/* SFACTOR
		 *  L> SVAR -> SCONST -> SFACTOR_2 -> SFACTOR_3 -> SFACTOR_4
		 */

		break;
	}

	case SFACTOR_2:{
		/* SFACTOR_2
		 *  L> TLPAREN -> SEXPR -> TRPAREN
		 */

		break;
	}

	case SFACTOR_3:{
		/* SFACTOR_3
		 *  L> TNOT -> SFACTOR
		 */

		break;
	}

	case SFACTOR_4:{
		/* SFACTOR_4
		 *  L> SSTDTYPE -> TLPAREN -> SEXPR -> TRPAREN
		 */

		break;
	}

	case SINSTAT_1_0:{
		/* SINSTAT_1_0
		 *  L> TLPAREN -> (*)SVAR -> SINSTAT_1_0_2 -> TRPAREN
		 *                            L> SINSTAT_1_0_2_0
		 *                                L> TCOMMA -> (*)SVAR
		 */
		break;
	}

	case SOUTFORM:{
		/* SOUTFORM
		 *  L> SOUTFORM_0 -> TSTRING
		 *      L> (*)SEXPR -> SOUTFORM_0_1
		 *                      L> SOUTFORM_0_1_0
		 *                          L> TCOLON -> TNUMBER
		 */

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
	debug_tree(node_SPROGRAM);

	fp = stdout;

	generate_assm(node_SPROGRAM);


	free_tree(node_SPROGRAM);
	end_scan();


	return 0;
}

