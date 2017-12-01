#include "cross-referencer.h"

void add_var_data(VarData* vdata, SyntaxTreeNode* namespace){

}

void list_variable(SyntaxTreeNode* node, SyntaxTreeNode* namespace){

	if(node->s_elem_it == SPROGRAM){
		namespace = node;
	}

	if(node->s_elem_it == SVARDEC){
		VarData* vdata = malloc(sizeof(VarData));

		node->data = (void*) vdata;
		namespace->data->var_data_tail->next = vdata;
		namespace->data->var_data_tail = vdata;
	}

	if(node->s_elem_it == SVARDEC){
		/* var name is type*/


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
/*
	if(check_type(node_SPROGRAM) == 0){
		printf("syntax error found.\ndetail:\n");

	}
*/


	free_tree(node_SPROGRAM);
	end_scan();

	return 0;
}