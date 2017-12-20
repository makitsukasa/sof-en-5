#include "cross-referencer.h"

int print_cross_referencer(SyntaxTreeNode* node){
	
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

	decide_printed_line_num_tree(node_SPROGRAM);

	fill_node_data_prepare(node_SPROGRAM);

	if(!fill_node_data(node_SPROGRAM, NULL, NULL)){
		debug_tree(node_SPROGRAM);

		debug_variable(node_SPROGRAM);
		free_tree(node_SPROGRAM);
		end_scan();
		return -1;
	}

	debug_tree(node_SPROGRAM);

	debug_variable(node_SPROGRAM);

	if(!check_type(node_SPROGRAM)){
		printf("error found.\n");
		free_tree(node_SPROGRAM);
		end_scan();
		return -1;
	}

	free_tree(node_SPROGRAM);
	end_scan();

	return 0;
}