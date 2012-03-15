// The following functions create the commands which conform each sentence.
// Beware: they return the pointer as void* !
// (this is done to treat them all as equals)
// Note: Sentences keep track of the type of each command.

// Arithmetical operations.
void* opcomm(char* var1, char* var2, int imm, operation op, optype type) {
  opcommand* o = (opcommand*) malloc(sizeof(opcommand));
  o->var1 = var1;
  o->type = type;
  o->imm = imm;
  o->var2 = var2;
  o->op = op;
  return (void*) o;
}

// Input/output.
void* iocomm(char* var, iooptype type) {
  iocommand* c = (iocommand*) malloc(sizeof(iocommand));
  c->var = var;
  c->type = type;
  return (void*) c;
}

// Declaration of variables (initialization).
void* declcomm(char* var, int val) {
  declcommand* c = (declcommand*) malloc(sizeof(declcommand));
  c->var = var;
  c->value = val;
  return (void*) c;
}

// Conditions (used within ifs and whiles).
void* condcomm(char* var1, char* var2, int imm, condtype type) {
  condition* cd = (condition*) malloc(sizeof(condition));
  cd->type = type;
  cd->imm = imm;
  cd->var1 = var1;
  cd->var2 = var2;
  return (void*) cd;
}

// If branching.
void* ifcomm(condition* cd, senblock* b1, senblock* b2) {
  ifcommand* c = (ifcommand*) malloc(sizeof(ifcommand));
  c->cond = cd;
  c->ifbranch = b1;
  c->elsebranch = b2;
  return (void*) c;
}

// While loops.
void* whilecomm(condition* cd, senblock* b) {
  whilecommand* w = (whilecommand*) malloc(sizeof(whilecommand));
  w->cond = cd;
  w->block = b;
  return (void*) w;
}
