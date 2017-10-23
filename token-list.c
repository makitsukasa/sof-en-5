#include "token-list.h"

/* keyword list */
KEY keyword[KEYWORDSIZE] = {
	{"and", 	TAND	},
	{"array",	TARRAY	},
	{"begin",	TBEGIN	},
	{"boolean",	TBOOLEAN},
	{"break",	TBREAK  },
	{"call",	TCALL	},
	{"char",	TCHAR	},
	{"div",		TDIV	},
	{"do",		TDO	},
	{"else",	TELSE	},
	{"end",		TEND	},
	{"false",	TFALSE	},
	{"if",		TIF	},
	{"integer",	TINTEGER},
	{"not",		TNOT	},
	{"of",		TOF	},
	{"or",		TOR	},
	{"procedure", TPROCEDURE},
	{"program",	TPROGRAM},
	{"read",	TREAD	},
	{"readln",	TREADLN },
	{"return", 	TRETURN },
	{"then",	TTHEN	},
	{"true",	TTRUE	},
	{"var",		TVAR	},
	{"while",	TWHILE	},
	{"write",	TWRITE  },
	{"writeln",	TWRITELN}
};

/* symbol list */
KEY symbol1[SYMBOL1SIZE] = {
	{"+", 	TPLUS	},
	{"-",	TMINUS	},
	{"*",	TSTAR	},
	{"=",	TEQUAL	},
	{"<",	TLE	},
	{">",	TGR	},
	{"(",	TLPAREN	},
	{")",	TRPAREN	},
	{"[",	TLSQPAREN	},
	{"]",	TRSQPAREN	},
	{".",	TDOT	},
	{",",	TCOMMA	},
	{":",	TCOLON	},
	{";",	TSEMI	}
};
KEY symbol2[SYMBOL2SIZE] = {
	{"<>",	TNOTEQ	},
	{"<=",	TLEEQ	},
	{">=",	TGREQ	},
	{":=",	TASSIGN	},
};

/* Token counter */
int numtoken[NUMOFTOKEN+1];

/* string of each token */
char *tokenstr[NUMOFTOKEN+1] = {
	"",
	"NAME", "program", "var", "array", "of", "begin", "end", "if", "then",
	 "else", "procedure", "return", "call", "while", "do", "not", "or", 
	"div", "and", "char", "integer", "boolean", "readln", "writeln", "true",
	 "false", "NUMBER", "STRING", "+", "-", "*", "=", "<>", "<", "<=", ">", 
	">=", "(", ")", "[", "]", ":=", ".", ",", ":", ";", "read","write", "break"
};

int main(int nc, char *np[]) {
	int token, i;

	if(nc < 2) {
		printf("File name id not given.\n");
		return 0;
	}
	if(init_scan(np[1]) < 0) {
		printf("File %s can not open.\n", np[1]);
		return 0;
	}
	/* initialize numtoken[] */
	for(i = 0; i <= NUMOFTOKEN; i++){
		numtoken[i] = 0;
	}

	while((token = scan()) >= 0) {
		/* count token */
		numtoken[token]++;
	}
	end_scan();
	/* output count */
	for(i = 1; i <= NUMOFTOKEN; i++){
		if(numtoken[i] <= 0) continue;
		printf("%s\t%d\n", tokenstr[i], numtoken[i]);
	}

	return 0;
}

void error(int linenum, char *mes) {
	fprintf(stderr, "\nline %d ERROR: %s\n", linenum, mes);
	end_scan();
	exit(-1);
}