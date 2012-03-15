// This file deals with all things about the symbol table and the contexts.
// Each context contains a symbol table, and they are linked
// (so a var is looked for in a broader scope each time).

// Frees the memory used by a symbol, its var, and all those linked by it
void freesymboltable(gcollector* gc) {
  void* p;
  while (!gc_isempty(gc))
    free(gc_pop(gc));
}

// Adds a new context.
void push_context() {
  extern context* ctxt;
  context* n = malloc(sizeof(context));
  n->next = ctxt;
  n->gc = malloc(sizeof(gcollector));
  gc_makeempty(n->gc);
  ctxt = n;  
}

// Eliminates the latest context.
void pop_context() {
  extern context* ctxt;
  context *tmp = ctxt->next;
  freesymboltable(ctxt->gc);
  ctxt = tmp;
}

void init_context() {
  extern context* ctxt;
  push_context();
  ctxt->next = NULL;
}

// Calculates the hash for each var.
unsigned long hash(char *str) {
  unsigned long hash = 0;
  int c;
  while (c = *str++)
    hash = hash * MULT + c;
  return hash % NHASH;
}

// Looks for a var in one context.
var* c_lookup(context* ctxt, char *s) {
  symbol *sy;
  unsigned long h = hash(s);
  for (sy = ctxt->sytable[h]; sy != NULL && sy->vble != NULL; sy = sy->next)
    if (strcmp(s, sy->vble->name) == 0)
      return sy->vble;
  return NULL;
}

// Looks for a var in all contexts (outwards).
var* lookup(char* s) {
  extern context* ctxt;
  context* c;
  var* v;
  for (c = ctxt; c != NULL; c = c->next)
    if ((v = c_lookup(c, s)) != NULL)
      return v;
  return NULL;
}

// Assigns a value to a var (creates if nonexistant)
// Note: "deep" flag indicates if it should operate globally
// or just in the nearest context
void assign_value(char *s, int value, int deep) {
  extern context* ctxt;
  var* v;
  if (deep)  v = lookup(s);
  else       v = c_lookup(ctxt, s);
  
  if (v != NULL)
    v->value = value;

  else {
    v = (var *) malloc(sizeof(var));
    symbol* sy = (symbol*) malloc(sizeof(symbol));
    if (v == NULL || sy == NULL) {
      msg_error_not_enough_mem();
      abort();
    }
    gc_push(ctxt->gc, v);
    gc_push(ctxt->gc, sy);

    strncpy(v->name, s, strlen(s));
    v->value = value;
    sy->vble = v;

    unsigned long h = hash(s);
    sy->next = ctxt->sytable[h];
    ctxt->sytable[h] = sy;
  }
}

// Looks for the nearest var with that name and changes its value.
// If there's none, it creates one in the nearest context.
void assign_var(char* s, int value) {
  assign_value(s, value, 1);
}
// Ensures a var with that name and value exists in the nearest context.
void create_var(char* s, int value) {
  assign_value(s, value, 0);
}

// Checks if there's a var with that name in the nearest context.
int exists_var(char* s) {
  extern context* ctxt;
  return (c_lookup(ctxt,s) != NULL);
}
