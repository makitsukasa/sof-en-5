#include "cross-referencer.h"

#define gr0 "gr0"
#define gr1 "gr1"
#define gr2 "gr2"
#define gr3 "gr3"
#define gr7 "gr7"

FILE* output_file;

typedef struct DCData_{
	char label[10];
	int is_ds;
	int num;
	char* str;
	struct DCData_* next;
} DCData;
DCData* dc_data_head;
DCData* dc_data_tail;

void generate_assm(SyntaxTreeNode* node);

/* instruction-words.c */
void iw_LD		(char* label, char* r1, char* r2 );
void iw_LD_3	(char* label, char* r , char* adr, char* x );
void iw_ST		(char* label, char* r1, char* r2 );
void iw_ST_3	(char* label, char* r , char* adr, char* x );
void iw_LAD		(char* label, char* r , char* adr);
void iw_LAD_num	(char* label, char* r , int num);
/*void iw_LAD_3	(char* label, char* r , char* adr, char* x);*/

void iw_ADDA	(char* label, char* r1, char* r2 );
/*void iw_ADDL	(char* label, char* r1, char* r2 );*/
void iw_SUBA	(char* label, char* r1, char* r2 );
/*void iw_SUBL	(char* label, char* r1, char* r2 );*/
void iw_MULA	(char* label, char* r1, char* r2 );
/*void iw_MULL	(char* label, char* r1, char* r2 );*/
void iw_DIVA	(char* label, char* r1, char* r2 );
/*void iw_DIVL	(char* label, char* r1, char* r2 );*/
void iw_AND		(char* label, char* r1, char* r2 );
void iw_OR		(char* label, char* r1, char* r2 );
void iw_XOR		(char* label, char* r1, char* r2 );

void iw_CPA		(char* label, char* r1, char* r2 );
/*
void iw_CPL		(char* label, char* r1, char* r2 );
void iw_SLA		(char* label, char* r , char* adr);
void iw_SRA		(char* label, char* r , char* adr);
void iw_SLL		(char* label, char* r , char* adr);
void iw_SRL		(char* label, char* r , char* adr);
*/
void iw_JMI		(char* label, char* adr);
void iw_JNZ		(char* label, char* adr);
void iw_JZE		(char* label, char* adr);
void iw_JOV		(char* label, char* adr);
void iw_JUMP	(char* label, char* adr);

/*void iw_PUSH	(char* label, char* adr);*/
void iw_PUSH_3	(char* label, char* adr, char* x);
void iw_PUSH_by_label(char* label, char* label_to_push);
void iw_PUSH_by_indexed_label(char* label, char* label_to_push, char* label_index);
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
/*
void iw_IN		(char* label, char* r, char* adr);
void iw_OUT		(char* label, char* r, char* adr);
void iw_RPUSH	(char* label);
void iw_RPOP	(char* label);
*/

void iw_label	(char* label);
void iw_comment	(char* str);

void iw_EQUAL	(char* label, char* end_label);
void iw_NOTEQ	(char* label, char* end_label);
void iw_LEA		(char* label, char* end_label);
void iw_LEEQA	(char* label, char* end_label);
void iw_GRA		(char* label, char* end_label);
void iw_GREQA	(char* label, char* end_label);

char* get_proc_label(ProcData* proc_data);
char* get_var_label	(VarData* var_data);
char* get_label		(SyntaxTreeNode* node);
char* get_end_label	(SyntaxTreeNode* node);

void add_footer_ds		(char* label, int size);
void add_footer_dc_num	(char* label, int num);
void add_footer_dc_str	(char* label, char* str);
void print_footer_dc	();

#define FOOTER_LIB "\
EOVF\n\
	CALL WRITENEWLINE\n\
	LAD gr1, EOVF1\n\
	LD gr2, gr0\n\
	CALL WRITESTR\n\
	CALL WRITENEWLINE\n\
	SVC 1 ; overflow error stop\n\
