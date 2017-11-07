#include "pretty-printer.h"

void print_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	if(node->syntaxElemIt == TEND){
		node->is_head_of_line = 1;
	}

	if(node->is_head_of_line){
		int i;
		for(i = 0; i < node->indent_depth; i++){
			printf("\t");
		}
	}

	if(node->syntaxElemIt <= NUMOFTOKEN){
		printf("%s", node->string_attr);
	}

	if(node->syntaxElemIt == TSEMI || node->syntaxElemIt == TBEGIN){
		printf("\n");
	}
	else{
		printf(" ");
	}

	print_tree(node->child);
	print_tree(node->next);
}

void free_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	free_tree(node->child);
	free_tree(node->next);

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
	p->next = NULL;
	p->child = NULL;

	return p;
}
