// Callback for errors when parsing the grammar
extern void yyerror(char* msg) {
  extern int yylineno;
  printf("Error in line %d: %s\n", yylineno, msg);
}

// Returns the next integer in the command line parameters (0 if invalid)
int read_next_value() {
  extern char** argv;
  extern int* argc;
  if (*argc == 0) return 0;
  int i = (int) strtol(argv[0], NULL, 10);
  argv++;
  *argc--;
  return i;
}

void print_var(char* s, int v) {
  printf("%s: %d\n", s, v);
}

// Final message to be displayed
void msg_quitting() {
  printf("Story finished.\n");
}
  
// Displayed on code quit
void msg_user_quit() {
  printf("We surrender!\n");
}

// Assorted error messages for various situations

void msg_error_not_enough_mem() {
  printf("Error: not enough memory.\n");
}
void msg_error_too_many_sentences() {
  printf("Error: too many sentences.\n");
}

void msg_error_there_are_no_sentences() {
  printf("Error: there are no sentences in this block.\n");
}

void msg_error_unknown_sentence() {
  printf("Error: unrecognized sentence.\n");
}

void msg_error_undefined_io() {
  printf("Error: there's no defined action for this I/O command.\n");
}

void msg_error_unknown_var(char* s) {
  printf("Error: I can't find %s (are you sure it's in the battlefield?).\n", s);
}
    
void msg_error_undefined_decl() {
  printf("Error: there's no defined action for this declaration.\n");
}
    
void msg_error_var_already_exists(char* s) {
  printf("Error: there's already someone called %s here.\n", s);
}
    
void msg_error_undefined_op() {
  printf("Error: there's no defined action for this arithmetic operation.\n");
}
    
void msg_error_divbyzero(char* s) {
  printf("Error: attempt to divide %s by zero.\n", s);
}
    
void msg_error_unknown_op() {
  printf("Error: unknown arithmetic operation.\n");
}
    
void msg_error_undefined_cond() {
  printf("Error: there's no defined action for this condition.\n");
}
    
void msg_error_undefined_if() {
  printf("Error: there's no defined action for this if branching.\n");
}
    
void msg_error_undefined_while() {
  printf("Error: there's no defined action for this while loop.\n");
}

void msg_error_too_much_garbage() {
  printf("Error: too much garbage declared, i can't take anymore!\n");
}

void msg_error_theres_no_garbage() {
  printf("Error: there's no garbage to clean up.\n");
}