EOVF1 DC '***** Run-Time Error : Overflow *****'\n\
E0DIV\n\
	JNZ EOVF\n\
	CALL WRITENEWLINE\n\
	LAD gr1, E0DIV1\n\
	LD gr2, gr0\n\
	CALL WRITESTR\n\
	CALL WRITENEWLINE\n\
	SVC 2 ; 0-divide error stop\n\
E0DIV1 DC '***** Run-Time Error : Zero-Divide *****'\n\
EROV\n\
	CALL WRITENEWLINE\n\
	LAD gr1, EROV1\n\
	LD gr2, gr0\n\
	CALL WRITESTR\n\
	CALL WRITENEWLINE\n\
	SVC 3 ; range-over error stop\n\
EROV1 DC '***** Run-Time Error : Range-Over in ArrayIndex *****'\n\
WRITECHAR\n\
; gr1 の値（文字）を gr2 のけた数で出力する．\n\
; gr2 が 0 なら必要最小限の桁数で出力する\n\
	RPUSH\n\
	LD gr6, SPACE\n\
	LD gr7, OBUFSIZE\n\
WC1\n\
	SUBA gr2, ONE ; while(--c > 0) {\n\
	JZE WC2\n\
	JMI WC2\n\
	ST gr6, OBUF,gr7 ; *p++ = ' ';\n\
	CALL BOVFCHECK\n\
	JUMP WC1 ; }\n\
WC2\n\
	ST gr1, OBUF,gr7 ; *p++ = gr1;\n\
	CALL BOVFCHECK\n\
	ST gr7, OBUFSIZE\n\
	RPOP\n\
	RET\n\
WRITESTR\n\
; gr1 が指す文字列を gr2 のけた数で出力する．\n\
; gr2 が 0 なら必要最小限の桁数で出力する\n\
	RPUSH\n\
	LD gr6, gr1 ; p = gr1;\n\
WS1\n\
	LD gr4, 0,gr6 ; while(*p != '¥0') {\n\
	JZE WS2\n\
	ADDA gr6, ONE ; p++;\n\
	SUBA gr2, ONE ; c- ;\n\
	JUMP WS1 ; }\n\
WS2\n\
	LD gr7, OBUFSIZE ; q = OBUFSIZE;\n\
	LD gr5, SPACE\n\
WS3\n\
	SUBA gr2, ONE ; while(--c >= 0) {\n\
	JMI WS4\n\
	ST gr5, OBUF,gr7 ; *q++ = ' ';\n\
	CALL BOVFCHECK\n\
	JUMP WS3 ; }\n\
WS4\n\
	LD gr4, 0,gr1 ; while(*gr1 != '¥0') {\n\
	JZE WS5\n\
	ST gr4, OBUF,gr7 ; *q++ = *gr1++;\n\
	ADDA gr1, ONE\n\
	CALL BOVFCHECK\n\
	JUMP WS4 ; }\n\
WS5\n\
	ST gr7, OBUFSIZE ; OBUFSIZE = q;\n\
	RPOP\n\
	RET\n\
BOVFCHECK\n\
	ADDA gr7, ONE\n\
	CPA gr7, BOVFLEVEL\n\
	JMI BOVF1\n\
	CALL WRITENEWLINE\n\
	LD gr7, OBUFSIZE\n\
BOVF1\n\
	RET\n\
BOVFLEVEL DC 256\n\
WRITEINT\n\
; gr1 の値（整数）を gr2 のけた数で出力する．\n\
; gr2 が 0 なら必要最小限の桁数で出力する\n\
	RPUSH\n\
	LD gr7, gr0 ; flag = 0;\n\
	CPA gr1, gr0 ; if(gr1>=0) goto WI1;\n\
	JPL WI1\n\
	JZE WI1\n\
	LD gr4, gr0 ; gr1= - gr1;\n\
	SUBA gr4, gr1\n\
	CPA gr4, gr1\n\
	JZE WI6\n\
	LD gr1, gr4\n\
	LD gr7, ONE ; flag = 1;\n\
