typedef struct Type_{
	int stdtype;
	int array_size;
} Type;

typedef struct ParamType{
	Type type;
	struct ParamType* next;
}

typedef struct Lines_{
	int linenum;
	struct Lines_ *next;
} Lines;

typedef struct VarData_{
	char name[MAXSTRSIZE];
	Type type;
	int is_param;
	int defined_line;
	Lines* referenced_lines;
	struct VarData_* next;
} VarData;

typedef struct ProcedureData_{
	char proc_name[MAXSTRSIZE];
	int proc_defined_line;
	ParamType* param_types;
	VarData* var_data_head;
	VarData* var_data_tail;
} ProcData;
