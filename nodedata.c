#include "cross-referencer.h"

void fill_node_data_prepare(SyntaxTreeNode* node){

	if(node == NULL) return;

	if(node->parse_result != PARSERESULT_MATCH) {
		fill_node_data_prepare(node->child);
		fill_node_data_prepare(node->brother);
		return;
	}

	switch(node->s_elem_it){
	case SPROGRAM:
		node->data = mem_alloc(sizeof(ProgData));

		((ProgData*)node->data)->defined_line = node->line_num;
		strcpy(((ProgData*)node->data)->name, node->child->brother->string_attr);

		fill_node_data_prepare(node->child);
		fill_node_data_prepare(node->brother);
		break;

	case SSUBPROGDEC:
		node->data = mem_alloc(sizeof(ProcData));
		
		((ProcData*)node->data)->defined_line = node->line_num;
		/* TPROCEDURE, SPROCEDURENAME, SSUBPROGDEC_2, TSEMI, SSUBPROGDEC_4, SCOMPSTAT, TSEMI*/
		((ProcData*)node->data)->define_finished_line_pretty_printed =
			node->child->brother->brother->brother->brother->brother->brother->line_num_pretty_printed;
		strcpy(((ProcData*)node->data)->name, node->child->brother->child->string_attr);

		fill_node_data_prepare(node->child);
		fill_node_data_prepare(node->brother);
		break;

	case SVARDEC:
	case SVARDEC_5_0:
	case SFORMPARAM:
	case SFORMPARAM_4_0:{
		/* i know what it means and i know it works */
		SyntaxTreeNode* node_SVARNAME;
		SyntaxTreeNode* node_SVARNAMES_1_0;
		VarDecData* var_dec_data;
		void *type;
		int is_param = (node->s_elem_it == SFORMPARAM ||
						 node->s_elem_it == SFORMPARAM_4_0) ? 1 : 0;

		if(node->s_elem_it == SVARDEC_5_0){
			fill_node_data_prepare(node->child->brother->brother);
			type = node->child->brother->brother->data;
		}
		else{
			fill_node_data_prepare(node->child->brother->brother->brother);
			type = node->child->brother->brother->brother->data;
		}

		if(node->s_elem_it == SVARDEC || node->s_elem_it == SFORMPARAM){
			node_SVARNAME = node->child->brother->child;
		}
		else{
			node_SVARNAME = node->child->child;
		}

		node_SVARNAME->data = mem_alloc(sizeof(VarData));
		((VarData*)node_SVARNAME->data)->is_declaration = 1;
		((VarData*)node_SVARNAME->data)->data = mem_alloc(sizeof(VarDecData));
		var_dec_data = (VarDecData*)((VarData*)node_SVARNAME->data)->data;
		strcpy(var_dec_data->name, node_SVARNAME->child->string_attr);
		var_dec_data->is_param = is_param;
		var_dec_data->line = node_SVARNAME->line_num;
		var_dec_data->line_pretty_printed = node_SVARNAME->line_num_pretty_printed;
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
			node_SVARNAME->data = mem_alloc(sizeof(VarData));
			((VarData*)node_SVARNAME->data)->is_declaration = 1;
			((VarData*)node_SVARNAME->data)->data = mem_alloc(sizeof(VarDecData));
			var_dec_data = (VarDecData*)((VarData*)node_SVARNAME->data)->data;
			strcpy(var_dec_data->name, node_SVARNAME->child->string_attr);
			var_dec_data->is_param = is_param;
			var_dec_data->line = node_SVARNAME->line_num;
			var_dec_data->line_pretty_printed = node_SVARNAME->line_num_pretty_printed;
			var_dec_data->type.stdtype = ((Type*) type)->stdtype;
			var_dec_data->type.array_size = ((Type*) type)->array_size;
		}

		fill_node_data_prepare(node->child);
		fill_node_data_prepare(node->brother);
		break;
	}

	case STYPE:{
		Type *type = mem_alloc(sizeof(Type));
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
		node->data = (void*)type;
	}

	default:
		fill_node_data_prepare(node->child);
		fill_node_data_prepare(node->brother);
	}
	
}

