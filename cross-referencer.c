#include "cross-referencer.h"

void* mem_alloc(size_t size){
	void* mem;
	mem = calloc(1, size);
	if(mem == NULL){
		printf("error i could not malloc\n");
		exit(-1);
	}
	return mem;
}

CrossRefRecord* list_cross_referencer(SyntaxTreeNode* node, CrossRefRecord* record_tail){

	CrossRefRecord* new_record;
	CrossRefRecord* returned_record;

	if(node == NULL) return record_tail;

	if(node->parse_result != PARSERESULT_MATCH) {
		returned_record = list_cross_referencer(node->child, record_tail);
		returned_record = list_cross_referencer(node->brother, returned_record);
		return returned_record;
	}

	switch(node->s_elem_it){

	case SPROGRAM:{
		record_tail->is_head = 1;
		returned_record = list_cross_referencer(node->child, record_tail);
		returned_record = list_cross_referencer(node->brother, returned_record);
		return returned_record;
	}

	case SSUBPROGDEC:{
		char buf[MAXSTRSIZE];
		ProcData* proc_data = (ProcData*)node->data;
		VarData* param_data_loop;
		ProcCallData* call_data_loop;
		new_record = malloc(sizeof(CrossRefRecord));
		record_tail->next = new_record;
		record_tail = new_record;

		strcpy(new_record->namespace, "");

		strcpy(new_record->name, proc_data->name);

		strcpy(new_record->type, "procedure(");
		for(param_data_loop = proc_data->var_data_head;
				param_data_loop != NULL &&
				param_data_loop->is_declaration == 1 &&
				((VarDecData*)param_data_loop->data)->is_param == 1;
				param_data_loop = param_data_loop->next){
			if(((VarDecData*)param_data_loop->data)->type.array_size == 0){
				sprintf(new_record->type, "%s%s,",
						new_record->type,
						SYNTAXDIC[((VarDecData*)param_data_loop->data)->type.stdtype]);
			}
			else{
				sprintf(new_record->type, "%sarray[%d] of %s,",
						new_record->type,
						((VarDecData*)param_data_loop->data)->type.array_size,
						SYNTAXDIC[((VarDecData*)param_data_loop->data)->type.stdtype]);
			}
		}
		if(new_record->type[strlen(new_record->type) - 1] == '('){
			new_record->type[strlen(new_record->type) - 1] = '\0';
		}
		else{
			new_record->type[strlen(new_record->type) - 1] = ')';
		}

		sprintf(buf, "%d", proc_data->defined_line);
		strcpy(new_record->def, buf);

		for(call_data_loop = proc_data->ref_head;
				call_data_loop != NULL;
				call_data_loop = call_data_loop->next){
			sprintf(new_record->ref, "%s%d,", new_record->ref, call_data_loop->line);
		}
		if(strlen(new_record->ref) > 0)
			new_record->ref[strlen(new_record->ref) - 1] = '\0';

		returned_record = list_cross_referencer(node->child, new_record);
		returned_record = list_cross_referencer(node->brother, returned_record);
		return returned_record;
	}

	case SVARNAME:{
		char buf[MAXSTRSIZE];
		VarData* var_data = (VarData*)node->data;
		VarDecData* var_dec_data;
		VarData* var_data_loop;
		if(!var_data->is_declaration){
			returned_record = list_cross_referencer(node->child, record_tail);
			returned_record = list_cross_referencer(node->brother, returned_record);
			return returned_record;
		}

		new_record = malloc(sizeof(CrossRefRecord));
		record_tail->next = new_record;
		record_tail = new_record;

		var_dec_data = (VarDecData*)var_data->data;

		if(var_dec_data->namespace == NULL)
			strcpy(new_record->namespace, "");			
		else strcpy(new_record->namespace, var_dec_data->namespace->name);

		strcpy(new_record->name, var_dec_data->name);

		if(var_dec_data->type.array_size == 0){
			strcpy(new_record->type, SYNTAXDIC[var_dec_data->type.stdtype]);
		}
		else{
			sprintf(new_record->type, "array[%d] of %s", 
				var_dec_data->type.array_size,
				SYNTAXDIC[var_dec_data->type.stdtype]);	
		}

		sprintf(buf, "%d", var_dec_data->line);
		strcpy(new_record->def, buf);

		strcpy(buf, "");
		
		for(var_data_loop = var_dec_data->ref_head;
				var_data_loop != NULL;
				var_data_loop = var_data_loop->next){
			VarRefData* var_ref_data = (VarRefData*)var_data_loop->data;
			sprintf(new_record->ref, "%s%d,", new_record->ref, var_ref_data->line);
		}
		if(strlen(new_record->ref) > 0)
			new_record->ref[strlen(new_record->ref) - 1] = '\0';

		returned_record = list_cross_referencer(node->child, new_record);
		returned_record = list_cross_referencer(node->brother, returned_record);
		return returned_record;
	}

	case SCOMPSTAT:{
		/* no decleration between "begin" and "end" */
		/* COMPSTAT has many node */
		return record_tail;
	}

	default:{
		returned_record = list_cross_referencer(node->child, record_tail);
		returned_record = list_cross_referencer(node->brother, returned_record);
		return returned_record;
	}

	}

}

void print_cross_referencer(CrossRefRecord* record){
	if(record == NULL) return;
	if(record->is_head){
		printf("namespace       ");
		printf("name            ");
		printf("type                            ");
		printf("def     ");
		printf("ref");
		printf("\n");
		print_cross_referencer(record->next);
		return;
	}
	printf("%-16.15s", record->namespace);
	printf("%-16.15s", record->name);
	printf("%-32.31s", record->type);
	printf("%-8.7s", record->def);
	printf("%.100s", record->ref);
	printf("\n");
	print_cross_referencer(record->next);
}


void free_cross_referencer(CrossRefRecord* record){
	if(record == NULL) return;
	free_cross_referencer(record->next);
	free(record);
}

int main(int nc, char *np[]){
	SyntaxTreeNode *node_SPROGRAM;
	CrossRefRecord *record_head;

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
		/*debug_tree(node_SPROGRAM);
		debug_variable(node_SPROGRAM);*/
		free_tree(node_SPROGRAM);
		end_scan();
		return -1;
	}

	/*debug_tree(node_SPROGRAM);
	debug_variable(node_SPROGRAM);*/

	if(!check_type(node_SPROGRAM)){
		printf("error found.\n");
		free_tree(node_SPROGRAM);
		end_scan();
		return -1;
	}

	record_head = malloc(sizeof(CrossRefRecord));

	list_cross_referencer(node_SPROGRAM, record_head);

	print_cross_referencer(record_head);

	free_cross_referencer(record_head);


	free_tree(node_SPROGRAM);
	end_scan();


	return 0;
}