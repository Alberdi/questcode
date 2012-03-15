#define MAXLEN 1024
#define MAXQUEUE 1024
#define NHASH 5381
#define MULT 33

// This file keeps record of the user-defined datatypes
// used throughout the compiler.

// The compiler recognizes six sentences.
typedef enum {
  S_IO, S_DECL, S_OP, S_IF, S_WHILE, S_QUIT
} sentype;

// Whether the condition checks a var against a var or an immediate.
typedef enum { CD_VAR, CD_IMM } condtype;

// Whether the operation involves a var and a var or a var and an immediate.
typedef enum { OP_VAR, OP_IMM } optype;

// The operation may be one of five kinds.
typedef enum {
  OP_ADD, OP_SUBS, OP_TIMES, OP_DIV, OP_EQ
} operation;

// Either input or ouput.
typedef enum { IOOP_IN, IOOP_OUT } iooptype;

// The standard model for a code variable.
typedef struct svar {
  char name[MAXLEN];
  int value;
} var;

// The garbage collector keeps track of used addresses
typedef struct {
  int start, end, size;
  void* queue[MAXQUEUE];
} gcollector;

// The symbol is used to link variables in the hashed symbol table.
typedef struct ssymbol {
  var* vble;
  struct ssymbol* next;
} symbol;

// Each block of code defines a context (a symbol table).
// They delegate upwards in a chain of responsibility.
typedef struct scontext {
  symbol* sytable[NHASH];
  gcollector* gc;
  struct scontext* next;
} context;
 
// A sentence is translated to a command. Due to casting, we need to know its type.
typedef struct {
  sentype type;
  void* command;
} sentence;

// A block of sentences is essentially a queue.
typedef struct {
  int start, end, size;
  sentence* queue[MAXQUEUE];
} senblock;

// The var to use, and whether to read or to print it.
typedef struct {
  char* var;
  iooptype type;
} iocommand;

// The var to initialize and its default value.
typedef struct {
  char* var;
  int value;
} declcommand;

// As an operation may involve vars or imms, it stores both to reduce the number of structs.
typedef struct {
  optype type;
  char* var1;
  int imm;
  char* var2;
  operation op;
} opcommand;

// The condition may check a var against another var or an immediate.
typedef struct {
  condtype type;
  char* var1;
  int imm;
  char* var2;
} condition;

// The if stores its condition and the blocks of code of both branches.
typedef struct {
  condition* cond;
  senblock* ifbranch;
  senblock* elsebranch;
} ifcommand;

// Similarly, a while stores its condition and its code block.
typedef struct {
  condition* cond;
  senblock* block;
} whilecommand;

