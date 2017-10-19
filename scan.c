#include "token-list.h"
#include <ctype.h>

int num_attr;
char string_attr[MAXSTRSIZE];

file* fp;
int cbuf;

int linenum;

int init_scan(char *filename){
	if ((fp = fopen(filename, "r")) == NULL) {
		error("error : file can not open");
	}
	cbuf = fgetc(fp);
}

int scan(void){
	if(cbuf == ' ' || cbuf == '\t'){
		cbuf = fgetc(fp);
		scan();
	}
	else if(isalpha(cbuf)){
		int i;
		char buf[MAXSTRSIZE] = {cbuf, '\0'};
		for(i = 1; isalnum(cbuf) || i < MAXSTRSIZE; i++){
			cbuf = getc(fp);
			buf[i] = cbuf;
		}
		buf[i] = '\0';

		for(i = 0; i < KEYWORDSIZE; i++){
			if(strcmp(buf, key[i]->keyword) == 0){
				
			}
		}
	}
	else if(isdigit(cbuf)){
		int i;
		char buf[MAXSTRSIZE] = {cbuf, '\0'};
		for(i = 1; isdigit(cbuf) || i < MAXSTRSIZE; i++){
			cbuf = getc(fp);
			buf[i] = cbuf;
		}
		buf[i] = '\0';
	}
}

int get_linenum(void){

}

void end_scan(void){

}