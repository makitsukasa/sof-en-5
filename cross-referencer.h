#include "pretty-printer.h"
#include <stddef.h>

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
	struct ProcedureData_* namespace;
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
	struct ProcCallData_* next;
} ProcCallData;

typedef struct ConstData_{
	Type type;
	int line;
	int val;
} ConstData;

typedef struct ProcedureData_{
	char name[MAXSTRSIZE];
	int defined_line;
	int define_finished_line_pretty_printed;
	VarData *var_data_head;
	VarData *var_data_tail;
	ProcCallData *ref_head;
	ProcCallData *ref_tail;
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

typedef struct CrossRefRecord_{
	int is_head;
	char namespace[MAXSTRSIZE];
	char name[MAXSTRSIZE];
	char type[MAXSTRSIZE];
	char def[MAXSTRSIZE];
	char ref[MAXSTRSIZE];
	struct CrossRefRecord_* next;
} CrossRefRecord;

extern void* mem_alloc(size_t size);

/* nodedata.c */
extern void fill_node_data_prepare(SyntaxTreeNode* node);
extern int fill_node_data(SyntaxTreeNode* node, SyntaxTreeNode* namespace, SyntaxTreeNode* global);
extern int check_type(SyntaxTreeNode* node);