WI1\n\
	LD gr6, SIX ; p = INTBUF+6;\n\
	ST gr0, INTBUF,gr6 ; *p = '¥0';\n\
	SUBA gr6, ONE ; p- ;\n\
	CPA gr1, gr0 ; if(gr1 == 0)\n\
	JNZ WI2\n\
	LD gr4, ZERO ; *p = '0';\n\
	ST gr4, INTBUF,gr6\n\
	JUMP WI5 ; }\n\
WI2 ; else {\n\
	CPA gr1, gr0 ; while(gr1 != 0) {\n\
	JZE WI3\n\
	LD gr5, gr1 ; gr5 = gr1 - (gr1 / 10) * 10;\n\
	DIVA gr1, TEN ; gr1 /= 10;\n\
	LD gr4, gr1\n\
	MULA gr4, TEN\n\
	SUBA gr5, gr4\n\
	ADDA gr5, ZERO ; gr5 += '0';\n\
	ST gr5, INTBUF,gr6 ; *p = gr5;\n\
	SUBA gr6, ONE ; p- ;\n\
	JUMP WI2 ; }\n\
WI3\n\
	CPA gr7, gr0 ; if(flag != 0) {\n\
	JZE WI4\n\
	LD gr4, MINUS ; *p = '-';\n\
	ST gr4, INTBUF,gr6\n\
	JUMP WI5 ; }\n\
WI4\n\
	ADDA gr6, ONE ; else p++;\n\
	; }\n\
WI5\n\
	LAD gr1, INTBUF,gr6 ; gr1 = p;\n\
	CALL WRITESTR ; WRITESTR();\n\
	RPOP\n\
	RET\n\
WI6\n\
	LAD gr1, MMINT\n\
	CALL WRITESTR ; WRITESTR();\n\
	RPOP\n\
	RET\n\
