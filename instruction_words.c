#include "compiler.h"

void iw_LD		(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tLD\t\t%s,%s\n", label, r1, r2);
}
void iw_ST		(char* label, char* r1, char* r2 ){
	fprintf(output_file, "%s\tST\t\t%s,%s\n", label, r1, r2);
}
void iw_LAD		(char* label, char* r , char* adr){
	fprintf(output_file, "%s\tLAD\t\t%s,%s\n", label, r, adr);
}
void iw_LAD_3	(char* label, char* r, char* adr, char* x){
	fprintf(output_file, "%s\tLAD\t\t%s,%s, %s\n", label, r, adr, x);
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

void iw_CPA		(char* label, char* r1, char* r2 ){}
void iw_CPL		(char* label, char* r1, char* r2 ){}

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