int fill_node_data(SyntaxTreeNode* node, SyntaxTreeNode* namespace, SyntaxTreeNode* global){
	int result_child = 1;
	int result_brother = 1;

	if(node == NULL) return 1;

	if(node->parse_result != PARSERESULT_MATCH) {
		result_child = fill_node_data(node->child, namespace, global);
		result_brother = fill_node_data(node->brother, namespace, global);
		return (result_child && result_brother) ? 1 : 0;
	}

	switch(node->s_elem_it){
	case SPROGRAM:{
		namespace = node;
		global = node;
		result_child = fill_node_data(node->child, namespace, global);
		break;
	}

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
		result_child = fill_node_data(node->child, namespace, global);
		result_brother = fill_node_data(node->brother, namespace, global);
		namespace = prev_namespace;
		break;
	}

	case SVARNAME:{
		/* 
		 * node->data is not NULL => data assigned in fill_node_data_prepare() => define stat
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

						if(node->line_num_pretty_printed < var_dec_data->line_pretty_printed){
							printf("error : line %d reference to %s before declare\n", 
									node->line_num, var_name);
							return 0;
						}

						node->data = mem_alloc(sizeof(VarData));
						((VarData*)node->data)->is_declaration = 0;
						((VarData*)node->data)->data = mem_alloc(sizeof(VarRefData));
						var_ref_data = ((VarData*)node->data)->data;
						var_ref_data->line = node->child->line_num;
						var_ref_data->data = var_data;

						if(var_dec_data->ref_head == NULL){
							var_dec_data->ref_head = (VarData*)node->data;
							var_dec_data->ref_tail = (VarData*)node->data;
						}
						else{
							var_dec_data->ref_tail->next = (VarData*)node->data;
							var_dec_data->ref_tail = (VarData*)node->data;
						}

						break;
					}
					var_data = var_data->next;
				}

				if(node->data != NULL){
					break;
				}

				/* fall through and check global variables */

			case SPROGRAM:
				var_data = ((ProgData*)global->data)->var_data_head;

				while(var_data != NULL){
					VarDecData* var_dec_data = var_data->data;
					if(strcmp(var_name, var_dec_data->name) == 0){
						VarRefData* var_ref_data;
						/*printf("namespace : global\n");*/

						if(node->line_num_pretty_printed < var_dec_data->line_pretty_printed){
							printf("error : line %d reference to %s before declare\n", 
									node->line_num, var_name);
							return 0;
						}
						
						node->data = mem_alloc(sizeof(VarData));
						((VarData*)node->data)->is_declaration = 0;
						((VarData*)node->data)->data = mem_alloc(sizeof(VarRefData));
						var_ref_data = ((VarData*)node->data)->data;
						var_ref_data->line = node->child->line_num;
						var_ref_data->data = var_data;
						
						if(var_dec_data->ref_head == NULL){
							var_dec_data->ref_head = (VarData*)node->data;
							var_dec_data->ref_tail = (VarData*)node->data;
						}
						else{
							var_dec_data->ref_tail->next = (VarData*)node->data;
							var_dec_data->ref_tail = (VarData*)node->data;
						}

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

			result_brother = fill_node_data(node->brother, namespace, global);
			break;
		}
		/* define */
		else{
			VarData *var_data;
			VarData *var_data_tail;
			VarDecData* var_dec_data = (VarDecData*)((VarData*)node->data)->data;
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

				var_dec_data->namespace = NULL;
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

				var_dec_data->namespace = proc_data;
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

			result_brother = fill_node_data(node->brother, namespace, global);
			break;

		}
	}

	case SCALLSTAT:{
		/* SCALLSTAT
		 *  L> TCALL -> SPROCEDURENAME -> SCALLSTAT_2
		 *               L> TNAME          L> SCALLSTAT_2_0
		 *                                     L> TLPAREN -> SEXPRS -> TRPAREN
		 *                                                    L> SEXPR -> SEXPRS_1
		 *                                                                 L> SEXPRS_1_0
		 *                                                                     L> TCOMMA -> SEXPR
		 */
		SyntaxTreeNode* node_SCALLSTAT_2;
		SyntaxTreeNode* node_SEXPRS;
		SyntaxTreeNode* node_SEXPRS_1_0;
		ProcCallData* call_data;
		char* call_name;
		ProcData* proc_data;
		int matched = 0;

		node->data = mem_alloc(sizeof(ProcCallData));
		call_data = (ProcCallData*)node->data;
		call_data->line = node->child->line_num;

		call_name = node->child->brother->child->string_attr;

		proc_data = ((ProgData*)global->data)->proc_data_head;

		while(proc_data != NULL){
			if(strcmp(proc_data->name, call_name) != 0){
				proc_data = proc_data->next;
				continue;
			}
			matched = 1;
			break;
		}

		if(!matched){
			printf("line %d : undefined reference to procedure \"%s\"\n",
					call_data->line, call_name);
			return 0;
		}

		if(node->line_num_pretty_printed < proc_data->define_finished_line_pretty_printed){
			printf("error : line %d reference to procedure \"%s\" before declare\n", 
					node->line_num, call_name);
			return 0;
		}

		call_data->proc_data = proc_data;
		if(proc_data->ref_head == NULL){
			proc_data->ref_head = call_data;
			proc_data->ref_tail = call_data;
		}
		else{
			proc_data->ref_tail->next = call_data;
			proc_data->ref_tail = call_data;
		}

		node_SCALLSTAT_2 = node->child->brother->brother;
		if(node_SCALLSTAT_2->parse_result == PARSERESULT_EMPTY){
			break;
		}

		node_SEXPRS = node_SCALLSTAT_2->child->child->brother;
		result_child = fill_node_data(node_SEXPRS->child, namespace, global);

		node_SEXPRS_1_0 = node_SEXPRS->child->brother;
		while(node_SEXPRS_1_0 != NULL){
			int result_new_child = fill_node_data(node_SEXPRS_1_0->child->brother, namespace, global);
			result_child = result_child && result_new_child ? 1 : 0;
			node_SEXPRS_1_0 = node_SEXPRS_1_0->brother;
		}

		break;
	}

	case SCONST:{
		/* SCONST
		 *  L> TNUMBER -> TFALSE -> TTRUE -> TSTRING (just 1 character)
		 */
		ConstData* const_data;
		SyntaxTreeNode* child = node->child;
		node->data = mem_alloc(sizeof(ConstData));
		const_data = node->data;

		while(child->parse_result != PARSERESULT_MATCH){
			child = child->brother;
		}
		const_data->line = child->line_num;
		const_data->type.array_size = 0;
		switch(child->s_elem_it){
		case TNUMBER:
			const_data->type.stdtype = TINTEGER;
			const_data->val = atoi(child->string_attr);
			break;
		case TFALSE:
			const_data->type.stdtype = TBOOLEAN;
			const_data->val = 0;
			break;
		case TTRUE:
			const_data->type.stdtype = TBOOLEAN;
			const_data->val = 1;
			break;
		case TSTRING:
			if(strlen(child->string_attr) > 1){
				printf("string of const stat has just one character\n");
				return 0;
			}
			/* const_data->type.stdtype = TSTRING; */
			const_data->type.stdtype = TCHAR;
			const_data->val = child->string_attr[0];
			break;
		}
		return 1;
	}


	default:{
		result_child = fill_node_data(node->child, namespace, global);
		result_brother = fill_node_data(node->brother, namespace, global);
		break;
	}
	}

	return (result_child && result_brother) ? 1 : 0;

}
#if 0
void debug_variable(SyntaxTreeNode* node){

	if(node == NULL) return;

	if(node->parse_result != PARSERESULT_MATCH) {
		debug_variable(node->child);
		debug_variable(node->brother);
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
				printf("dp%9p ", var_data);
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
				printf("dp%9p ", var_data);
				printf("p %9p ", var_ref_data->data);

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
			printf("      type");
			printf("\n");
		}
		else if(node->s_elem_it == SCONST){
			ConstData* const_data = (ConstData*)node->data;
			char arr[][5] = {"char", "int ", "bool"};
			printf("%s%d ", arr[const_data->type.stdtype - TCHAR], 
							const_data->type.array_size);
			printf("  l%2d ", const_data->line);
			printf("const %d ", const_data->val);
			printf("\n");
		}
		else if(node->s_elem_it == SCALLSTAT){
			ProcCallData* call_data = (ProcCallData*) node->data;
			printf("call    ");
			printf("l%2d ", call_data->line);
			printf("dp%9p ", call_data);
			printf("p %9p ", call_data->proc_data);
			/*printf("np%9p ", ((ProcData*)node->data)->next);*/
			printf("\n");
		}
		else{
			printf("some data at %s\n", SYNTAXDIC[node->s_elem_it]);
		}
	}
	debug_variable(node->child);
	debug_variable(node->brother);

}
#endif

