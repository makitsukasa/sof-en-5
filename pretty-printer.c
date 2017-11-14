#include "pretty-printer.h"

const char* SYNTAXDIC[NUMOFSYNTAX + 1] = {
	"", "NAME", "program", "var", "array", "of", "begin", "end", "if", "then",
	"else", "procedure", "return", "call", "while", "do", "not", "or", "div",
	"and", "char", "integer", "boolean", "readln", "writeln", "true", "false",
	"INTEGER", "STRING", "+", "-", "*", "=", "<>", "<", "<=", ">", ">=",
	"(", ")", "[", "]", ":=", ".", ",", ":", ";", "read", "write", "break",
	"SPROGRAM", "SBLOCK", "SBLOCK_0", "SBLOCK_0_0", "SVARDEC", "SVARDEC_5",
	"SVARDEC_5_1", "SVARNAMES", "SVARNAMES_1", "SVARNAMES_1_0", "SVARNAME",
	"STYPE", "SSTDTYPE", "SARRTYPE", "SSUBPROGDEC", "SSUBPROGDEC_2",
	"SSUBPROGDEC_4", "SPROCEDURENAME", "SFORMPARAM", "SFORMPARAM_4",
	"SFORMPARAM_4_0", "SCOMPSTAT", "SCOMPSTAT_2", "SCOMPSTAT_2_0", "SSTAT",
	"SCONDSTAT", "SCONDSTAT_4", "SCONDSTAT_4_0", "SITERSTAT", "SEXITSTAT",
	"SCALLSTAT", "SCALLSTAT_2", "SCALLSTAT_2_0", "SEXPRS", "SEXPRS_1",
	"SEXPRS_1_0", "SRETSTAT", "SASSIGNSTAT", "SLEFTPART", "SVAR", "SVAR_1",
	"SVAR_1_0", "SEXPR", "SEXPR_1", "SEXPR_1_0", "SSIMPLEEXPR", "SSIMPLEEXPR_0",
	"SSIMPLEEXPR_0_0", "SSIMPLEEXPR_2", "SSIMPLEEXPR_2_0", "STERM", "STERM_1",
	"STERM_1_0", "SFACTOR", "SFACTOR_2", "SFACTOR_3", "SFACTOR_4", "SCONST",
	"SMULOP", "SADDOP", "SRELATOP", "SINSTAT", "SINSTAT_0", "SINSTAT_1",
	"SINSTAT_1_0", "SINSTAT_1_0_2", "SINSTAT_1_0_2_0", "SOUTSTAT", "SOUTSTAT_0",
	"SOUTSTAT_1", "SOUTSTAT_1_0", "SOUTSTAT_1_0_2", "SOUTSTAT_1_0_2_0",
	"SOUTFORM", "SOUTFORM_0", "SOUTFORM_0_1", "SOUTFORM_0_1_0", "SEMPTYSTAT", 
};

int is_head_of_line;

SyntaxTreeNode* malloc_tree_node(){
	SyntaxTreeNode *node = malloc(sizeof(SyntaxTreeNode));
	if(node == NULL){
		printf("error i could not malloc\n");
		exit(-1);
	}
	node->brother = NULL;
	node->child = NULL;

	return node;
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

	printf("%9p ", node);
	printf("%16s ", SYNTAXDIC[node->s_elem_it]);
	printf("l%3d ", node->line_num);
	printf("d%d ", node->indent_depth);
	printf("w%d ", node->iter_depth);
	printf("%s ", node->parse_result == PARSERESULT_MATCH ? "T" : 
				node->parse_result == PARSERESULT_DIFFERENCE ? "F" :
				node->parse_result == PARSERESULT_EMPTY ? "E" : "A");
	printf("c%9p ", node->child);
	printf("b%9p ", node->brother);
	printf("%s ", node->string_attr);
	printf("\n");

	debug_tree(node->child);
	debug_tree(node->brother);
}

void dump_expected_token(int s_elem_it){
	int i;
	int max_it = 1;

	if(s_elem_array[s_elem_it].op == SELEMOP_ONE_OF){
		max_it = s_elem_array[s_elem_it].children_num;
	}

	for(i = 0; i < max_it; i++){
		int child_it = s_elem_array[s_elem_it].children[i];
		if(s_elem_array[child_it].op == SELEMOP_TERMINATOR){
			printf("%s,", SYNTAXDIC[child_it]);
		}else{
			dump_expected_token(child_it);
		}
	}	
}

