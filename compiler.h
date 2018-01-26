#include "cross-referencer.h"

FILE* output_file;

typedef struct DCData_{
	char label[10];
	int is_ds;
	char* str;
	int num;
	struct DCData_* next;
}DCData;
DCData* dc_data_head;
DCData* dc_data_tail;

void iw_LD		(char* label, char* r1, char* r2 );
void iw_ST		(char* label, char* r1, char* r2 );
void iw_LAD		(char* label, char* r , char* adr);
void iw_LAD_3	(char* label, char* r , char* adr, char* x);

void iw_ADDA	(char* label, char* r1, char* r2 );
void iw_ADDL	(char* label, char* r1, char* r2 );
void iw_SUBA	(char* label, char* r1, char* r2 );
void iw_SUBL	(char* label, char* r1, char* r2 );
void iw_AND		(char* label, char* r1, char* r2 );
void iw_OR		(char* label, char* r1, char* r2 );
void iw_XOR		(char* label, char* r1, char* r2 );

void iw_CPA		(char* label, char* r1, char* r2 );
void iw_JMI		(char* label, char* adr);
void iw_JNZ		(char* label, char* adr);
void iw_JZE		(char* label, char* adr);
void iw_JOV		(char* label, char* adr);
void iw_JUMP	(char* label, char* adr);

void iw_PUSH	(char* label, char* adr);
void iw_PUSH_3	(char* label, char* adr, char* x);
void iw_PUSH_by_label	(char* label, char* label_to_push);
void iw_POP		(char* label, char* r);

void iw_CALL	(char* label, char* adr);
void iw_RET		(char* label);

void iw_SVC		(char* label, char* adr);
void iw_NOP		(char* label);

void iw_START	(char* label);
void iw_END		(char* label);
void iw_DS		(char* label, int n);

void iw_DC		(char* label, int n);
void iw_DC_str	(char* label, char* str);
void iw_IN		(char* label, char* r, char* adr);
void iw_OUT		(char* label, char* r, char* adr);

void iw_label	(char* label);
void iw_comment	(char* str);

DCData* add_footer_dc_num(char* label, int num);
DCData* add_footer_dc_str(char* label, char* str);
void print_footer_dc();

char* get_proc_label(ProcData* proc_data);
char* get_var_label(VarData* var_data);
char* get_label(SyntaxTreeNode* node);
char* get_end_label(SyntaxTreeNode* node);