int check_type(SyntaxTreeNode* node){
	int result_child = 1;
	int result_brother = 1;

	if(node == NULL) return 1;

	if(node->parse_result != PARSERESULT_MATCH) {
		result_child = check_type(node->child);
		result_brother = check_type(node->brother);
		return (result_child && result_brother) ? 1 : 0;
	}

	/*printf("check_type l%2d %s\n", node->line_num, SYNTAXDIC[node->s_elem_it]);
	fflush(stdout);*/

	switch(node->s_elem_it){
	case SSUBPROGDEC:{
		/* SSUBPROGDEC
		 *  L>TPROCEDURE->SPROCEDURENAME->SSUBPROGDEC_2->TSEMI->SSUBPROGDEC_4->SCOMPSTAT->TSEMI
		 *                                 L> SFORMPARAM         L> SVARDEC
		 */
		if(!check_type(node->child->brother->brother->brother->brother->brother)){
			printf("\t in SUBPROGDEC\n");
			return 0;
		}
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
			printf("\t in if EXPR then \n");
			return 0;
		}
		type = (Type*)node_SEXPR->data;
		if(type->stdtype != TBOOLEAN || type->array_size != 0){
			printf("error : conditional sentence of if statement is not boolean\n");
			return 0;
		}
		return 1;
	}

	case SITERSTAT:{
		/* SITERSTAT
		 *  L> TWHILE -> (*)SEXPR -> TDO -> SSTAT
		 */
		SyntaxTreeNode* node_SEXPR = node->child->brother;
		Type* type;
		if(!check_type(node_SEXPR)){
			printf("\t in while EXPR do \n");
			return 0;
		}
		type = (Type*)node_SEXPR->data;
		if(type->stdtype != TBOOLEAN || type->array_size != 0){
			printf("error : conditional sentence of while statement is not boolean\n");
			return 0;
		}
		return 1;
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
		SyntaxTreeNode* node_SCALLSTAT_2 = node->child->brother->brother;
		SyntaxTreeNode* node_SEXPRS_1_0;
		ProcCallData* call_data = (ProcCallData*)node->data;
		ProcData* proc_data = call_data->proc_data;
		
		/* just 0 argument */
		if(node_SCALLSTAT_2->parse_result == PARSERESULT_EMPTY){
			VarData* param_data = proc_data->var_data_head;
			VarDecData* param_dec_data = param_data == NULL ? NULL : (VarDecData*)param_data->data;
			node_SEXPRS_1_0 = NULL;
			if(param_data != NULL && param_dec_data->is_param == 1){
				printf("error : line %d call statement has too few arguments\n", 
						call_data->line);
				return 0;
			}
			/*printf("NOerr : line %d type of argument of call statement is ok\n",
					call_data->line);*/
			return 1;
		}

		/* 1 or more arguments */
		else{
			SyntaxTreeNode* node_SEXPR = node_SCALLSTAT_2->child->child->brother->child;
			SyntaxTreeNode* node_SEXPRS_1 = node_SEXPR->brother;
			VarData* param_data;
			VarDecData* param_dec_data;

			/* try first argument */
			param_data = proc_data->var_data_head;
			if(param_data == NULL || ((VarDecData*)param_data->data)->is_param == 0){
				printf("error : line %d call statement has too many arguments\n", 
						call_data->line);
				return 0;
			}
			param_dec_data = (VarDecData*)param_data->data;
			if(!check_type(node_SEXPR)){
				printf("\t in EXPR\n");
				return 0;
			}
			if(param_dec_data->type.stdtype != ((Type*)node_SEXPR->data)->stdtype ||
					param_dec_data->type.array_size != ((Type*)node_SEXPR->data)->array_size){
				printf("error : line %d type of argument of call statement is wrong\n",
						call_data->line);
				return 0;
			}

			/*printf("call statement argument match %s\n", param_dec_data->name);*/

			if(node_SEXPRS_1->parse_result == PARSERESULT_EMPTY){
				/* just 1 argument */
				node_SEXPRS_1_0 = NULL;
			}
			else{
				/* 2 or more arguments */
				node_SEXPRS_1_0 = node_SEXPRS_1->child;
			}

			while(node_SEXPRS_1_0 != NULL && node_SEXPRS_1_0->parse_result == PARSERESULT_MATCH){
				param_data = param_data->next;
				param_dec_data = (VarDecData*)param_data->data;
				node_SEXPR = node_SEXPRS_1_0->child->brother;
				if(param_data == NULL || param_dec_data->is_param == 0){
					printf("error : line %d call statement has too few arguments\n", 
							call_data->line);
					return 0;
				}
				if(!check_type(node_SEXPR)){
					printf("\t in EXPR\n");
					return 0;
				}
				if(param_dec_data->type.stdtype != ((Type*)node_SEXPR->data)->stdtype ||
						param_dec_data->type.array_size != ((Type*)node_SEXPR->data)->array_size){
					printf("error : line %d type of argument of call statement is wrong\n",
							call_data->line);
					return 0;
				}

				/*printf("call statement argument match %s\n", param_dec_data->name);*/

				node_SEXPRS_1_0 = node_SEXPRS_1_0->brother;
			}

			/* no more argument but yes more param */
			param_data = param_data->next;
			if(param_data != NULL && ((VarDecData*)param_data->data)->is_param == 1){
				printf("error : line %d call statement has too few arguments\n", 
						call_data->line);
				return 0;
			}

			/*printf("NOerr : line %d type of argument of call statement is ok\n",
					call_data->line);*/
			return 1;
		}

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
				printf("\t in index of array %p\n", node_SEXPR);
				return 0;
			}
			node_SEXPR_type = (Type*)node_SEXPR->data;
			if(node_SEXPR_type->stdtype != TINTEGER || node_SEXPR_type->array_size != 0){
				printf("error : index of array is not an integer\n");
				return 0;
			}
			node->data = mem_alloc(sizeof(Type));
			node_SVAR_type = (Type*)node->data;
			node_SVAR_type->stdtype = var_dec_data->type.stdtype;
			node_SVAR_type->array_size = 0;
			return 1;
		}
		/* pattern 2 */
		else/* if(node->child->brother->child->parse_result == PARSERESULT_EMPTY) */{
			VarDecData* var_dec_data = 
				(VarDecData*)((VarData*)((VarRefData*)((VarData*)node->child->data)->data)->data)->data;
			Type* node_SVAR_type;

			node->data = mem_alloc(sizeof(Type));
			node_SVAR_type = (Type*)node->data;
			node_SVAR_type->stdtype = var_dec_data->type.stdtype;
			node_SVAR_type->array_size = var_dec_data->type.array_size;
			return 1;
		}
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

		node->data = mem_alloc(sizeof(Type));
		type = (Type*)node->data;

		if(!check_type(node_SSIMPLEEXPR)){
			printf("\t in SIMPLEEXPR\n");
			return 0;
		}
		type->stdtype = ((Type*)node_SSIMPLEEXPR->data)->stdtype;
		type->array_size = ((Type*)node_SSIMPLEEXPR->data)->array_size;

		while(node_SEXPR_1_0->parse_result != PARSERESULT_DIFFERENCE){
			SyntaxTreeNode* node_rel_op = node_SEXPR_1_0->child->child;
			Type* node_SSIMPLEEXPR_type;

			if(!check_type(node_SEXPR_1_0->child->brother)){
				printf("\t in EXPR_1_0\n");
				return 0;
			}
			while(node_rel_op->parse_result != PARSERESULT_MATCH){
				node_rel_op = node_rel_op->brother;
			}

			node_SSIMPLEEXPR_type = (Type*)node_SEXPR_1_0->child->brother->data;
			if(type->stdtype != node_SSIMPLEEXPR_type->stdtype ||
					 type->array_size != 0 || node_SSIMPLEEXPR_type->array_size != 0){
				printf("error : line %d operand type of operator \"%s\"\n",
						node->line_num,
						SYNTAXDIC[node_rel_op->s_elem_it]);
				/*printf("%d-%d %d-%d\n", type->stdtype, type->array_size,
					node_SSIMPLEEXPR_type->stdtype, node_SSIMPLEEXPR_type->array_size);*/
				return 0;
			}

			type->stdtype = TBOOLEAN;
			type->array_size = 0;

			node_SEXPR_1_0 = node_SEXPR_1_0->brother;
		}
		
		return 1;
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

		/*printf("check_type SIMPLEEXPR %p\n", node);*/

		node->data = mem_alloc(sizeof(Type));
		type = (Type*)node->data;

		if(!check_type(node_STERM)){
			printf("\t in TERM\n");
			fflush(stdout);
			return 0;
		}
		type->stdtype = ((Type*)node_STERM->data)->stdtype;
		type->array_size = ((Type*)node_STERM->data)->array_size;

		while(node_SSIMPLEEXPR_2_0->parse_result != PARSERESULT_DIFFERENCE){
			SyntaxTreeNode* node_add_op = node_SSIMPLEEXPR_2_0->child->child;
			int required_type = TINTEGER;
			Type* node_SFACTOR_type;

			if(!check_type(node_SSIMPLEEXPR_2_0->child->brother)){
				printf("\t in TERM_1_0\n");
				fflush(stdout);
				return 0;
			}
			while(node_add_op->parse_result != PARSERESULT_MATCH){
				node_add_op = node_add_op->brother;
			}
			if(node_add_op->s_elem_it == TOR){
				required_type = TBOOLEAN;
			}
			/*
			printf("koko\n");
			printf("%p %s\n", node_SSIMPLEEXPR_2_0, SYNTAXDIC[node_SSIMPLEEXPR_2_0->s_elem_it]);
			printf("%p %s\n", node_SSIMPLEEXPR_2_0->child,
						SYNTAXDIC[node_SSIMPLEEXPR_2_0->child->s_elem_it]);
			printf("%p %s\n", node_SSIMPLEEXPR_2_0->child->brother,
						SYNTAXDIC[node_SSIMPLEEXPR_2_0->child->brother->s_elem_it]);
			*/
			node_SFACTOR_type = (Type*)node_SSIMPLEEXPR_2_0->child->brother->data;

			if(type->stdtype != required_type || type->array_size != 0){
				printf("error : left operand type of operator \"%s\"\n",
						SYNTAXDIC[node_add_op->s_elem_it]);
				printf("%s\n", SYNTAXDIC[type->stdtype]);
				printf("%d\n", type->array_size);
				printf("%s\n", SYNTAXDIC[required_type]);
				return 0;
			}
			if(node_SFACTOR_type->stdtype != required_type ||
					node_SFACTOR_type->array_size != 0){
				printf("error : right operand type of operator \"%s\"\n",
						SYNTAXDIC[node_add_op->s_elem_it]);
				return 0;
			}

			node_SSIMPLEEXPR_2_0 = node_SSIMPLEEXPR_2_0->brother;

		}
		
		return 1;
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

		node->data = mem_alloc(sizeof(Type));
		type = (Type*)node->data;

		if(!check_type(node_SFACTOR)){
			printf("\t in FACTOR\n");
			return 0;
		}
		type->stdtype = ((Type*)node_SFACTOR->data)->stdtype;
		type->array_size = ((Type*)node_SFACTOR->data)->array_size;

		while(node_STERM_1_0->parse_result != PARSERESULT_DIFFERENCE){
			SyntaxTreeNode* node_mul_op = node_STERM_1_0->child->child;
			int required_type = TINTEGER;
			Type* node_SFACTOR_type;

			if(!check_type(node_STERM_1_0->child->brother)){
				printf("\t in FACTOR under TERM\n");
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
				node->data = mem_alloc(sizeof(Type));
				((Type*)node->data)->stdtype = ((ConstData*)child->data)->type.stdtype;
				((Type*)node->data)->array_size = ((ConstData*)child->data)->type.array_size;
			}
			else{
				if(!check_type(child)){
					printf("\t in %s\n", SYNTAXDIC[child->s_elem_it]);
					return 0;
				}
				node->data = mem_alloc(sizeof(Type));
				((Type*)node->data)->stdtype = ((Type*)child->data)->stdtype;
				((Type*)node->data)->array_size = ((Type*)child->data)->array_size;
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
			node->data = mem_alloc(sizeof(Type));
			((Type*)node->data)->stdtype = ((Type*)child->data)->stdtype;
			((Type*)node->data)->array_size = ((Type*)child->data)->array_size;
			return 1;
		}

		printf("\t in ( EXPR )\n");
		return 0;
	}

	case SFACTOR_3:{
		/* SFACTOR_3
		 *  L> TNOT -> SFACTOR
		 */
		SyntaxTreeNode* child = node->child->brother;
		if(check_type(child)){
			Type* type;
			node->data = mem_alloc(sizeof(Type));
			type = (Type*)node->data;
			type->stdtype = ((Type*)child->data)->stdtype;
			type->array_size = ((Type*)child->data)->array_size;
			if(type->stdtype == TBOOLEAN && type->array_size == 0){
				return 1;
			}
			printf("error : boolean value required after \"not\" token \n");
			return 0;

		}

		printf("\t in FACTOR after \"not\" token\n");
		return 0;
	}

	case SFACTOR_4:{
		/* SFACTOR_4
		 *  L> SSTDTYPE -> TLPAREN -> SEXPR -> TRPAREN
		 */
		SyntaxTreeNode* node_SEXPR = node->child->brother->brother;
		SyntaxTreeNode* node_Tstdtype = node->child->child;
		Type* node_SEXPR_type;
		Type* type;
		if(!check_type(node_SEXPR)){
			printf("\t in EXPR\n");
			return 0;
		}
		node_SEXPR_type = (Type*)node_SEXPR->data;

		node->data = mem_alloc(sizeof(Type));
		type = (Type*)node->data;

		if(node_SEXPR_type->array_size != 0){
			printf("error : line %d type conversion statement has array\n",
					node->line_num);
			return 0;
		}

		while(node_Tstdtype->parse_result != PARSERESULT_MATCH){
			node_Tstdtype = node_Tstdtype->brother;
		}

		type->stdtype = node_Tstdtype->s_elem_it;
		type->array_size = 0;

		return 1;

	}

	case SINSTAT_1_0:{
		/* SINSTAT_1_0
		 *  L> TLPAREN -> (*)SVAR -> SINSTAT_1_0_2 -> TRPAREN
		 *                            L> SINSTAT_1_0_2_0
		 *                                L> TCOMMA -> (*)SVAR
		 */
		SyntaxTreeNode* node_SVAR = node->child->brother;
		SyntaxTreeNode* node_SINSTAT_1_0_2_0 = node->child->brother->brother->child;
		Type* child_type;

		if(!check_type(node_SVAR)){
			printf("\t in VAR %p\n", node_SVAR);
			return 0;
		}
		child_type = (Type*)node_SVAR->data;
		if(child_type->stdtype == TBOOLEAN || child_type->array_size != 0){
			printf("error line %d augment type of read statement is wrong\n",
					node->child->line_num);
			return 0;
		}

		while(node_SINSTAT_1_0_2_0 != NULL &&
				node_SINSTAT_1_0_2_0->parse_result == PARSERESULT_MATCH){
			node_SVAR = node_SINSTAT_1_0_2_0->child->brother;
			if(!check_type(node_SVAR)){
				printf("\t in VAR\n");
				return 0;
			}
			child_type = (Type*)node_SVAR->data;
			if(child_type->stdtype == TBOOLEAN || child_type->array_size != 0){
				printf("error line %d augment type of read statement is wrong\n",
						node->child->line_num);
				return 0;
			}
			node_SINSTAT_1_0_2_0 = node_SINSTAT_1_0_2_0->brother;
		}
		
		return 1;
	}

	case SOUTFORM:{
		/* SOUTFORM
		 *  L> SOUTFORM_0 -> TSTRING
		 *      L> (*)SEXPR -> SOUTFORM_0_1
		 *                      L> SOUTFORM_0_1_0
		 *                          L> TCOLON -> TNUMBER
		 */
		SyntaxTreeNode* node_SEXPR;
		Type* node_SEXPR_type;

		if(node->child->parse_result != PARSERESULT_MATCH){
			/* SOUTFORM_0 is empty but TSTRING is matched */
			return 1;
		}
		node_SEXPR = node->child->child;

		if(!check_type(node_SEXPR)){
			printf("\t in EXPR\n");
			return 0;
		}

		node_SEXPR_type = (Type*) node_SEXPR->data;
		if(node_SEXPR_type->array_size != 0){
			printf("error line %d augment type of write statement is must not be an array\n",
					node->child->line_num);
			return 0;
		}

	}

	default:{
		result_child = check_type(node->child);
		result_brother = check_type(node->brother);
		break;
	}
	}

	return (result_child && result_brother) ? 1 : 0;

}