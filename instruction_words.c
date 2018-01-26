#include "compiler.h"

void iw_LD		(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tLD\t\t%s,%s\n", label, r1, r2);
}
void iw_LD_3	(char* label, char* r , char* adr, char* x ){
	fprintf(output_file, "%s\tLD\t\t%s,%s,%s\n", label, r, adr, x);
}
void iw_ST		(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tST\t\t%s,%s\n", label, r1, r2);
}
void iw_ST_3	(char* label, char* r , char* adr, char* x ){
	fprintf(output_file, "%s\tST\t\t%s,%s,%s\n", label, r, adr, x);
}
void iw_LAD		(char* label, char* r , char* adr){
	fprintf(output_file, "%s\tLAD\t\t%s,%s\n", label, r, adr);
}
void iw_LAD_3	(char* label, char* r , char* adr, char* x){
	fprintf(output_file, "%s\tLAD\t\t%s,%s,%s\n", label, r, adr, x);
}

void iw_ADDA	(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tADDA\t%s,%s\n", label, r1, r2);
	iw_JOV("", "EOVF");
}
void iw_ADDL	(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tADDL\t%s,%s\n", label, r1, r2);
	iw_JOV("", "EOVF");
}
void iw_SUBA	(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tSUBA\t%s,%s\n", label, r1, r2);
	iw_JOV("", "EOVF");
}
void iw_SUBL	(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tSUBL\t%s,%s\n", label, r1, r2);
	iw_JOV("", "EOVF");
}
void iw_MULA	(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tMULA\t%s,%s\n", label, r1, r2);
	iw_JOV("", "EOVF");
}
void iw_MULL	(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tMULL\t%s,%s\n", label, r1, r2);
	iw_JOV("", "EOVF");
}
void iw_DIVA	(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tDIVA\t%s,%s\n", label, r1, r2);
	iw_JOV("", "EOVF");
}
void iw_DIVL	(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tDIVL\t%s,%s\n", label, r1, r2);
	iw_JOV("", "EOVF");
}
void iw_AND		(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tAND\t\t%s,%s\n", label, r1, r2);
}
void iw_OR		(char* label, char* r1, char* r2 ){	
	fprintf(output_file, "%s\tOR\t\t%s,%s\n", label, r1, r2);
}
void iw_XOR		(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tXOR\t\t%s,%s\n", label, r1, r2);
}

void iw_CPA		(char* label, char* adr, char* x ){
	fprintf(output_file, "%s\tCPA\t\t%s,%s\n", label, adr, x);
}
void iw_CPL		(char* label, char* adr, char* x ){
	fprintf(output_file, "%s\tCPL\t\t%s,%s\n", label, adr, x);
}

void iw_SLA		(char* label, char* r , char* adr){}
void iw_SRA		(char* label, char* r , char* adr){}
void iw_SLL		(char* label, char* r , char* adr){}
void iw_SRL		(char* label, char* r , char* adr){}

void iw_JPL		(char* label, char* adr){
	fprintf(output_file, "%s\tJPL\t\t%s\n", label, adr);
}
void iw_JMI		(char* label, char* adr){
	fprintf(output_file, "%s\tJMI\t\t%s\n", label, adr);
}
void iw_JNZ		(char* label, char* adr){
	fprintf(output_file, "%s\tJNZ\t\t%s\n", label, adr);
}
void iw_JZE		(char* label, char* adr){
	fprintf(output_file, "%s\tJZE\t\t%s\n", label, adr);
}
void iw_JOV		(char* label, char* adr){
	fprintf(output_file, "%s\tJOV\t\t%s\n", label, adr);
}
void iw_JUMP	(char* label, char* adr){
	fprintf(output_file, "%s\tJUMP\t%s\n", label, adr);
}

void iw_PUSH	(char* label, char* adr){
	fprintf(output_file, "%s\tPUSH\t%s\n", label, adr);
}
void iw_PUSH_3	(char* label, char* adr, char* x){
	fprintf(output_file, "%s\tPUSH\t%s,%s\n", label, adr, x);
}
void iw_PUSH_by_label(char* label, char* label_to_push){
	iw_LAD		(label, "gr1", label_to_push);
	iw_PUSH_3	("", "0", "gr1");
}
void iw_POP		(char* label, char* r){
	fprintf(output_file, "%s\tPOP\t\t%s\n", label, r);
}

