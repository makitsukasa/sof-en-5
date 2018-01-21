#include "compiler.h"

void iw_LD		(char* label, char* r1, char* r2 ){
	fprintf(fp, "\tLD\t\t%s,%s\n", r1, r2);
}
void iw_ST		(char* label, char* r1, char* r2 ){}
void iw_LAD		(char* label, char* r , char* adr){
	fprintf(fp, "\tLAD\t\t%s,%s\n", r, adr);
}
void iw_LAD_3	(char* label, char* r, char* adr, char* x){
	fprintf(fp, "\tLAD\t\t%s,%s, %s\n", r, adr, x);
}

void iw_ADDA	(char* label, char* r1, char* r2 ){}
void iw_ADDL	(char* label, char* r1, char* r2 ){}
void iw_SUBA	(char* label, char* r1, char* r2 ){}
void iw_SUBL	(char* label, char* r1, char* r2 ){}
void iw_AND		(char* label, char* r1, char* r2 ){}
void iw_OR		(char* label, char* r1, char* r2 ){}
void iw_XOR		(char* label, char* r1, char* r2 ){}

void iw_CPA		(char* label, char* r1, char* r2 ){}
void iw_CPL		(char* label, char* r1, char* r2 ){}

void iw_SLA		(char* label, char* r , char* adr){}
void iw_SRA		(char* label, char* r , char* adr){}
void iw_SLL		(char* label, char* r , char* adr){}
void iw_SRL		(char* label, char* r , char* adr){}

void iw_JPL		(char* label, char* adr){}
void iw_JMI		(char* label, char* adr){}
void iw_JNZ		(char* label, char* adr){}
void iw_JZE		(char* label, char* adr){}
void iw_JOV		(char* label, char* adr){}
void iw_JUMP	(char* label, char* adr){}

void iw_PUSH	(char* label, char* adr, char* x){
	fprintf(fp, "%s\tPUSH\t%s,%s\n", label, adr, x);
}
void iw_POP		(char* label, char* r){
	fprintf(fp, "%s\tPOP\t\t%s\n", label, r);
}

void iw_CALL	(char* label, char* adr){
	fprintf(fp, "%s\tCALL\t%s\n", label, adr);
}
void iw_RET		(char* label){
	fprintf(fp, "%s\tRET\n", label);
}

void iw_SVC		(char* label, char* adr){
	fprintf(fp, "\tSVC\t\t%s\n", adr);
}
void iw_NOP		(char* label){
	fprintf(fp, "%s\tNOP\n", label);
}

void iw_START	(char* label){
	fprintf(fp, "%s\tSTART\n", label);
}
void iw_END		(char* label){
	fprintf(fp, "%s\tEND\n", label);
}
void iw_DS		(char* label, int n){
	fprintf(fp, "%s\tDS\t\t%d\n", label, n);
}

void iw_DC		(char* label, int n){
	fprintf(fp, "%s\tDC\t\t%d\n", label, n);
}
void iw_DC_str	(char* label, char* str){
	fprintf(fp, "%s\tDC\t\t'%s'\n", label, str);
}
void iw_IN		(char* label, char* r , char* adr){
	fprintf(fp, "%s\tIN\t\t%s,%s\n", label, r, adr);
}
void iw_OUT		(char* label, char* r , char* adr){
	fprintf(fp, "%s\tOUT\t\t%s,%s\n", label, r, adr);
}
/*void iw_RPUSH	(char* label){}
void iw_RPOP	(char* label){}*/

void iw_label	(char* label){
	fprintf(fp, "%s\n", label);
}
void iw_comment	(char* str){
	fprintf(fp, "; %s\n", str);
}

