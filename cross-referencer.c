#include "cross-referencer.h"


void fill_node_data(SyntaxTreeNode* node){

	if(node == NULL) return;

	if(node->parse_result != PARSERESULT_MATCH) {
		fill_node_data(node->child);
		fill_node_data(node->brother);
		return;
	}

	switch(node->s_elem_it){
	case SPROGRAM:
		node->data = calloc(1, sizeof(ProgData));

		((ProgData*)node->data)->defined_line = node->line_num;
		strcpy(((ProgData*)node->data)->name, node->child->brother->string_attr);

		fill_node_data(node->child);
		fill_node_data(node->brother);
		break;

	case SSUBPROGDEC:
		node->data = calloc(1, sizeof(ProcData));
		
		((ProcData*)node->data)->defined_line = node->line_num;
		strcpy(((ProcData*)node->data)->name, node->child->brother->child->string_attr);

		fill_node_data(node->child);
		fill_node_data(node->brother);
		break;

	case SVARDEC:
	case SVARDEC_5_0:
	case SFORMPARAM:
	case SFORMPARAM_4_0:
		{
			/* i know what it means and i know it works */
			SyntaxTreeNode* node_SVARNAME;
			SyntaxTreeNode* node_SVARNAMES_1_0;
			VarDecData* var_dec_data;
			void *type;
			int is_param = (node->s_elem_it == SFORMPARAM ||
							 node->s_elem_it == SFORMPARAM_4_0) ? 1 : 0;

			if(node->s_elem_it == SVARDEC_5_0){
				fill_node_data(node->child->brother->brother);
				type = node->child->brother->brother->data;
			}
			else{
				fill_node_data(node->child->brother->brother->brother);
				type = node->child->brother->brother->brother->data;
			}

			if(node->s_elem_it == SVARDEC || node->s_elem_it == SFORMPARAM){
				node_SVARNAME = node->child->brother->child;
			}
			else{
				node_SVARNAME = node->child->child;
			}

			node_SVARNAME->data = calloc(1, sizeof(VarData));
			((VarData*)node_SVARNAME->data)->is_declaration = 1;
			((VarData*)node_SVARNAME->data)->data = calloc(1, sizeof(VarDecData));
			var_dec_data = (VarDecData*)((VarData*)node_SVARNAME->data)->data;
			strcpy(var_dec_data->name, node_SVARNAME->child->string_attr);
			var_dec_data->is_param = is_param;
			var_dec_data->line = node_SVARNAME->line_num;
			var_dec_data->type.stdtype = ((Type*) type)->stdtype;
			var_dec_data->type.array_size = ((Type*) type)->array_size;

			if(node->s_elem_it == SVARDEC || node->s_elem_it == SFORMPARAM){
				node_SVARNAMES_1_0 = node->child->brother->child->brother->child;
			}
			else{
				node_SVARNAMES_1_0 = node->child->child->brother->child;
			}

			for(; (node_SVARNAMES_1_0 != NULL &&
					node_SVARNAMES_1_0->parse_result == PARSERESULT_MATCH);
					node_SVARNAMES_1_0 = node_SVARNAMES_1_0->brother){
				SyntaxTreeNode *node_SVARNAME = node_SVARNAMES_1_0->child->brother;
				node_SVARNAME->data = calloc(1, sizeof(VarData));
				((VarData*)node_SVARNAME->data)->is_declaration = 1;
				((VarData*)node_SVARNAME->data)->data = calloc(1, sizeof(VarDecData));
				var_dec_data = (VarDecData*)((VarData*)node_SVARNAME->data)->data;
				strcpy(var_dec_data->name, node_SVARNAME->child->string_attr);
				var_dec_data->is_param = is_param;
				var_dec_data->line = node_SVARNAME->line_num;
				var_dec_data->type.stdtype = ((Type*) type)->stdtype;
				var_dec_data->type.array_size = ((Type*) type)->array_size;
			}


			fill_node_data(node->child);
			break;
		}

	case STYPE:
		{
			Type *type = calloc(1, sizeof(Type));
			SyntaxTreeNode *child = node->child;
			SyntaxTreeNode *node_stdtype;
			while(child->parse_result != PARSERESULT_MATCH){
				child = child->brother;
			}
			if(child->s_elem_it == SSTDTYPE){
				node_stdtype = child->child;
				while(node_stdtype->parse_result != PARSERESULT_MATCH){
					node_stdtype = node_stdtype->brother;
				}
				type->stdtype = node_stdtype->s_elem_it;
				type->array_size = 0;
			}
			else /* if(node->child->s_elem_it == SARRAYTYPE) */{
				node_stdtype = child->child->brother->brother->
								brother->brother->brother->child;
				while(node_stdtype->parse_result != PARSERESULT_MATCH){
					node_stdtype = node_stdtype->brother;
				}
				type->stdtype = node_stdtype->s_elem_it;
				type->array_size = atoi(child->child->brother->brother->string_attr);
			}
			node->data = (void*) type;

		}
	default:
		fill_node_data(node->child);
		fill_node_data(node->brother);

	}
	
}

