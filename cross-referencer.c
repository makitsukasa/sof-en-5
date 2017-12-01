#include "cross-referencer.h"


void list_variable(SyntaxTreeNode* node, SyntaxTreeNode* namespace){

	if(node == NULL) return;

	switch(node->s_elem_it){
	case SPROGRAM:
	case SSUBPROGDEC:
		namespace = node;
		node->data = malloc(sizeof(ProcData));
		list_variable(node->child, namespace);
		list_variable(node->brother, namespace);
		break;

	case SVARDEC:

	case SVARNAMES:
		else{
			SyntaxTreeNode* node_SVARNAMES_1_0 = node->child->brother->child;
			node->child->data = malloc(sizeof(VarData));
			for(; node_SVARNAMES_1_0 != NULL && node_SVARNAMES_1_0->parse_result != PARSERESULT_DIFFERENCE;){
				node_SVARNAMES_1_0->child->brother->data = malloc(sizeof(VarData));
				node_SVARNAMES_1_0 = node_SVARNAMES_1_0->brother;
			}
			if(((ProcData*)namespace->data)->var_data_tail == NULL){
				((ProcData*)namespace->data)->var_data_tail = node->data;	
			}
			else{
				((ProcData*)namespace->data)->var_data_tail->next = node->data;
				((ProcData*)namespace->data)->var_data_tail = node->data;
			}
			list_variable(node->brother, namespace);
			break;
		}

	default:
		list_variable(node->child, namespace);
		list_variable(node->brother, namespace);

	}
	
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

	debug_tree(node_SPROGRAM);
/*
	if(check_type(node_SPROGRAM) == 0){
		printf("syntax error found.\ndetail:\n");

	}
*/


	free_tree(node_SPROGRAM);
	end_scan();

	return 0;
}