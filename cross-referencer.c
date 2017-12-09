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
			/* nobody can read this */
			/* but i know what it means and i know it works */
			SyntaxTreeNode* node_SVARNAME;
			SyntaxTreeNode* node_SVARNAMES_1_0;
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
			strcpy(((VarData*)node_SVARNAME->data)->name,
					node_SVARNAME->child->string_attr);

			((VarData*)node_SVARNAME->data)->is_param = is_param;
			((VarData*)node_SVARNAME->data)->defined_line = node_SVARNAME->line_num;
			((VarData*)node_SVARNAME->data)->type.stdtype = ((Type*) type)->stdtype;
			((VarData*)node_SVARNAME->data)->type.array_size = ((Type*) type)->array_size;

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
				strcpy(((VarData*)node_SVARNAME->data)->name,
						node_SVARNAMES_1_0->child->brother->child->string_attr);

				((VarData*)node_SVARNAME->data)->is_param = is_param;
				((VarData*)node_SVARNAME->data)->defined_line = node_SVARNAME->line_num;
				((VarData*)node_SVARNAME->data)->type.stdtype = ((Type*) type)->stdtype;
				((VarData*)node_SVARNAME->data)->type.array_size = ((Type*) type)->array_size;
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

int list_variable(SyntaxTreeNode* node, SyntaxTreeNode* namespace){
	int result_child = 1;
	int result_brother = 1;

	if(node == NULL) return 1;

	if(node->parse_result != PARSERESULT_MATCH) {
		result_child = list_variable(node->child, namespace);
		result_brother = list_variable(node->brother, namespace);
		return (result_child && result_brother) ? 1 : 0;
	}

	switch(node->s_elem_it){
	case SPROGRAM:

		namespace = node;
		result_child = list_variable(node->child, namespace);
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
			result_child = list_variable(node->child, namespace);
			result_brother = list_variable(node->brother, namespace);
			namespace = prev_namespace;
			break;
		}

	case SVARNAME:
		{
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
				if(strcmp(((VarData*)node->data)->name, var_data->name) == 0){
					printf("error : redefinition variable \"%s\"\n", 
							var_data->name);
					return 0;
				}
				var_data = var_data->next;
			}

			result_brother = list_variable(node->brother, namespace);
			break;

		}

	default:
		result_child = list_variable(node->child, namespace);
		result_brother = list_variable(node->brother, namespace);
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
			char type[][5] = {"char", "int ", "bool"};
			printf("%s%d ", type[((VarData*)node->data)->type.stdtype - TCHAR],
								((VarData*)node->data)->type.array_size);
			printf("%s ", ((VarData*)node->data)->is_param ? "p" : "-");
			printf("l%2d ", ((VarData*)node->data)->defined_line);
			printf("dp%9p ", node->data);
			printf("lh%9p ", ((VarData*)node->data)->referenced_line_head);
			printf("np%9p ", ((VarData*)node->data)->next);
			printf("%s ", ((VarData*)node->data)->name);
			printf("\n");
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

	if(!list_variable(node_SPROGRAM, NULL)){
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