int fill_var_data(SyntaxTreeNode* node, SyntaxTreeNode* namespace, SyntaxTreeNode* global){
	int result_child = 1;
	int result_brother = 1;

	if(node == NULL) return 1;

	if(node->parse_result != PARSERESULT_MATCH) {
		result_child = fill_var_data(node->child, namespace, global);
		result_brother = fill_var_data(node->brother, namespace, global);
		return (result_child && result_brother) ? 1 : 0;
	}

	switch(node->s_elem_it){
	case SPROGRAM:

		namespace = node;
		global = node;
		result_child = fill_var_data(node->child, namespace, global);
		break;

	case SSUBPROGDEC:{
		SyntaxTreeNode* prev_namespace = namespace;
		ProgData *prog_data = (ProgData*)namespace->data;
		ProcData *proc_data = prog_data->proc_data_head;

		if(prog_data->proc_data_tail == NULL){
			prog_data->proc_data_head = (ProcData*)node->data;
			prog_data->proc_data_tail = (ProcData*)node->data;
		}
		else{
			prog_data->proc_data_tail->next = (ProcData*)node->data;
			prog_data->proc_data_tail = (ProcData*)node->data;
		}

		while(proc_data != NULL && proc_data != prog_data->proc_data_tail){
			if(strcmp(((ProcData*)node->data)->name, proc_data->name) == 0){
				printf("error : redefinition subprogram \"%s\"\n", proc_data->name);
				return 0;
			}
			proc_data = proc_data->next;
		}

		namespace = node;
		result_child = fill_var_data(node->child, namespace, global);
		result_brother = fill_var_data(node->brother, namespace, global);
		namespace = prev_namespace;
		break;
	}

	case SVARNAME:
		/* 
		 * node->data is not NULL => data assigned in fill_node_data() => define stat
		 * node->data is NULL => not define stat => reference stat
		 */

		/* reference */
		if(node->data == NULL){
			VarData* var_data;
			char* var_name = node->child->string_attr;
			/*printf("l%2d ref of %s\n",
					node->child->line_num, node->child->string_attr);*/

			switch(namespace->s_elem_it){
			case SSUBPROGDEC:
				var_data = ((ProcData*)namespace->data)->var_data_head;

				while(var_data != NULL){
					VarDecData* var_dec_data = var_data->data;
					if(strcmp(var_name, var_dec_data->name) == 0){
						VarRefData* var_ref_data;
						/*printf("namespace : procedure %s\n", ((ProcData*)namespace->data)->name);*/
						node->data = malloc(sizeof(VarData));
						((VarData*)node->data)->is_declaration = 0;
						((VarData*)node->data)->data = malloc(sizeof(VarRefData));
						var_ref_data = ((VarData*)node->data)->data;
						var_ref_data->line = node->child->line_num;
						var_ref_data->data = var_data;
						break;
					}
					var_data = var_data->next;
				}

				if(node->data != NULL){
					break;
				}

				/* fall through and check global */

			case SPROGRAM:
				var_data = ((ProgData*)global->data)->var_data_head;

				while(var_data != NULL){
					VarDecData* var_dec_data = var_data->data;
					if(strcmp(var_name, var_dec_data->name) == 0){
						VarRefData* var_ref_data;
						/*printf("namespace : global\n");*/
						node->data = malloc(sizeof(VarData));
						((VarData*)node->data)->is_declaration = 0;
						((VarData*)node->data)->data = malloc(sizeof(VarRefData));
						var_ref_data = ((VarData*)node->data)->data;
						var_ref_data->line = node->child->line_num;
						var_ref_data->data = var_data;

						break;
					}
					var_data = var_data->next;
				}
				if(node->data != NULL){
					break;
				}

				/* not found */
				printf("error : line %d \"%s\" undeclared\n",
						node->child->line_num, var_name);
				return 0;
			}

			result_brother = fill_var_data(node->brother, namespace, global);
			break;
		}
		/* define */
		else{
			VarData *var_data;
			VarData *var_data_tail;
			if(namespace->s_elem_it == SPROGRAM){
				ProgData *prog_data = (ProgData*)namespace->data;
				if(prog_data->var_data_tail == NULL){
					prog_data->var_data_head = (VarData*)node->data;
					prog_data->var_data_tail = (VarData*)node->data;
				}
				else{
					prog_data->var_data_tail->next = (VarData*)node->data;
					prog_data->var_data_tail = (VarData*)node->data;
				}
				var_data = prog_data->var_data_head;
				var_data_tail = prog_data->var_data_tail;
			}
			else/* if(namespace->s_elem_it == SSUBPROGDEC) */{
				ProcData *proc_data = (ProcData*)namespace->data;
				if(proc_data->var_data_tail == NULL){
					proc_data->var_data_head = (VarData*)node->data;
					proc_data->var_data_tail = (VarData*)node->data;
				}
				else{
					proc_data->var_data_tail->next = (VarData*)node->data;
					proc_data->var_data_tail = (VarData*)node->data;
				}
				var_data = proc_data->var_data_head;
				var_data_tail = proc_data->var_data_tail;
			}

			while(var_data != NULL && var_data != var_data_tail){
				char* node_name = ((VarDecData*)((VarData*)node->data)->data)->name;
				char* loop_name = ((VarDecData*)var_data->data)->name;
				if(strcmp(node_name, loop_name) == 0){
					printf("error : redefinition variable \"%s\"\n", 
							node_name);
					return 0;
				}
				var_data = var_data->next;
			}

			result_brother = fill_var_data(node->brother, namespace, global);
			break;

		}

	case SCONST:{
		/* SCONST
		 *  L> TNUMBER -> TFALSE -> TTRUE -> TSTRING (just 1 character)
		 */
		ConstData* const_data;
		SyntaxTreeNode* child = node->child;
		node->data = malloc(sizeof(ConstData));
		const_data = node->data;

		while(child->parse_result != PARSERESULT_MATCH){
			child = child->brother;
		}
		switch(child->s_elem_it){
		case TNUMBER:
			const_data->type.stdtype = TINTEGER;
			const_data->type.array_size = 0;
			const_data->val = atoi(child->string_attr);
			break;
		case TFALSE:
			const_data->type.stdtype = TBOOLEAN;
			const_data->type.array_size = 0;
			const_data->val = 0;
			break;
		case TTRUE:
			const_data->type.stdtype = TBOOLEAN;
			const_data->type.array_size = 0;
			const_data->val = 1;
			break;
		case TSTRING:
			if(strlen(child->string_attr) > 1){
				printf("string of const stat has just one character\n");
				return 0;
			}
			const_data->type.stdtype = TBOOLEAN;
			const_data->type.array_size = 0;
			const_data->val = child->string_attr[0];
			break;
		}
		return 1;
	}


	default:
		result_child = fill_var_data(node->child, namespace, global);
		result_brother = fill_var_data(node->brother, namespace, global);
		break;
	}

	return (result_child && result_brother) ? 1 : 0;

}

