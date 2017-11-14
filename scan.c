#include "token-list.h"
#include <ctype.h>

int num_attr;
char string_attr[MAXSTRSIZE];

FILE *fp;
int cbuf[2];

int line_num;

void update_cbuf(void){
	cbuf[0] = cbuf[1];
	cbuf[1] = fgetc(fp);
}

void clear_attr(void){
	int i;
	for(i = 0; i < MAXSTRSIZE; i++) string_attr[i] = '\0';
	num_attr = 0;
}

/* check new line or not and update cbuf
 * return value :
 * 	not new line	: 0
 * 	\n or \r		: 1
 * 	\n\r or \r\n	: 2
 */
int check_new_line(void){
	if(	(cbuf[0] == '\r' && cbuf[1] == '\n') ||
		(cbuf[0] == '\n' && cbuf[1] == '\r') ){
		/* ignore 2 characters */
		update_cbuf();
		update_cbuf();
		return 2;
	}
	if(cbuf[0] == '\r'  || cbuf[0] == '\n'){
		/* ignore a character */
		update_cbuf();
		return 1;
	}
	return 0;
}

int init_scan(char *filename){
	line_num = 0;
	if ((fp = fopen(filename, "r")) == NULL) {
		return -1;
	}
	update_cbuf();
	update_cbuf();
	clear_attr();
	return 0;
}

int scan(void){
	int i;

	clear_attr();

	if(line_num <= 0){
		line_num = 1;
	}

	if(cbuf[0] == EOF){
		/*printf("eof\n");*/
		return -1;
	}

	/* separator (space or tab) */
	if(cbuf[0] == ' '  || cbuf[0] == '\t'){
		update_cbuf();
		/*printf("separator (space or tab)\n");*/
		return scan();
	}

	/* separator (new line) */
	if(check_new_line() > 0){
		line_num++;
		/*printf("separator (new line)\n");*/
		return scan();
	}

	/* name or keyword 
	 *
	 * start	: alpha
	 * end		: !(alpha or num)
	 */
	if(isalpha(cbuf[0])){
		for(i = 0; isalnum(cbuf[0]) && i < MAXSTRSIZE; i++){
			string_attr[i] = cbuf[0];
			update_cbuf();
		}
		if(i >= MAXSTRSIZE){
			error(line_num, "too long token (name)");
			return -1;
		}
		for(i = 0; i < KEYWORDSIZE; i++){
			if(strcmp(string_attr, keyword[i].keyword) == 0){
				/*printf("keyword %s\n", string_attr);*/
				return keyword[i].keytoken;
			}
		}
		/*printf("name %s\n", string_attr);*/
		return TNAME;
	}

	/* digit
	 *
	 * start	: digit
	 * end		: !digit
	 */
	if(isdigit(cbuf[0])){
		for(i = 0; isdigit(cbuf[0]) && i < MAXSTRSIZE; i++){
			string_attr[i] = cbuf[0];
			update_cbuf();
		}
		if(i > 5){
			error(line_num, "too long token (digit)");
			return -1;
		}
		num_attr = atoi(string_attr);
		if(num_attr > 32767){
			error(line_num, "too big digit");
			return -1;
		}
		/*printf("digit %d\n", num_attr);*/
		return TNUMBER;
	}

	/* string 
	 *
	 * start	: '
	 * end		: ' ( '' is not end )
	 */
	if(cbuf[0] == '\''){
		/* ignore a character ( ' ) */
		update_cbuf();

		for(i = 0; i < MAXSTRSIZE; i++){
			if(cbuf[0] == EOF || check_new_line() > 0){
				error(line_num, "missing terminating ' character");
			}
			else if(cbuf[0] == '\''){
				/* '' is not end */
				if(cbuf[1] == '\''){
					string_attr[i] = '\'';
					i++;
					string_attr[i] = '\'';
					update_cbuf();
				}
				/* ' is end */
				else{
					/* ignore a character ( ' ) */
					update_cbuf();
					break;
				}
			}
			string_attr[i] = cbuf[0];
			update_cbuf();
		}
		if(i >= MAXSTRSIZE){
			error(line_num, "too long token (string)");
			return -1;
		}
		/*printf("string %s\n", string_attr);*/
		return TSTRING;
	}

	/* comment / * * / 
	 *
	 * start	: / *
	 * end		: * /
	 */
	if(cbuf[0] == '/' && cbuf[1] == '*'){
		/* ignore 2 characters ( / and * ) */
		update_cbuf();
		update_cbuf();

		for(i = 0; i < MAXSTRSIZE; i++){
			if(cbuf[0] == EOF){
				error(line_num, "unterminated comment");
				return -1;
			}
			else if(cbuf[0] == '*' && cbuf[1] == '/'){
				/* ignore 2 characters ( * and / ) */
				update_cbuf();
				update_cbuf();
				break;
			}
			string_attr[i] = cbuf[0];
			update_cbuf();
		}
		/*printf("comment/ ** / %s\n", string_attr);*/
		return scan();
	}

	/* comment {}
	 *
	 * start	: {
	 * end		: }
	 */
	if(cbuf[0] == '{'){
		/* ignore a character ( { ) */
		update_cbuf();
		for(i = 0; i < MAXSTRSIZE; i++){
			if(cbuf[0] == EOF){
				error(line_num, "unterminated comment");
				return -1;
			}
			else 
			if(cbuf[0] == '}'){
				/* ignore a character ( } ) */
				update_cbuf();
				break;
			}
			string_attr[i] = cbuf[0];
			update_cbuf();
		}
		/*printf("comment{} %s\n", string_attr);*/
		return scan();
	}

	/* symbol 
	 * 
	 * 1 or 2 characters
	 * check lists in the oder
	 * 2 lists (symbol1, symbol2) are defined in token-list.h
	 */
	for(i = 0; i < SYMBOL2SIZE; i++){
		if(	cbuf[0] == symbol2[i].keyword[0] &&
			cbuf[1] == symbol2[i].keyword[1]){
			string_attr[0] = cbuf[0];
			string_attr[1] = cbuf[1];
			update_cbuf();
			update_cbuf();
			return symbol2[i].keytoken;
		}
	}
	for(i = 0; i < SYMBOL1SIZE; i++){
		if(cbuf[0] == symbol1[i].keyword[0]){
			string_attr[0] = cbuf[0];
			update_cbuf();
			return symbol1[i].keytoken;
		}
	}
	error(line_num, "unknown symbol");
	return -1;
}

void end_scan(void){
	fclose(fp);
}
