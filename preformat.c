#include "pretty-printer.h"

int is_head_of_line;

void debug_tree(SyntaxTreeNode* node){
	if(node == NULL) return;
	/*if(node->parse_result != PARSERESULT_MATCH) return;*/

	printf("%16s ", SYNTAXDIC[node->sElemIt]);
	printf("d%d ", node->indent_depth);
	printf("w%d ", node->iter_depth);
	printf("%s ", node->parse_result == PARSERESULT_MATCH ? "T" : 
				node->parse_result == PARSERESULT_NOTMATCH ? "F" : "E");
	printf("%s ", node->string_attr);
	printf("\n");

	debug_tree(node->child);
	debug_tree(node->brother);
}

void print_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	if(node->sElemIt == TPROGRAM) {
		printf("%s", node->string_attr);
		is_head_of_line = 0;
		print_tree(node->child);
		print_tree(node->brother);
		return;
	}

	if(node->parse_result != PARSERESULT_MATCH || node->sElemIt > NUMOFTOKEN){
		print_tree(node->child);
		print_tree(node->brother);
		return;
	}

	if(node->sElemIt == TEND || node->sElemIt == TELSE){
		is_head_of_line = 1;
	}

	if(is_head_of_line){
		int i;
		printf("\n");
		for(i = 0; i < node->indent_depth; i++){
			printf("\t");
		}
		is_head_of_line = 0;
	}
	else{
		printf(" ");
	}

	if(node->sElemIt == TSTRING){
		printf("'%s'", node->string_attr);
	}
	else{
		printf("%s", node->string_attr);
	}

	if(node->sElemIt == TSEMI || node->sElemIt == TBEGIN || node->sElemIt == TDO){
		is_head_of_line = 1;
	}

	print_tree(node->child);
	print_tree(node->brother);
}


