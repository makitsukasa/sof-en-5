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
		node->data = calloc(1, sizeof(ProgData));

		((ProgData*)node->data)->defined_line = node->line_num;
		namespace = node;
		strcpy(((ProgData*)node->data)->name, node->child->brother->string_attr);

		list_variable(node->child, namespace);
		list_variable(node->brother, namespace);
		break;

	case SSUBPROGDEC:
		node->data = calloc(1, sizeof(ProcData));
		
		((ProcData*)node->data)->defined_line = node->line_num;
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
			/*  */
			SyntaxTreeNode* node_SVARNAME;
			SyntaxTreeNode* node_SVARNAMES_1_0;
			void *type;
			int is_param = (node->s_elem_it == SFORMPARAM ||
							 node->s_elem_it == SFORMPARAM_4_0) ? 1 : 0;

			if(node->s_elem_it == SVARDEC_5_0){
				list_variable(node->child->brother->brother, namespace);
				type = node->child->brother->brother->data;
			}
			else{
				list_variable(node->child->brother->brother->brother, namespace);
				type = node->child->brother->brother->brother->data;
			}

			if(node->s_elem_it == SVARDEC || node->s_elem_it == SFORMPARAM){
				node_SVARNAME = node->child->brother->child;
			}
			else{
				node_SVARNAME = node->child->child;
			}

			node_SVARNAME->data = calloc(1, sizeof(VarData));
			strcpy(((VarData*)node_SVARNAME->data)->name,
					node_SVARNAME->child->string_attr);

			((VarData*)node_SVARNAME->data)->is_param = is_param;
			((VarData*)node_SVARNAME->data)->defined_line = node_SVARNAME->line_num;
			((VarData*)node_SVARNAME->data)->type.stdtype = ((Type*) type)->stdtype;
			((VarData*)node_SVARNAME->data)->type.array_size = ((Type*) type)->array_size;

			if(node->s_elem_it == SVARDEC || node->s_elem_it == SFORMPARAM){
				node_SVARNAMES_1_0 = node->child->brother->child->brother->child;
			}
			else{
				node_SVARNAMES_1_0 = node->child->child->brother->child;
			}

			for(; (node_SVARNAMES_1_0 != NULL &&
					node_SVARNAMES_1_0->parse_result == PARSERESULT_MATCH);
					node_SVARNAMES_1_0 = node_SVARNAMES_1_0->brother){
				SyntaxTreeNode *node_SVARNAME = node_SVARNAMES_1_0->child->brother;
				node_SVARNAME->data = calloc(1, sizeof(VarData));
				strcpy(((VarData*)node_SVARNAME->data)->name,
						node_SVARNAMES_1_0->child->brother->child->string_attr);

				((VarData*)node_SVARNAME->data)->is_param = is_param;
				((VarData*)node_SVARNAME->data)->defined_line = node_SVARNAME->line_num;
				((VarData*)node_SVARNAME->data)->type.stdtype = ((Type*) type)->stdtype;
				((VarData*)node_SVARNAME->data)->type.array_size = ((Type*) type)->array_size;
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

	case STYPE:
		{
			Type *type = malloc(sizeof(Type));
			SyntaxTreeNode *child = node->child;
			SyntaxTreeNode *node_stdtype;
			while(child->parse_result != PARSERESULT_MATCH){
				child = child->brother;
			}
			if(child->s_elem_it == SSTDTYPE){
				node_stdtype = child->child;
				while(node_stdtype->parse_result != PARSERESULT_MATCH){
					node_stdtype = node_stdtype->brother;
				}
				type->stdtype = node_stdtype->s_elem_it;
				type->array_size = 0;
			}
			else /* if(node->child->s_elem_it == SARRAYTYPE) */{
				node_stdtype = child->child->brother->brother->
								brother->brother->brother->child;
				while(node_stdtype->parse_result != PARSERESULT_MATCH){
					node_stdtype = node_stdtype->brother;
				}
				type->stdtype = node_stdtype->s_elem_it;
				type->array_size = atoi(child->child->brother->brother->string_attr);
			}
			node->data = (void*) type;

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
			printf("progr   ");
			printf("l%d ", ((ProcData*)node->data)->defined_line);
			printf("vh%9p ", ((ProcData*)node->data)->var_data_head);
			printf("np%9p ", ((ProcData*)node->data)->next);
			printf("%s ", ((ProcData*)node->data)->name);
			printf("\n");
		}
		if(node->s_elem_it == SSUBPROGDEC){
			printf("proce   ");
			printf("l%d ", ((ProcData*)node->data)->defined_line);
			printf("vh%9p ", ((ProcData*)node->data)->var_data_head);
			printf("np%9p ", ((ProcData*)node->data)->next);
			printf("%s ", ((ProcData*)node->data)->name);
			printf("\n");
		}
		if(node->s_elem_it == SVARNAME){
			char type[][5] = {"char", "int ", "bool"};
			printf("%s%d ", type[((VarData*)node->data)->type.stdtype - TCHAR],
								((VarData*)node->data)->type.array_size);
			printf("%s ", ((VarData*)node->data)->is_param ? "p" : "_");
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