MMINT DC '-32768'\n\
WRITEBOOL\n\
; gr1 の値（真理値）が 0 なら'FALSE'を\n\
; 0 以外なら'TRUE'を gr2 のけた数で出力する．\n\
; gr2 が 0 なら必要最小限の桁数で出力する\n\
	RPUSH\n\
	CPA gr1, gr0 ; if(gr1 != 0)\n\
	JZE WB1\n\
	LAD gr1, WBTRUE ; gr1 = \"TRUE\";\n\
	JUMP WB2\n\
WB1 ; else\n\
	LAD gr1, WBFALSE ; gr1 = \"FALSE\";\n\
WB2\n\
	CALL WRITESTR ; WRITESTR();\n\
	RPOP\n\
	RET\n\
WBTRUE DC 'TRUE'\n\
WBFALSE DC 'FALSE'\n\
WRITENEWLINE\n\
; 改行を出力する\n\
	RPUSH\n\
	LD gr7, OBUFSIZE\n\
	LD gr6, NEWLINE\n\
	ST gr6, OBUF,gr7\n\
	ADDA gr7, ONE\n\
	ST gr7, OBUFSIZE\n\
	OUT OBUF, OBUFSIZE\n\
	ST gr0, OBUFSIZE\n\
	RPOP\n\
	RET\n\
FLUSH\n\
	RPUSH\n\
	LD gr7, OBUFSIZE\n\
	JZE FL1\n\
	CALL WRITENEWLINE\n\
FL1\n\
	RPOP\n\
	RET\n\
READCHAR\n\
; gr1 が指す番地に文字一つを読み込む\n\
	RPUSH\n\
	LD gr5, RPBBUF ; if(RPBBUF != '¥0') {\n\
	JZE RC0\n\
	ST gr5, 0,gr1 ; *gr1 = RPBBUF;\n\
	ST gr0, RPBBUF ; RPBBUF = '¥0'\n\
	JUMP RC3 ; return; }\n\
RC0\n\
	LD gr7, INP ; inp = INP;\n\
	LD gr6, IBUFSIZE ; if(IBUFSIZE == 0) {\n\
	JNZ RC1\n\
	IN IBUF, IBUFSIZE ; IN();\n\
	LD gr7, gr0 ; inp = 0; }\n\
RC1\n\
	CPA gr7, IBUFSIZE ; if(inp == IBUFSIZE) {\n\
	JNZ RC2\n\
	LD gr5, NEWLINE ; *gr1 = '¥n';\n\
	ST gr5, 0,gr1\n\
	ST gr0, IBUFSIZE ; IBUFSIZE = INP = 0;\n\
	ST gr0, INP\n\
	JUMP RC3 ; }\n\
RC2 ; else {\n\
	LD gr5, IBUF,gr7 ; *gr1 = *inp++;\n\
	ADDA gr7, ONE\n\
	ST gr5, 0,gr1\n\
	ST gr7, INP ; INP = inp;\n\
RC3 ; }\n\
	RPOP\n\
	RET\n\
READINT\n\
; gr1 が指す番地に整数値一つを読み込む\n\
	RPUSH\n\
RI1 ; do {\n\
	CALL READCHAR ; ch = READCHAR();\n\
	LD gr7, 0,gr1\n\
	CPA gr7, SPACE ; } while(ch==' ' || ch=='¥t' || ch=='¥n');\n\
	JZE RI1\n\
	CPA gr7, TAB\n\
	JZE RI1\n\
	CPA gr7, NEWLINE\n\
	JZE RI1\n\
	LD gr5, ONE ; flag = 1\n\
	CPA gr7, MINUS ; if(ch == '-') {\n\
	JNZ RI4\n\
	LD gr5, gr0 ; flag = 0;\n\
	CALL READCHAR ; ch = READCHAR();\n\
	LD gr7, 0,gr1\n\
RI4 ; }\n\
	LD gr6, gr0 ; v = 0;\n\
RI2\n\
	CPA gr7, ZERO ; while('0' <= ch && ch <= '9') {\n\
	JMI RI3\n\
	CPA gr7, NINE\n\
	JPL RI3\n\
	MULA gr6, TEN ; v = v*10+ch-'0';\n\
	ADDA gr6, gr7\n\
	SUBA gr6, ZERO\n\
	CALL READCHAR ; ch = READSCHAR();\n\
	LD gr7, 0,gr1\n\
	JUMP RI2 ; }\n\
RI3\n\
	ST gr7, RPBBUF ; ReadPushBack();\n\
	ST gr6, 0,gr1 ; *gr1 = v;\n\
	CPA gr5, gr0 ; if(flag == 0) {\n\
	JNZ RI5\n\
	SUBA gr5, gr6 ; *gr1 = -v;\n\
	ST gr5, 0,gr1\n\
RI5 ; }\n\
	RPOP\n\
	RET\n\
READSKIPTONEWLINE\n\
; 入力を改行コードまで（改行コードも含む）読み飛ばす\n\
	ST gr0, IBUFSIZE\n\
	ST gr0, INP\n\
	ST gr0, RPBBUF\n\
	RET\n\
TRUE DC 1\n\
FALSE DC 0\n\
ONE DC 1\n\
SIX DC 6\n\
TEN DC 10\n\
SPACE DC #0020 ; ' '\n\
MINUS DC #002D ; '-'\n\
TAB DC #0009 ; '¥t'\n\
ZERO DC #0030 ; '0'\n\
NINE DC #0039 ; '9'\n\
NEWLINE DC #000A ; '¥n'\n\
INTBUF DS 8\n\
OBUFSIZE DC 0\n\
IBUFSIZE DC 0\n\
INP DC 0\n\
OBUF DS 257\n\
IBUF DS 257\n\
RPBBUF DC 0\n"
