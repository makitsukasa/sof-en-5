#include "pretty-printer.h"

typedef struct Type_{
	int stdtype;
	int array_size;
} Type;

typedef struct Lines_{
	int linenum;
	struct Lines_ *next;
} Lines;

typedef struct VarData_{
	char name[MAXSTRSIZE];
	Type type;
	int is_param;
	int defined_line;
	Lines *referenced_line_head;
	Lines *referenced_line_tail;
	struct VarData_ *next;
} VarData;

typedef struct ProcedureData_{
	char name[MAXSTRSIZE];
	int is_main_routine;
	int defined_line;
	VarData *var_data_head;
	VarData *var_data_tail;
	struct ProcedureData_ *next;
} ProcData;


#define CHECKTYPERESULT_NOTMATCH 0
#define CHECKTYPERESULT_MATCH 1
#define CHECKTYPERESULT_IGNORED 2
void list_name(SyntaxTreeNode *node);
Type check_type(SyntaxTreeNode *node);