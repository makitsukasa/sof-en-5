#include "pretty-printer.h"

const char* SYNTAXDIC[NUMOFSYNTAX + 1] = {
	"", "NAME", "program", "var", "array", "of", "begin", "end", "if", "then",
	"else", "procedure", "return", "call", "while", "do", "not", "or", "div",
	"and", "char", "integer", "boolean", "readln", "writeln", "true", "false",
	"INTEGER", "STRING", "+", "-", "*", "=", "<>", "<", "<=", ">", ">=",
	"(", ")", "[", "]", ":=", ".", ",", ":", ";", "read", "write", "break",
	"SPROGRAM", "SBLOCK", "SBLOCK_0", "SBLOCK_0_0", "SVARDEC", "SVARDEC_5",
	"SVARDEC_5_0", "SVARNAMES", "SVARNAMES_1", "SVARNAMES_1_0", "SVARNAME",
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
int line_num_pretty_printed;

SyntaxTreeNode* malloc_tree_node(){
	SyntaxTreeNode *node = calloc(1, sizeof(SyntaxTreeNode));
	if(node == NULL){
		printf("error i could not malloc\n");
		exit(-1);
	}

	return node;
}

void free_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	if(node->data != NULL){
		if(node->s_elem_it == SVARNAME && ((VarData*)node->data)->data != NULL){
			free(((VarData*)node->data)->data);
		}

		free(node->data);
	}

	free_tree(node->child);
	free_tree(node->brother);

	free(node);
}

#if 1
void print_tree_node(SyntaxTreeNode* node){
	printf("%9p ", node);
	printf("%16s ", SYNTAXDIC[node->s_elem_it]);
	printf("l%3d ", node->line_num);
	printf("rl%3d ", node->line_num_pretty_printed);
	printf("d%d ", node->indent_depth);
	printf("w%d ", node->iter_depth);
	printf("%s ", node->parse_result == PARSERESULT_MATCH ? "T" : 
				node->parse_result == PARSERESULT_DIFFERENCE ? "F" :
				node->parse_result == PARSERESULT_EMPTY ? "E" : "A");
	printf("c%9p ", node->child);
	printf("b%9p ", node->brother);
	printf("%s ", node->string_attr);
	printf("\n");
	if(node->data != NULL){
		printf("         %9p ", node->data);
		if(node->s_elem_it == SPROGRAM || node->s_elem_it == SSUBPROGDEC){

		}
		else if(node->s_elem_it == SVARDEC){

		}
		printf("\n");	
	}
}

void debug_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	if(0 && node->parse_result == PARSERESULT_DIFFERENCE){
		debug_tree(node->child);
		debug_tree(node->brother);
		return;
	}

	print_tree_node(node);

	debug_tree(node->child);
	debug_tree(node->brother);
}
#endif

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
			print_tree_node(this);
			printf("'S CHILD\n");
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
			print_tree_node(this);
			printf("needs this token\n");
			*/
			printf("expected ' %s ' token (line %d)\n", SYNTAXDIC[node->s_elem_it], node->line_num);
			return PARSERESULT_ACCIDENT;
		}
		else if(node->parse_result == PARSERESULT_ACCIDENT){
			/*
			print_tree_node(this);
			printf("this token is illegal\n");
			*/
			printf("unauthorized %s token (line %d)\n", SYNTAXDIC[node->s_elem_it], node->line_num);
			return PARSERESULT_ACCIDENT;
		}
	}

	else if(node->parse_result == PARSERESULT_EMPTY || node->parse_result == PARSERESULT_DIFFERENCE){
		/*
		print_tree_node(this);
		printf("needs this token\n");
		*/
		printf("expected %s (line %d) ", SYNTAXDIC[node->s_elem_it], node->line_num);
		printf("(ex.");
		dump_expected_token(node->s_elem_it);
		printf(")\n");
		return PARSERESULT_ACCIDENT;
	}

	printf("this statement will run when parse is completed with no error and execute find_error_tree\n" );
	return PARSERESULT_EMPTY;
}

