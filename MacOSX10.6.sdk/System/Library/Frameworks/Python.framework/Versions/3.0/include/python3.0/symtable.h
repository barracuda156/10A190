#ifndef Py_SYMTABLE_H
#define Py_SYMTABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/* XXX(ncoghlan): This is a weird mix of public names and interpreter internal
 *                names.
 */

typedef enum _block_type { FunctionBlock, ClassBlock, ModuleBlock }
    _Py_block_ty;

struct _symtable_entry;

struct symtable {
	const char *st_filename;        /* name of file being compiled */
	struct _symtable_entry *st_cur; /* current symbol table entry */
	struct _symtable_entry *st_top; /* symbol table entry for module */
	PyObject *st_blocks;            /* dict: map AST node addresses
	                                 *       to symbol table entries */
	PyObject *st_stack;             /* list: stack of namespace info */
	PyObject *st_global;            /* borrowed ref to st_top->st_symbols */
	int st_nblocks;                 /* number of blocks used */
	PyObject *st_private;           /* name of current class or NULL */
	PyFutureFeatures *st_future;    /* module's future features */
};

typedef struct _symtable_entry {
	PyObject_HEAD
	PyObject *ste_id;        /* int: key in ste_table->st_blocks */
	PyObject *ste_symbols;   /* dict: variable names to flags */
	PyObject *ste_name;      /* string: name of current block */
	PyObject *ste_varnames;  /* list of variable names */
	PyObject *ste_children;  /* list of child blocks */
	_Py_block_ty ste_type;   /* module, class, or function */
	int ste_unoptimized;     /* false if namespace is optimized */
	unsigned ste_nested : 1;      /* true if block is nested */
	unsigned ste_free : 1;        /* true if block has free variables */
	unsigned ste_child_free : 1;  /* true if a child block has free vars,
				         including free refs to globals */
	unsigned ste_generator : 1;   /* true if namespace is a generator */
	unsigned ste_varargs : 1;     /* true if block has varargs */
	unsigned ste_varkeywords : 1; /* true if block has varkeywords */
	unsigned ste_returns_value : 1;  /* true if namespace uses return with
	                                    an argument */
	int ste_lineno;          /* first line of block */
	int ste_opt_lineno;      /* lineno of last exec or import * */
	int ste_tmpname;         /* counter for listcomp temp vars */
	struct symtable *ste_table;
} PySTEntryObject;

PyAPI_DATA(PyTypeObject) PySTEntry_Type;

#define PySTEntry_Check(op) (Py_TYPE(op) == &PySTEntry_Type)

PyAPI_FUNC(int) PyST_GetScope(PySTEntryObject *, PyObject *);

PyAPI_FUNC(struct symtable *) PySymtable_Build(mod_ty, const char *, 
					      PyFutureFeatures *);
PyAPI_FUNC(PySTEntryObject *) PySymtable_Lookup(struct symtable *, void *);

PyAPI_FUNC(void) PySymtable_Free(struct symtable *);

/* Flags for def-use information */

#define DEF_GLOBAL 1           /* global stmt */
#define DEF_LOCAL 2            /* assignment in code block */
#define DEF_PARAM 2<<1         /* formal parameter */
#define DEF_NONLOCAL 2<<2      /* nonlocal stmt */
#define USE 2<<3               /* name is used */
#define DEF_STAR 2<<4          /* parameter is star arg */
#define DEF_DOUBLESTAR 2<<5    /* parameter is star-star arg */
#define DEF_INTUPLE 2<<6       /* name defined in tuple in parameters */
#define DEF_FREE 2<<7          /* name used but not defined in nested block */
#define DEF_FREE_GLOBAL 2<<8   /* free variable is actually implicit global */
#define DEF_FREE_CLASS 2<<9    /* free variable from class's method */
#define DEF_IMPORT 2<<10        /* assignment occurred via import */

#define DEF_BOUND (DEF_LOCAL | DEF_PARAM | DEF_IMPORT)

/* GLOBAL_EXPLICIT and GLOBAL_IMPLICIT are used internally by the symbol
   table.  GLOBAL is returned from PyST_GetScope() for either of them. 
   It is stored in ste_symbols at bits 12-15.
*/
#define SCOPE_OFFSET 11
#define SCOPE_MASK (DEF_GLOBAL | DEF_LOCAL | DEF_PARAM | DEF_NONLOCAL)

#define LOCAL 1
#define GLOBAL_EXPLICIT 2
#define GLOBAL_IMPLICIT 3
#define FREE 4
#define CELL 5

/* The following two names are used for the ste_unoptimized bit field */
#define OPT_IMPORT_STAR 1
#define OPT_TOPLEVEL 2  /* top-level names, including eval and exec */

#define GENERATOR 1
#define GENERATOR_EXPRESSION 2

#ifdef __cplusplus
}
#endif
#endif /* !Py_SYMTABLE_H */
