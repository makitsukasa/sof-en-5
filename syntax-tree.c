#include "pretty-printer.h"

void debug_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	printf("%20s ", SYNTAXDIC[node->syntaxElemIt]);
	printf("n%d ", node->is_head_of_line);
	printf("d%d ", node->indent_depth);
	printf("w%d ", node->iter_depth);
	printf("%s\n", node->parse_result ? "T" : "F");

	debug_tree(node->child);
	debug_tree(node->brother);
}

void print_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	if(node->parse_result == PARSERESULT_NOTMATCH) return;

	if(node->syntaxElemIt == TEND || node->syntaxElemIt == TELSE){
		node->is_head_of_line = 1;
	}

	if(node->is_head_of_line){
		int i;
		printf("\n");
		for(i = 0; i < node->indent_depth; i++){
			printf("\t");
		}
	}

	if(node->syntaxElemIt <= NUMOFTOKEN){
		printf("%s", node->string_attr);
	}

	if(node->syntaxElemIt == TSEMI || node->syntaxElemIt == TBEGIN){
		node->brother->is_head_of_line = 1;
	}
	else if(node->syntaxElemIt <= NUMOFTOKEN){
		printf(" ");
	}

	print_tree(node->child);
	print_tree(node->brother);
}

void free_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	free_tree(node->child);
	free_tree(node->brother);

	free(node);
}

/*
malloc_tree_node(0, "", 0, 0, 0);
*/
SyntaxTreeNode* malloc_tree_node(){
	SyntaxTreeNode *p = malloc(sizeof(SyntaxTreeNode));

	if(p == NULL){
		printf("error i could not malloc\n");
		exit(-1);
	}
/*
	p->syntaxElemIt = syntaxElemIt_;
	strcpy(p->string_attr, string_attr_);
	p->is_head_of_line = is_head_of_line_;
	p->indent_depth = indent_depth_;
	p->iter_depth = iter_depth_;
*/
	p->brother = NULL;
	p->child = NULL;

	return p;
}
