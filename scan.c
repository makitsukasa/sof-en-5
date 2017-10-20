#include "token-list.h"
#include <ctype.h>

int num_attr;
char string_attr[MAXSTRSIZE];

FILE *fp;
int cbuf[2];

int linenum;

void update_cbuf(void){
	cbuf[0] = cbuf[1];
	cbuf[1] = fgetc(fp);
}

void clear_attr(void){
	int i;
	for(i = 0; i < MAXSTRSIZE; i++) string_attr[i] = '\0';
	num_attr = 0;
}

int init_scan(char *filename){
	if ((fp = fopen(filename, "r")) == NULL) {
		error("error : file can not open");
	}
	update_cbuf();
	update_cbuf();
	clear_attr();
	return 0;
}

int scan(void){
	clear_attr();

	if(cbuf[0] == EOF){
		//printf("eof\n");
		return -1;
	}
	/* separator (space or tab)
	 */
	/*else */if(cbuf[0] == ' '  || cbuf[0] == '\t'){
		update_cbuf();
		//printf("separator (space or tab)\n");
		return scan();
	}
	/* separator (new line)
	 */
	/*else */if(cbuf[0] == '\r'  || cbuf[0] == '\n'){
		if(	(cbuf[0] == '\r' && cbuf[1] == '\n') ||
			(cbuf[0] == '\n' && cbuf[1] == '\r') ){
				update_cbuf();
		}
		update_cbuf();
		//printf("separator (new line)\n");
		return scan();
	}
	/* name or keyword 
	 *
	 * start	: alpha
	 * end		: !(alpha or num)
	 */
	/*else */if(isalpha(cbuf[0])){
		int i;
		for(i = 0; isalnum(cbuf[0]) && i < MAXSTRSIZE; i++){
			string_attr[i] = cbuf[0];
			update_cbuf();
		}
		if(i >= MAXSTRSIZE){
			fprintf(stderr, "too long token (name)\n");
			return -1;
		}
		for(i = 0; i < KEYWORDSIZE; i++){
			if(strcmp(string_attr, key[i].keyword) == 0){
				//printf("keyword %s\n", string_attr);
				return key[i].keytoken;
			}
		}
		//printf("name %s\n", string_attr);
		return TNAME;
	}
	/* digit 
	 *
	 * start	: digit
	 * end		: !digit
	 */
	/*else */if(isdigit(cbuf[0])){
		int i;
		for(i = 0; isdigit(cbuf[0]) && i < MAXSTRSIZE; i++){
			string_attr[i] = cbuf[0];
			update_cbuf();
		}
		if(i >= MAXSTRSIZE){
			fprintf(stderr, "too long token (digit)\n");
			return -1;
		}
		num_attr = atoi(string_attr);
		//printf("digit %d\n", num_attr);
		return TNUMBER;
	}
	/* string 
	 *
	 * start	: '
	 * end		: ' 
	 * '' is not end
	 */
	/*else */if(cbuf[0] == '\''){
		int i;
		/* ignore start ' */
		update_cbuf();
		for(i = 0; i < MAXSTRSIZE; i++){
			if(cbuf[0] == '\''){
				if(cbuf[1] == '\''){
					string_attr[i] = '\'';
					i++;
					string_attr[i] = '\'';
					update_cbuf();
				}
				else{
					/* ignore end ' */
					update_cbuf();
					break;
				}
			}
			string_attr[i] = cbuf[0];
			update_cbuf();
		}
		if(i >= MAXSTRSIZE){
			fprintf(stderr, "too long token (string)\n");
			return -1;
		}
		//printf("string %s\n", string_attr);
		return TSTRING;
	}
	/* comment / * * / 
	 *
	 * start	: / *
	 * end		: * /
	 */
	/*else */if(cbuf[0] == '/' && cbuf[1] == '*'){
		int i;
		/* ignore / and * */
		update_cbuf();
		update_cbuf();
		for(i = 0; i < MAXSTRSIZE; i++){
			if(cbuf[0] == '*' && cbuf[1] == '/'){
				/* ignore * and / */
				update_cbuf();
				update_cbuf();
				break;
			}
			string_attr[i] = cbuf[0];
			update_cbuf();
		}
		//printf("comment/**/ %s\n", string_attr);
		return 0;
	}
	/* comment {} 
	 *
	 * start	: {
	 * end		: }
	 */
	/*else */if(cbuf[0] == '{'){
		int i;
		/* ignore { */
		update_cbuf();
		for(i = 0; i < MAXSTRSIZE; i++){
			if(cbuf[0] == '}'){
				/* ignore } */
				update_cbuf();
				break;
			}
			string_attr[i] = cbuf[0];
			update_cbuf();
		}
		//printf("comment{} %s\n", string_attr);
		return 0;
	}
	/* symbol */
	/*else*/{
		int i;
		for(i = 0; i < SYMBOL2SIZE; i++){
			if(	cbuf[0] == symbol2[i].keyword[0] &&
				cbuf[1] == symbol2[i].keyword[1]){
				update_cbuf();
				update_cbuf();
				return symbol2[i].keytoken;
			}
		}
		for(i = 0; i < SYMBOL1SIZE; i++){
			if(cbuf[0] == symbol1[i].keyword[0]){
				update_cbuf();
				return symbol1[i].keytoken;
			}
		}
		fprintf(stderr, "unknown symbol %c\n", cbuf[0]);
		return -1;
	}
	return -1;
}

int get_linenum(void){
	return 0;
}

void end_scan(void){
	fclose(fp);
}