#include "pretty-printer.h"

const char* SYNTAXDIC[NUMOFSYNTAX + 1] = {
	"", "NAME", "program", "var", "array", "of", "begin", "end", "if", "then",
	"else", "procedure", "return", "call", "while", "do", "not", "or", "div",
	"and", "char", "integer", "boolean", "readln", "writeln", "true", "false",
	"INTEGER", "STRING", "+", "-", "*", "=", "<>", "<", "<=", ">", ">=",
	"(", ")", "[", "]", ":=", ".", ",", ":", ";", "read", "write", "break",
	"SPROGRAM", "SBLOCK", "SBLOCK_1", "SBLOCK_1_1", "SVARDEC", "SVARDEC_6",
	"SVARDEC_6_1", "SVARNAMES", "SVARNAMES_1", "SVARNAMES_1_1", "SVARNAME",
	"STYPE", "SSTDTYPE", "SARRTYPE", "SSUBPROGDEC", "SSUBPROGDEC_3",
	"SSUBPROGDEC_5", "SPROCEDURENAME", "SFORMPARAM", "SFORMPARAM_5",
	"SFORMPARAM_5_1", "SCOMPSTAT", "SCOMPSTAT_3", "SCOMPSTAT_3_1", "SSTAT",
	"SCONDSTAT", "SCONDSTAT_5", "SCONDSTAT_5_1", "SITERSTAT", "SEXITSTAT",
	"SCALLSTAT", "SCALLSTAT_3", "SCALLSTAT_3_1", "SEXPRS", "SEXPRS_2",
	"SEXPRS_2_1", "SRETSTAT", "SASSIGNSTAT", "SLEFTPART", "SVAR", "SVAR_2",
	"SVAR_2_1", "SEXPR", "SEXPR_2", "SEXPR_2_1", "SSIMPLEEXPR", "SSIMPLEEXPR_1",
	"SSIMPLEEXPR_1_1", "SSIMPLEEXPR_3", "SSIMPLEEXPR_3_1", "STERM", "STERM_2",
	"STERM_2_1", "SFACTOR", "SFACTOR_3", "SFACTOR_4", "SFACTOR_5", "SCONST",
	"SMULOP", "SADDOP", "SRELATOP", "SINSTAT", "SINSTAT_1", "SINSTAT_2",
	"SINSTAT_2_1", "SINSTAT_2_1_3", "SINSTAT_2_1_3_1", "SOUTSTAT", "SOUTSTAT_1",
	"SOUTSTAT_2", "SOUTSTAT_2_1", "SOUTSTAT_2_1_3", "SOUTSTAT_2_1_3_1",
	"SOUTFORM", "SOUTFORM_1", "SOUTFORM_1_2", "SOUTFORM_1_2_1", "SEMPTYSTAT", 
};

int is_head_of_line;

/*
malloc_tree_node(0, "", 0, 0, 0);
*/
SyntaxTreeNode* malloc_tree_node(){
	SyntaxTreeNode *p = malloc(sizeof(SyntaxTreeNode));
	if(p == NULL){
		printf("error i could not malloc\n");
		exit(-1);
	}
	p->brother = NULL;
	p->child = NULL;

	return p;
}

void free_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	free_tree(node->child);
	free_tree(node->brother);

	free(node);
	node = NULL;
}

void debug_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

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
	else if(node->sElemIt == TSEMI || node->sElemIt == TCOMMA ||
			node->sElemIt == TDOT || node->sElemIt == TLPAREN){
		/* not print " " */
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

int main(int nc, char *np[]) {
	if(nc < 2) {
		printf("File name id not given.\n");
		exit(-1);
	}

	if(init_scan(np[1]) < 0) {
		printf("File %s can not open.\n", np[1]);
		exit(-1);
	}

	init_parse();

	SyntaxTreeNode *node_SPROGRAM = parse(SPROGRAM, 0);

	debug_tree(node_SPROGRAM);

	if(node_SPROGRAM->parse_result == PARSERESULT_MATCH){
		print_tree(node_SPROGRAM);
		printf("\n");
	}

	free_tree(node_SPROGRAM);

	return 0;
}