#if 0
int indent_difference(int grampa_s_elem_it, int parent_s_elem_it, int child_s_elem_it){

	if(grampa_s_elem_it == SITERSTAT &&
		parent_s_elem_it == SSTAT && child_s_elem_it == SCOMPSTAT){
		return -1;
	}
	if(grampa_s_elem_it == SCONDSTAT &&
		parent_s_elem_it == SSTAT && child_s_elem_it == SCOMPSTAT){
		return -1;
	}
	if(grampa_s_elem_it == SCONDSTAT_4_0 &&
		parent_s_elem_it == SSTAT && child_s_elem_it == SCOMPSTAT){
		return -1;
	}

	if(parent_s_elem_it == SBLOCK && child_s_elem_it == SBLOCK_0)
		return 1;
	if(parent_s_elem_it == SSUBPROGDEC && child_s_elem_it == SSUBPROGDEC_4)
		return 1;
	if(parent_s_elem_it == SITERSTAT && child_s_elem_it == SSTAT)
		return 1;
	if(parent_s_elem_it == SCONDSTAT && child_s_elem_it == SSTAT)
		return 1;
	if(parent_s_elem_it == SCONDSTAT_4_0 && child_s_elem_it == SSTAT)
		return 1;
	if(parent_s_elem_it == SCOMPSTAT && child_s_elem_it == SSTAT)
		return 1;
	if(parent_s_elem_it == SCOMPSTAT && child_s_elem_it == SCOMPSTAT_2)
		return 1;
	if(parent_s_elem_it == SVARDEC && child_s_elem_it != TVAR)
		return 1;

	return 0;
}

void preformat_tree(SyntaxTreeNode* parent, SyntaxTreeNode* this, int indent_depth){
	int i;
	int g_it = (parent == NULL) ? 0 : parent->s_elem_it;
	int p_it = (this   == NULL) ? 0 : this  ->s_elem_it;
	SyntaxTreeNode* node;
	if(this == NULL) return;

	this->indent_depth = indent_depth;

	node = this->child;
	for(i = 0; node != NULL; i++){
		int indent = indent_depth + indent_difference(g_it, p_it, node->s_elem_it);
		preformat_tree(this, node, indent);
		node = node->brother;
	}
}
#endif

#if 0
void decide_printed_line_num_tree(SyntaxTreeNode* node){
	if(node == NULL) return;

	if(node->s_elem_it == TPROGRAM) {
		is_head_of_line = 0;
		line_num_pretty_printed = 1;
		node->line_num_pretty_printed = line_num_pretty_printed;
		decide_printed_line_num_tree(node->child);
		decide_printed_line_num_tree(node->brother);
		return;
	}

	if(node->parse_result != PARSERESULT_MATCH || node->s_elem_it > NUMOFTOKEN){
		node->line_num_pretty_printed = line_num_pretty_printed;
		decide_printed_line_num_tree(node->child);
		decide_printed_line_num_tree(node->brother);
		return;
	}

	if(node->s_elem_it == TEND || node->s_elem_it == TELSE){
		is_head_of_line = 1;
	}

	if(is_head_of_line){
		line_num_pretty_printed++;
		is_head_of_line = 0;
	}

	node->line_num_pretty_printed = line_num_pretty_printed;

	if(node->s_elem_it == TSEMI || node->s_elem_it == TBEGIN ||
		node->s_elem_it == TDO || node->s_elem_it == TTHEN || 
		node->s_elem_it == TELSE || node->s_elem_it == TVAR){
		is_head_of_line = 1;
	}

	decide_printed_line_num_tree(node->child);
	decide_printed_line_num_tree(node->brother);
}
#endif

#if 0
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
			node->s_elem_it == TDOT || 
			(node->s_elem_it == TLPAREN && node->parent->s_elem_it != SFACTOR_2)){
		/* not print " " before print token */
	}
	else{
		/* print " " before print token */
		printf(" ");
	}

	if(node->s_elem_it == TSTRING){
		printf("'%s'", node->string_attr);
	}
	else{
		printf("%s", node->string_attr);
	}

	if(node->s_elem_it == TSEMI || node->s_elem_it == TBEGIN ||
		node->s_elem_it == TDO || node->s_elem_it == TTHEN || 
		node->s_elem_it == TELSE || node->s_elem_it == TVAR){
		is_head_of_line = 1;
	}

	print_tree(node->child);
	print_tree(node->brother);
}

int pretty_printer_main(int nc, char *np[]) {
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

	if(node_SPROGRAM->parse_result == PARSERESULT_MATCH){
		preformat_tree(NULL, node_SPROGRAM, 0);
		print_tree(node_SPROGRAM);
		/*printf("\n");*/
		/*debug_tree(node_SPROGRAM);*/
	}
	else{
		/*debug_tree(node_SPROGRAM);*/
		printf("syntax error found.\ndetail:\n");
		find_error_tree(node_SPROGRAM);
	}

	free_tree(node_SPROGRAM);
	end_scan();

	return 0;
}

#endif