void iw_CALL	(char* label, char* adr){
	fprintf(output_file, "%s\tCALL\t%s\n", label, adr);
}
void iw_RET		(char* label){
	fprintf(output_file, "%s\tRET\n", label);
}

void iw_SVC		(char* label, char* adr){
	fprintf(output_file, "\tSVC\t\t%s\n", adr);
}
void iw_NOP		(char* label){
	fprintf(output_file, "%s\tNOP\n", label);
}

void iw_START	(char* label){
	fprintf(output_file, "%s\tSTART\n", label);
}
void iw_END		(char* label){
	fprintf(output_file, "%s\tEND\n", label);
}
void iw_DS		(char* label, int n){
	fprintf(output_file, "%s\tDS\t\t%d\n", label, n);
}

void iw_DC		(char* label, int n){
	fprintf(output_file, "%s\tDC\t\t%d\n", label, n);
}
void iw_DC_str	(char* label, char* str){
	fprintf(output_file, "%s\tDC\t\t'%s'\n", label, str);
}
void iw_IN		(char* label, char* r , char* adr){
	fprintf(output_file, "%s\tIN\t\t%s,%s\n", label, r, adr);
}
void iw_OUT		(char* label, char* r , char* adr){
	fprintf(output_file, "%s\tOUT\t\t%s,%s\n", label, r, adr);
}
/*void iw_RPUSH	(char* label){}
void iw_RPOP	(char* label){}*/

void iw_label	(char* label){
	fprintf(output_file, "%s\n", label);
}
void iw_comment	(char* str){
	fprintf(output_file, "; %s\n", str);
}

/* gr1 <- ( (gr1) = (gr2) ? TRUE : FALSE ) */
void iw_EQUAL	(char* label, char* end_label){
	iw_comment	("GRA");
	iw_LD	("", "gr3", "TRUE");
	iw_CPA	("", "gr1", "gr2");
	iw_JZE	("", end_label);
	iw_LD	("", "gr3", "FALSE");
	iw_LD	(end_label, "gr1", "gr3");
}
/* gr1 <- ( (gr1) != (gr2) ? TRUE : FALSE ) */
void iw_NOTEQ	(char* label, char* end_label){
	iw_comment	("GRA");
	iw_LD	("", "gr3", "TRUE");
	iw_CPA	("", "gr1", "gr2");
	iw_JNZ	("", end_label);
	iw_LD	("", "gr3", "FALSE");
	iw_LD	(end_label, "gr1", "gr3");
}
/* gr1 <- ( (gr1) < (gr2) ? TRUE : FALSE ) */
void iw_LEA		(char* label, char* end_label){
	iw_comment	("GRA");
	iw_LD	("", "gr3", "TRUE");
	iw_CPA	("", "gr1", "gr2");
	iw_JMI	("", end_label);
	iw_LD	("", "gr3", "FALSE");
	iw_LD	(end_label, "gr1", "gr3");
}
/* gr1 <- ( (gr1) <= (gr2) ? TRUE : FALSE ) */
void iw_LEEQA	(char* label, char* end_label){
	iw_comment	("GRA");
	iw_LD	("", "gr3", "FALSE");
	iw_CPA	("", "gr1", "gr2");
	iw_JPL	("", end_label);
	iw_LD	("", "gr3", "TRUE");
	iw_LD	(end_label, "gr1", "gr3");
}
/* gr1 <- ( (gr1) > (gr2) ? TRUE : FALSE ) */
void iw_GRA		(char* label, char* end_label){
	iw_comment	("GRA");
	iw_LD	("", "gr3", "TRUE");
	iw_CPA	("", "gr1", "gr2");
	iw_JPL	("", end_label);
	iw_LD	("", "gr3", "FALSE");
	iw_LD	(end_label, "gr1", "gr3");
}
/* gr1 <- ( (gr1) >= (gr2) ? TRUE : FALSE ) */
void iw_GREQA	(char* label, char* end_label){
	iw_comment	("GRA");
	iw_LD	("", "gr3", "FALSE");
	iw_CPA	("", "gr1", "gr2");
	iw_JMI	("", end_label);
	iw_LD	("", "gr3", "TRUE");
	iw_LD	(end_label, "gr1", "gr3");
}

