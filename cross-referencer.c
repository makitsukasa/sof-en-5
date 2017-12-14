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

	case SSUBPROGDEC:
		{
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
					printf("error : redefinition subprogram \"%s\"\n", 
							proc_data->name);
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
			printf("l%2d ref of %s\n",
					node->child->line_num, node->child->string_attr);

			switch(namespace->s_elem_it){
			case SSUBPROGDEC:
				var_data = ((ProcData*)namespace->data)->var_data_head;

				while(var_data != NULL){
					VarDecData* var_dec_data = var_data->data;
					if(strcmp(var_name, var_dec_data->name) == 0){
						VarRefData* var_ref_data;
						printf("namespace : procedure %s\n", ((ProcData*)namespace->data)->name);
						node->data = malloc(sizeof(VarData));
						((VarData*)node->data)->is_declaration = 0;
						((VarData*)node->data)->data = malloc(sizeof(VarRefData));
						var_ref_data = ((VarData*)node->data)->data;
						var_ref_data->line = node->child->line_num;
						var_ref_data->var_dec = var_data;
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
						printf("namespace : global\n");
						node->data = malloc(sizeof(VarData));
						((VarData*)node->data)->is_declaration = 0;
						((VarData*)node->data)->data = malloc(sizeof(VarRefData));
						var_ref_data = ((VarData*)node->data)->data;
						var_ref_data->line = node->child->line_num;
						var_ref_data->var_dec = var_data;

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
		break;

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
		if(node->s_elem_it == SSUBPROGDEC){
			printf("procedu ");
			printf("l%2d ", ((ProcData*)node->data)->defined_line);
			printf("dp%9p ", node->data);
			printf("vh%9p ", ((ProcData*)node->data)->var_data_head);
			printf("np%9p ", ((ProcData*)node->data)->next);
			printf("%s ", ((ProcData*)node->data)->name);
			printf("\n");
		}
		if(node->s_elem_it == SVARNAME){
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
				printf("var ref ");
				printf("l%2d ", var_ref_data->line);
				printf(" p%9p ", var_ref_data->var_dec);
				printf("\n");
			}
		}
	}
	print_variable(node->child);
	print_variable(node->brother);
}

Type check_type(SyntaxTreeNode* node){
	Type type;
	return type;
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

	/*debug_tree(node_SPROGRAM);*/

	if(!fill_var_data(node_SPROGRAM, NULL, NULL)){
		free_tree(node_SPROGRAM);
		end_scan();
		return -1;
	}

	print_variable(node_SPROGRAM);

/*
	if(check_type(node_SPROGRAM) == 0){
		printf("syntax error found.\ndetail:\n");

	}
*/


	free_tree(node_SPROGRAM);
	end_scan();

	return 0;
}