void print_variable(SyntaxTreeNode* node){

	if(node == NULL) return;

	if(node->parse_result != PARSERESULT_MATCH) {
		print_variable(node->child);
		print_variable(node->brother);
		return;
	}

	if(node->data != NULL){
		if(node->s_elem_it == SPROGRAM){
			printf("program ");
			printf("l%2d ", ((ProgData*)node->data)->defined_line);
			printf("dp%9p ", node->data);
			printf("vh%9p ", ((ProgData*)node->data)->var_data_head);
			printf("ph%9p ", ((ProgData*)node->data)->proc_data_head);
			printf("%s ", ((ProgData*)node->data)->name);
			printf("\n");
		}
		else if(node->s_elem_it == SSUBPROGDEC){
			printf("procedu ");
			printf("l%2d ", ((ProcData*)node->data)->defined_line);
			printf("dp%9p ", node->data);
			printf("vh%9p ", ((ProcData*)node->data)->var_data_head);
			printf("np%9p ", ((ProcData*)node->data)->next);
			printf("%s ", ((ProcData*)node->data)->name);
			printf("\n");
		}
		else if(node->s_elem_it == SVARNAME){
			VarData* var_data = (VarData*)node->data;
			if(var_data->is_declaration){
				VarDecData* var_dec_data = (VarDecData*)var_data->data;
				char type[][5] = {"char", "int ", "bool"};
				printf("%s%d ", type[var_dec_data->type.stdtype - TCHAR],
									var_dec_data->type.array_size);
				printf("%s ", var_dec_data->is_param ? "p" : "-");
				printf("l%2d ", var_dec_data->line);
				printf("dp%9p ", node->data);
				printf("rh%9p ", var_dec_data->ref_head);
				printf("np%9p ", var_data->next);
				printf("%s ", var_dec_data->name);
				printf("\n");
			}
			else{
				VarRefData* var_ref_data = (VarRefData*)var_data->data;
				VarDecData* var_dec_data = (VarDecData*)((VarData*)var_ref_data->data)->data;
				char type[][5] = {"char", "int ", "bool"};
				printf("var ref ");
				printf("l%2d ", var_ref_data->line);
				printf(" p%9p ", var_ref_data->data);

				printf("%s%d ", type[var_dec_data->type.stdtype - TCHAR],
									var_dec_data->type.array_size);
				printf("%s ", var_dec_data->name);
				printf("\n");
			}
		}
		else if(node->s_elem_it == STYPE){
			Type* type = (Type*)node->data;
			char arr[][5] = {"char", "int ", "bool"};
			printf("%s%d ", arr[type->stdtype - TCHAR], type->array_size);
			printf("\n");
		}
		else{
			printf("some %s\n", SYNTAXDIC[node->s_elem_it]);
		}
	}
	print_variable(node->child);
	print_variable(node->brother);
}

