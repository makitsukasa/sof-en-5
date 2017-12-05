#include "cross-referencer.h"


void list_variable(SyntaxTreeNode* node, SyntaxTreeNode* namespace){

	if(node == NULL) return;

	if(node->parse_result != PARSERESULT_MATCH) {
		list_variable(node->child, namespace);
		list_variable(node->brother, namespace);
		return;
	}

	switch(node->s_elem_it){
	case SPROGRAM:
		node->data = calloc(1, sizeof(ProcData));

		namespace = node;
		strcpy(((ProcData*)node->data)->name, node->child->brother->string_attr);

		list_variable(node->child, namespace);
		list_variable(node->brother, namespace);
		break;

	case SSUBPROGDEC:
		node->data = calloc(1, sizeof(ProcData));

		strcpy(((ProcData*)node->data)->name, node->child->brother->child->string_attr);
		namespace = node;

		list_variable(node->child, namespace);
		list_variable(node->brother, namespace);
		break;

	case SVARDEC:
	case SVARDEC_5_0:
	case SFORMPARAM:
	case SFORMPARAM_4_0:
		{
			SyntaxTreeNode* node_SVARNAME;
			SyntaxTreeNode* node_SVARNAMES_1_0;
			if(node->s_elem_it == SVARDEC || node->s_elem_it == SFORMPARAM){
				node_SVARNAME = node->child->brother->child;
			}
			else{
				node_SVARNAME = node->child->child;
			}
			if(node->s_elem_it == SVARDEC || node->s_elem_it == SFORMPARAM){
				node_SVARNAMES_1_0 = node->child->brother->child->brother->child;
			}
			else{
				node_SVARNAMES_1_0 = node->child->child->brother->child;
			}
			node_SVARNAME->data = calloc(1, sizeof(VarData));
			strcpy(((VarData*)node_SVARNAME->data)->name,
					node_SVARNAME->child->string_attr);
			if(node->s_elem_it == SFORMPARAM || node->s_elem_it == SFORMPARAM_4_0){
				((VarData*)node_SVARNAME->data)->is_param = 1;
			}

			for(; node_SVARNAMES_1_0 != NULL && node_SVARNAMES_1_0->parse_result != PARSERESULT_DIFFERENCE;){
				node_SVARNAMES_1_0->child->brother->data = calloc(1, sizeof(VarData));
				strcpy(((VarData*)node_SVARNAMES_1_0->child->brother->data)->name,
						node_SVARNAMES_1_0->child->brother->child->string_attr);
				node_SVARNAMES_1_0 = node_SVARNAMES_1_0->brother;
			}

			if(((ProcData*)namespace->data)->var_data_tail == NULL){
				((ProcData*)namespace->data)->var_data_tail = node->data;	
			}
			else{
				((ProcData*)namespace->data)->var_data_tail->next = node->data;
				((ProcData*)namespace->data)->var_data_tail = node->data;
			}

			list_variable(node->child, namespace);
			break;
		}

	default:
		list_variable(node->child, namespace);
		list_variable(node->brother, namespace);

	}
	
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
			printf("prog  ");
			printf("l%d ", ((ProcData*)node->data)->defined_line);
			printf("vh%9p ", ((ProcData*)node->data)->var_data_head);
			printf("np%9p ", ((ProcData*)node->data)->next);
			printf("%s ", ((ProcData*)node->data)->name);
			printf("\n");
		}
		if(node->s_elem_it == SSUBPROGDEC){
			printf("proc  ");
			printf("l%d ", ((ProcData*)node->data)->defined_line);
			printf("vh%9p ", ((ProcData*)node->data)->var_data_head);
			printf("np%9p ", ((ProcData*)node->data)->next);
			printf("%s ", ((ProcData*)node->data)->name);
			printf("\n");
		}
		if(node->s_elem_it == SVARNAME){
			printf("t%d ", ((VarData*)node->data)->type.stdtype);
			printf("p%d ", ((VarData*)node->data)->is_param);
			printf("l%d ", ((VarData*)node->data)->defined_line);
			printf("vh%9p ", ((VarData*)node->data)->referenced_line_head);
			printf("np%9p ", ((VarData*)node->data)->next);
			printf("%s ", ((VarData*)node->data)->name);
			printf("\n");
		}
	}
	print_variable(node->child);
	print_variable(node->brother);
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

	list_variable(node_SPROGRAM, NULL);

	/*debug_tree(node_SPROGRAM);*/

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