DCData* add_footer_dc_num(char* label, int num){
	DCData* hoge = malloc(sizeof(DCData));
	strcpy(hoge->label, label);
	hoge->str = NULL;
	hoge->is_ds = 0;
	hoge->num = num;
	if(dc_data_tail == NULL){
		dc_data_head = hoge;
		dc_data_tail = hoge;
	}
	else{
		dc_data_tail->next = hoge;
		dc_data_tail = hoge;
	}
	return dc_data_tail;
}

DCData* add_footer_dc_str(char* label, char* str){
	add_footer_dc_num(label, 0);
	dc_data_tail->str = calloc(strlen(str), sizeof(char));
	strcpy(dc_data_tail->str, str);
	return dc_data_tail;
}

DCData* add_footer_ds(char* label, int size){
	add_footer_dc_num(label, size);
	dc_data_tail->is_ds = 1;
	return dc_data_tail;
}


void print_footer_dc(){
	DCData* dc_data = dc_data_head;
	while(dc_data != NULL){
		if(dc_data->is_ds){
			iw_DS		(dc_data->label, dc_data->num);
		}
		else if(dc_data->str == NULL){
			iw_DC		(dc_data->label, dc_data->num);
		}
		else{
			iw_DC_str	(dc_data->label, dc_data->str);
		}
		dc_data = dc_data->next;
	}
}

char* get_proc_label(ProcData* proc_data){
	char* hoge = calloc(sizeof(char), 10);
	fprintf(output_file, "; %7.7lx %s %s\n",
			(unsigned long)proc_data, "procedure", proc_data->name);
	sprintf(hoge, "$%7.7lx", (unsigned long)proc_data);
	return hoge;
	/*
	char* hoge = calloc(sizeof(char), MAXSTRSIZE + 10);
	sprintf(hoge, "%%%7s", proc_data->name);
	return hoge;
	*/
}

char* get_var_label(VarData* var_data){
	char* hoge;
	if(!var_data->is_declaration){
		VarRefData* var_ref_data = (VarRefData*)var_data->data;
		return get_var_label((VarData*)var_ref_data->data);
	}
	hoge = calloc(sizeof(char), 10);
	VarDecData* var_dec_data = (VarDecData*)var_data->data;
	if(var_data->is_declaration == 0){
		VarRefData* var_ref_data = (VarRefData*)var_data->data;
		VarData* var_data = (VarData*)var_ref_data->data;
		var_dec_data = (VarDecData*)var_data->data;
	}
	/*
	if(var_dec_data->namespace->name == NULL){
		sprintf(hoge, "$%4s%%%%g%%",
				var_dec_data->name);
	}
	else{
		sprintf(hoge, "$%3s%%%3s",
				var_dec_data->name, var_dec_data->namespace->name);
	}
	*/
	fprintf(output_file, "; %7.7lx %s %s %% %s\n",
			(unsigned long)var_data, "var", var_dec_data->name,
			(var_dec_data->namespace->name != NULL ?
					var_dec_data->namespace->name : "%%grobal%%"));
	sprintf(hoge, "$%7.7lx", (unsigned long)var_data);
	return hoge;
}

char* get_label(SyntaxTreeNode* node){
	char* hoge;
	if(node->s_elem_it == SVAR){
		return get_label(node->child);
	}
	if(node->s_elem_it == SVARNAME){
		return get_var_label((VarData*)node->data);
	}
	else if(node->s_elem_it == SSUBPROGDEC){
		return get_proc_label((ProcData*)node->data);
	}
	fprintf(output_file, "; %7.7lx %s %s\n",
			(unsigned long)node, SYNTAXDIC[node->s_elem_it], node->string_attr);
	hoge = calloc(sizeof(char), 10);
	sprintf(hoge, "$%7.7lx", (unsigned long)node);
	return hoge;
}

char* get_end_label(SyntaxTreeNode* node){
	char* hoge = calloc(sizeof(char), MAXSTRSIZE + 10);
	sprintf(hoge, ".%7.7lx", (unsigned long)node);
	return hoge;
}
