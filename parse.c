#if 0

#include "pretty-printer.h"
#include <stdarg.h>

#endif

/* http://marycore.jp/prog/c-lang/variable-argument/ */

/*
int series(int count, ...){
	int i;
	int ret_val = 1;
	va_list s;
	va_start(s, count);
	for (i = 0; i < count; i++) {
		if(!va_arg(s, int)){
			ret_val = 0;
		}
	}
	printf("series %d returns %d\n", count, ret_val);
	return ret_val;
}

int parallel(int count, ...){
	int i;
	int ret_val = 0;
	va_list s;
	va_start(s, count);
	for (i = 0; i < count; i++) {
		if(va_arg(s, int)){
			ret_val = 1;
		}
	}
	printf("parallel %d returns %d\n", count, ret_val);
	return ret_val;
}

int zero_or_more(int s){
	printf("zero_or_more\n");
	return 0;
}

int zero_or_one(int s){
	printf("zero_or_one\n");
	return 0;
}

int parse(int syntax){
	switch(syntax){
	case SPROGRAM:
		return series(5, parse(TPROGRAM), parse(TNAME), parse(TSEMI), parse(SBLOCK), parse(TCOLON));

	case SBLOCK:
		return series(2, zero_or_more(parallel(2, parse(SVARDEC), parse(SSUBPROGDEC))), parse(SCOMPSTAT));

		return series(2, PSTILL ,zero_or_more())

	default:
		return 1;
	}
	return 0;
}
*/

