#include "pretty-printer.h"

typedef struct Type_{
	int stdtype; /* TINTEGER or TSTRING or TBOOLEAN */
	int array_size;
} Type;

typedef struct Lines_{
	int linenum;
	struct Lines_ *next;
} Lines;

typedef struct VarData_{
	int is_declaration;
	void* data;
	struct VarData_ *next;
}VarData;

typedef struct VarDecData_{
	char name[MAXSTRSIZE];
	Type type;
	int is_param;
	int line;
	int line_pretty_printed;
	struct VarData_* ref_head;
	struct VarData_* ref_tail;
}VarDecData;

typedef struct VarRefData_{
	struct VarData_* data;
	int line;
}VarRefData;

typedef struct ProcCallData_{
	struct ProcedureData_* proc_data;
	int line;
} ProcCallData;

typedef struct ConstData_{
	Type type;
	int line;
	int val;
} ConstData;

/*
typedef struct VarData_{
	char name[MAXSTRSIZE];
	Type type;
	int is_param;
	int defined_line;
	Lines *referenced_line_head;
	Lines *referenced_line_tail;
	struct VarRef_* ref_head;
	struct VarRef_* ref_tail;
	struct VarData_ *next;
} VarData;

typedef struct VarRef_{
	VarData* var_data;
	int linenum;
} VarRef;

*/

typedef struct ProcedureData_{
	char name[MAXSTRSIZE];
	int defined_line;
	int define_finished_line_pretty_printed;
	VarData *var_data_head;
	VarData *var_data_tail;
	struct ProcedureData_ *next;
} ProcData;

typedef struct ProgramData_{
	char name[MAXSTRSIZE];
	int defined_line;
	VarData *var_data_head;
	VarData *var_data_tail;
	ProcData *proc_data_head;
	ProcData *proc_data_tail;
} ProgData;


/* nodedata.c */
void fill_node_data_prepare(SyntaxTreeNode* node);
int fill_node_data(SyntaxTreeNode* node, SyntaxTreeNode* namespace, SyntaxTreeNode* global);
int check_type(SyntaxTreeNode* node);