int check_type(SyntaxTreeNode* node){
	int result_child = 1;
	int result_brother = 1;

	if(node == NULL) return 1;

	if(node->parse_result != PARSERESULT_MATCH) {
		result_child = check_type(node->child);
		result_brother = check_type(node->brother);
		return (result_child && result_brother) ? 1 : 0;
	}

	printf("check_type l%2d %s\n", node->line_num, SYNTAXDIC[node->s_elem_it]);
	fflush(stdout);

	switch(node->s_elem_it){
	case SSUBPROGDEC:
		/* SSUBPROGDEC
		 *  L>TPROCEDURE->SPROCEDURENAME->SSUBPROGDEC_2->TSEMI->SSUBPROGDEC_4->SCOMPSTAT->TSEMI
		 */
		if(!check_type(node->child->brother->brother->brother->brother->brother)){
			printf("error at SUBPROGDEC\n");
			return 0;
		}
		return 1;

	case SVAR:
		/* hoge[piyo] (array) */
		/* SVAR
		 *  L> SVARNAME -> SVAR_1
		 *                  L> SVAR_1_0
		 *                      L> TLSQPAREN -> (*)SEXPR -> TRSQPAREN
		 */

		/* hoge (not array) */
		/* SVAR
		 *  L> SVARNAME -> SVAR_1
		 *                  L> SVAR_1_0(EMPTY)
		 */

		/* reference statement looks like array */
		if(node->child->brother->child->parse_result == PARSERESULT_MATCH){
			VarDecData* var_dec_data = 
				(VarDecData*)((VarData*)((VarRefData*)((VarData*)node->child->data)->data)->data)->data;
			SyntaxTreeNode* node_SEXPR = node->child->brother->child->child->brother;
			Type* node_SEXPR_type;
			Type* node_SVAR_type;
			if(var_dec_data->type.array_size == 0){
				printf("error : variable \"%s\" (defined at line %d) is not array\n", 
						var_dec_data->name, var_dec_data->line);
				return 0;
			}
			if(!check_type(node_SEXPR)){
				printf("error at array[int]\n");
				return 0;
			}
			node_SEXPR_type = (Type*)node_SEXPR->data;
			if(node_SEXPR_type->stdtype != TINTEGER || node_SEXPR_type->array_size != 0){
				printf("error : index of array is not an integer\n");
				return 0;
			}
			node->data = malloc(sizeof(Type));
			node_SVAR_type = (Type*)node->data;
			node_SVAR_type->stdtype = var_dec_data->type.stdtype;
			node_SVAR_type->array_size = 0;
			printf("svar (array) ok\n");
			return 1;
		}
		/* reference statement looks like NOT-array */
		else/* if(node->child->brother->child->parse_result == PARSERESULT_EMPTY) */{
			VarDecData* var_dec_data = 
				(VarDecData*)((VarData*)((VarRefData*)((VarData*)node->child->data)->data)->data)->data;
			Type* node_SVAR_type;
			if(var_dec_data->type.array_size != 0){
				printf("error : variable \"%s\" (defined at line %d) is array\n", 
						var_dec_data->name, var_dec_data->line);
				return 0;
			}
			node->data = malloc(sizeof(Type));
			node_SVAR_type = (Type*)node->data;
			node_SVAR_type->stdtype = var_dec_data->type.stdtype;
			node_SVAR_type->array_size = 0;
			printf("svar (not array) ok\n");
			return 1;
		}

	case SCONDSTAT:{
		/* SCONDSTAT
		 *  L> TIF -> (*)SEXPR -> TTHEN -> SSTAT -> SCONDSTAT_4
		 *	                                         L> SCONDSTAT_4_0
		 *                                               L> TELSE -> SSTAT
		 */
		SyntaxTreeNode* node_SEXPR = node->child->brother;
		Type* type;
		if(!check_type(node_SEXPR)){
			printf("error at if EXPR then \n");
			return 0;
		}
		type = (Type*)node_SEXPR->data;
		if(type->stdtype != TBOOLEAN || type->array_size != 0){
			printf("error : conditional sentence is not boolean\n");
			return 0;
		}
		return 1;
	}

	case SEXPR:{
		/* SEXPR
		 *  L> SSIMPLEEXPR -> SEXPR_1
		 *                     L> SEXPR_1_0
		 *                         L> SRELATOP -> SSIMPLEEXPR
		 */
		/* operator has left-to-right associativity */
		SyntaxTreeNode* node_SSIMPLEEXPR = node->child;
		SyntaxTreeNode* node_SEXPR_1_0 = node->child->brother->child;
		Type* type;

		node->data = malloc(sizeof(Type));
		type = (Type*) node->data;

		if(!check_type(node_SSIMPLEEXPR)){
			printf("error at SIMPLEEXPR\n");
			return 0;
		}
		type->stdtype = ((Type*)node_SSIMPLEEXPR->data)->stdtype;
		type->array_size = ((Type*)node_SSIMPLEEXPR->data)->array_size;

		while(node_SEXPR_1_0->parse_result != PARSERESULT_EMPTY){
			SyntaxTreeNode* node_rel_op = node_SEXPR_1_0->child->child;
			Type* node_SSIMPLEEXPR_type;

			if(!check_type(node_SEXPR_1_0->child->brother)){
				printf("error at EXPR_1_0\n");
				return 0;
			}
			while(node_rel_op->parse_result != PARSERESULT_MATCH){
				node_rel_op = node_rel_op->brother;
			}

			node_SSIMPLEEXPR_type = (Type*)node_SEXPR_1_0->child->brother->data;
			if(type->stdtype != node_SSIMPLEEXPR_type->stdtype ||
					 type->array_size != 0 || node_SSIMPLEEXPR_type->array_size != 0){
				printf("error : operand type of operator \"%s\"\n",
						SYNTAXDIC[node_rel_op->s_elem_it]);
				return 0;
			}

			node_SEXPR_1_0 = node_SEXPR_1_0->brother;
		}
		
		return 0;
	}

	case SSIMPLEEXPR:{
		/* SSIMPLEEXPR
		 *  L> SSIMPLEEXPR_0 -> STERM -> SSIMPLEEXPR_2
		 *      L> SSIMPLEEXPR_0_0        |
		 *          L> TPLUS -> TMINUS    |
		 *                                L> SSIMPLEEXPR_2_0
		 *                                    L> SADDOP -> STERM
		 */	
		SyntaxTreeNode* node_STERM = node->child->brother;
		SyntaxTreeNode* node_SSIMPLEEXPR_2_0 = node->child->brother->brother->child;
		Type* type;

		node->data = malloc(sizeof(Type));
		type = (Type*) node->data;

		if(!check_type(node_STERM)){
			printf("error at TERM\n");
			return 0;
		}
		type->stdtype = ((Type*)node_STERM->data)->stdtype;
		type->array_size = ((Type*)node_STERM->data)->array_size;

		while(node_SSIMPLEEXPR_2_0->parse_result != PARSERESULT_DIFFERENCE){
			SyntaxTreeNode* node_add_op = node_SSIMPLEEXPR_2_0->child->child;
			int required_type = TINTEGER;
			Type* node_SFACTOR_type;

			if(!check_type(node_SSIMPLEEXPR_2_0->child->brother)){
				printf("error at TERM_1_0\n");
				return 0;
			}
			while(node_add_op->parse_result != PARSERESULT_MATCH){
				node_add_op = node_add_op->brother;
			}
			if(node_add_op->s_elem_it == TAND){
				required_type = TBOOLEAN;
			}

			printf("koko\n");
			printf("%p %s\n", node_SSIMPLEEXPR_2_0, SYNTAXDIC[node_SSIMPLEEXPR_2_0->s_elem_it]);
			printf("%p %s\n", node_SSIMPLEEXPR_2_0->child,
						SYNTAXDIC[node_SSIMPLEEXPR_2_0->child->s_elem_it]);
			printf("%p %s\n", node_SSIMPLEEXPR_2_0->child->brother,
						SYNTAXDIC[node_SSIMPLEEXPR_2_0->child->brother->s_elem_it]);
			node_SFACTOR_type = (Type*)node_SSIMPLEEXPR_2_0->child->brother->data;

			node_SFACTOR_type = (Type*)node_SSIMPLEEXPR_2_0->child->brother->data;
			if(type->stdtype != required_type || type-> array_size != 0){
				printf("error : left operand type of operator \"%s\"\n",
						SYNTAXDIC[node_add_op->s_elem_it]);
				return 0;
			}
			if(node_SFACTOR_type->stdtype != required_type ||
					node_SFACTOR_type-> array_size != 0){
				printf("error : right operand type of operator \"%s\"\n",
						SYNTAXDIC[node_add_op->s_elem_it]);
				return 0;
			}

			node_SSIMPLEEXPR_2_0 = node_SSIMPLEEXPR_2_0->brother;

		}
		
		return 0;
	}

	case STERM:{
		/* STERM
		 *  L> SFACTOR -> STERM_1
		 *                 L> STERM_1_0
		 *                     L> SMULOP -> SFACTOR
		 */
		/* operator has left-to-right associativity */
		SyntaxTreeNode* node_SFACTOR = node->child;
		SyntaxTreeNode* node_STERM_1_0 = node->child->brother->child;
		Type* type;

		node->data = malloc(sizeof(Type));
		type = (Type*)node->data;

		if(!check_type(node_SFACTOR)){
			printf("error at FACTOR\n");
			return 0;
		}
		type->stdtype = ((Type*)node_SFACTOR->data)->stdtype;
		type->array_size = ((Type*)node_SFACTOR->data)->array_size;

		while(node_STERM_1_0->parse_result != PARSERESULT_DIFFERENCE){
			SyntaxTreeNode* node_mul_op = node_STERM_1_0->child->child;
			int required_type = TINTEGER;
			Type* node_SFACTOR_type;

			if(!check_type(node_STERM_1_0->child->brother)){
				printf("error at FACTOR under TERM\n");
				return 0;
			}
			while(node_mul_op->parse_result != PARSERESULT_MATCH){
				node_mul_op = node_mul_op->brother;
			}
			if(node_mul_op->s_elem_it == TAND){
				required_type = TBOOLEAN;
			}

			node_SFACTOR_type = (Type*)node_STERM_1_0->child->brother->data;
			if(type->stdtype != required_type || type-> array_size != 0){
				printf("error : left operand type of operator \"%s\"\n",
						SYNTAXDIC[node_mul_op->s_elem_it]);
				return 0;
			}
			if(node_SFACTOR_type->stdtype != required_type ||
					node_SFACTOR_type-> array_size != 0){
				printf("error : right operand type of operator \"%s\"\n",
						SYNTAXDIC[node_mul_op->s_elem_it]);
				return 0;
			}

			type->stdtype = required_type;
			type->array_size = 0;

			node_STERM_1_0 = node_STERM_1_0->brother;

		}
		
		return 1;
	}

	case SFACTOR:{
		/* SFACTOR
		 *  L> SVAR -> SCONST -> SFACTOR_2 -> SFACTOR_3 -> SFACTOR_4
		 */
		SyntaxTreeNode* child = node->child;
		while(1){
			if(child->parse_result != PARSERESULT_MATCH){
				child = child->brother;
				continue;
			}
			if(child->s_elem_it == SCONST){
				node->data = malloc(sizeof(Type));
				((Type*)node->data)->stdtype = ((ConstData*)child->data)->type.stdtype;
				((Type*)node->data)->array_size = ((ConstData*)child->data)->type.array_size;
			}
			else{
				if(!check_type(child)){
					printf("error at %s\n", SYNTAXDIC[child->s_elem_it]);
					return 0;
				}
				node->data = malloc(sizeof(Type));
				((Type*)node->data)->stdtype = ((Type*)child->data)->stdtype;
				((Type*)node->data)->array_size = ((Type*)child->data)->array_size;
			}
			return 1;
		}
		printf("nobody can come here just for debug\n");
		return 0;
	}

	case SCONST:{
		/* SCONST
		 *  L> TNUMBER -> TFALSE -> TTRUE -> TSTRING(just 1 character)
		 */
		SyntaxTreeNode* child = node->child;
		while(1){
			if(child->parse_result != PARSERESULT_MATCH){
				child = child->brother;
			}
			check_type(child);
			node->data = malloc(sizeof(Type));
			((Type*)node->data)->stdtype = ((Type*)child->data)->stdtype;
			((Type*)node->data)->array_size = ((Type*)child->data)->array_size;
			if(child->s_elem_it == TSTRING){
				/* just 1 character */
				if(((Type*)node->data)->array_size > 1){
					printf("error : const val needed just one character\n");
					return 0;
				}
			}
			return 1;
		}
		printf("nobody can come here just for debug\n");
		return 0;
	}

	case SFACTOR_2:{
		/* SFACTOR_2
		 *  L> TLPAREN -> SEXPR -> TRPAREN
		 */
		SyntaxTreeNode* child = node->child->brother;
		if(check_type(child)){
			node->data = malloc(sizeof(Type));
			((Type*)node->data)->stdtype = ((Type*)child->data)->stdtype;
			((Type*)node->data)->array_size = ((Type*)child->data)->array_size;
			return 1;
		}

		printf("error at ( EXPR )\n");
		return 0;
	}

	case SFACTOR_3:{
		/* SFACTOR_3
		 *  L> TNOT -> SFACTOR
		 */
		SyntaxTreeNode* child = node->child->brother;
		if(check_type(child)){
			Type* type;
			node->data = malloc(sizeof(Type));
			type = (Type*)node->data;
			type->stdtype = ((Type*)child->data)->stdtype;
			type->array_size = ((Type*)child->data)->array_size;
			if(type->stdtype == TBOOLEAN && type->array_size == 0){
				return 1;
			}
			printf("error : boolean value required after \"not\" token \n");
			return 0;

		}

		printf("error at NOT FACTOR\n");
		return 0;
	}

	case SFACTOR_4:{
		/* SFACTOR_4
		 *  L> SSTDTYPE -> TLPAREN -> SEXPR -> TRPAREN
		 */
		SyntaxTreeNode* child = node->child->brother;
		if(check_type(child)){
			Type* type;
			node->data = malloc(sizeof(Type));
			type = (Type*)node->data;
			type->stdtype = ((Type*)child->data)->stdtype;
			type->array_size = ((Type*)child->data)->array_size;
			if(type->stdtype == TBOOLEAN && type->array_size == 0){
				return 1;
			}
			printf("error : boolean value required after \"not\" token \n");
			return 0;

		}

		printf("error at STDTYPE(EXPR)\n");
		return 0;
	}

	default:
		result_child = check_type(node->child);
		result_brother = check_type(node->brother);
		break;

	}

	return (result_child && result_brother) ? 1 : 0;

}

int main(int nc, char *np[]){
	SyntaxTreeNode *node_SPROGRAM;

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

	fill_node_data(node_SPROGRAM);

	if(!fill_var_data(node_SPROGRAM, NULL, NULL)){
		free_tree(node_SPROGRAM);
		end_scan();
		return -1;
	}


	debug_tree(node_SPROGRAM);

	print_variable(node_SPROGRAM);

	if(!check_type(node_SPROGRAM)){
		printf("error found.\n");
	}

	free_tree(node_SPROGRAM);
	end_scan();

	return 0;
}