int find_error_tree(SyntaxTreeNode* node){
	if(node == NULL) return PARSERESULT_EMPTY;

	if(node->parse_result == PARSERESULT_ACCIDENT){
		if(find_error_tree(node->child) == PARSERESULT_ACCIDENT){
			/*
			printf("%9p ", node);
			printf("%16s ", SYNTAXDIC[node->s_elem_it]);
			printf("l%d ", node->line_num);
			printf("d%d ", node->indent_depth);
			printf("w%d ", node->iter_depth);
			printf("child %9p ", node->child);
			printf("bro %9p ", node->brother);
			printf("%s ", node->string_attr);
			printf("'S CHILD");
			printf("\n");
			*/
			printf("\tin parsing %s (line %d)\n", SYNTAXDIC[node->s_elem_it], node->line_num);
			return PARSERESULT_ACCIDENT;
		}
	}

	if(find_error_tree(node->brother) == PARSERESULT_ACCIDENT){
		return PARSERESULT_ACCIDENT;
	}

	if(node->s_elem_it <= NUMOFTOKEN){
		if(node->parse_result == PARSERESULT_DIFFERENCE){
			/*
			printf("%9p ", node);
			printf("%16s ", SYNTAXDIC[node->s_elem_it]);
			printf("l%d ", node->line_num);
			printf("d%d ", node->indent_depth);
			printf("w%d ", node->iter_depth);
			printf("child %9p ", node->child);
			printf("bro %9p ", node->brother);
			printf("%s ", node->string_attr);
			printf("needs this token");
			printf("\n");
			*/
			printf("expected ' %s ' token (line %d)\n", SYNTAXDIC[node->s_elem_it], node->line_num);
			return PARSERESULT_ACCIDENT;
		}
		else if(node->parse_result == PARSERESULT_ACCIDENT){
			/*
			printf("%9p ", node);
			printf("%16s ", SYNTAXDIC[node->s_elem_it]);
			printf("l%d ", node->line_num);
			printf("d%d ", node->indent_depth);
			printf("w%d ", node->iter_depth);
			printf("child %9p ", node->child);
			printf("bro %9p ", node->brother);
			printf("%s ", node->string_attr);
			printf("this token is illegal");
			printf("\n");
			*/
			printf("unauthorized %s token (line %d)\n", SYNTAXDIC[node->s_elem_it], node->line_num);
			return PARSERESULT_ACCIDENT;
		}
	}

	else if(node->parse_result == PARSERESULT_EMPTY || node->parse_result == PARSERESULT_DIFFERENCE){
		/*
		printf("%9p ", node);
		printf("%16s ", SYNTAXDIC[node->s_elem_it]);
		printf("l%d ", node->line_num);
		printf("d%d ", node->indent_depth);
		printf("w%d ", node->iter_depth);
		printf("child %9p ", node->child);
		printf("bro %9p ", node->brother);
		printf("%s ", node->string_attr);
		printf("needs this token");
		printf("\n");
		*/
		printf("expected %s (line %d) ", SYNTAXDIC[node->s_elem_it], node->line_num);
		printf("(ex.");
		dump_expected_token(node->s_elem_it);
		printf(")\n");
		return PARSERESULT_ACCIDENT;
	}

	return PARSERESULT_EMPTY;
}

void print_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	if(node->s_elem_it == TPROGRAM) {
		printf("%s", node->string_attr);
		is_head_of_line = 0;
		print_tree(node->child);
		print_tree(node->brother);
		return;
	}

	if(node->parse_result != PARSERESULT_MATCH || node->s_elem_it > NUMOFTOKEN){
		print_tree(node->child);
		print_tree(node->brother);
		return;
	}

	if(node->s_elem_it == TEND || node->s_elem_it == TELSE){
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
	else if(node->s_elem_it == TSEMI || node->s_elem_it == TCOMMA ||
			node->s_elem_it == TDOT || node->s_elem_it == TLPAREN){
		/* not print " " */
	}
	else{
		printf(" ");
	}

	if(node->s_elem_it == TSTRING){
		printf("'%s'", node->string_attr);
	}
	else{
		printf("%s", node->string_attr);
	}

	if(node->s_elem_it == TSEMI || node->s_elem_it == TBEGIN || node->s_elem_it == TDO){
		is_head_of_line = 1;
	}

	print_tree(node->child);
	print_tree(node->brother);
}

int main(int nc, char *np[]) {
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

	node_SPROGRAM = parse(SPROGRAM, 0, 0);

	debug_tree(node_SPROGRAM);
	
	if(node_SPROGRAM->parse_result == PARSERESULT_MATCH){
		print_tree(node_SPROGRAM);
		printf("\n");
	}
	else{
		printf("syntax error found.\ndetail:\n");
		find_error_tree(node_SPROGRAM);
	}

	free_tree(node_SPROGRAM);
	end_scan();

	return 0;
}

