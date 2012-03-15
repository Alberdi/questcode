// The following functions deal with the top layer of execution of the compiler.
// The most important ones execute each sentence, according to its peculiarities.
// They also perform most of the error checking (accessing a non-declared variable, etc).

// Performs memory cleaning and other pre-exiting tasks
void abort() {
  extern senblock* c;
  extern context* ctxt;
  freesymboltable(ctxt->gc);
  msg_quitting();
  exit(0);
}

// Executes a whole block of sentences.
void execute(senblock* b) {
  sentence* s;
  while (b != NULL && !sb_isempty(b)) {
    s = sb_pop(b);
    switch (s->type) {
      case S_IO:    execute_io((iocommand*)s->command);       break;
      case S_DECL:  execute_decl((declcommand*)s->command);   break;
      case S_OP:    execute_op((opcommand*)s->command);       break;
      case S_IF:    execute_if((ifcommand*)s->command);       break;
      case S_WHILE: execute_while((whilecommand*)s->command); break;
      case S_QUIT:  execute_quit();                           break;
      default:
        msg_error_unknown_sentence();
        abort();
    }
  }
}

// Prints a var or reads its value from the parameters
void execute_io(iocommand* c) {
  if (c == NULL) {
    msg_error_undefined_io();
    abort();
  }
  var* v = lookup(c->var);
  if (v == NULL) {
    msg_error_unknown_var(c->var);
    abort();
  }
  if (c->type == IOOP_IN) {
    int value = read_next_value();
    assign_var(c->var, value);
  } else
    print_var(v->name, v->value);
}

// Creates a variable in this context
// Note: two vars can't share a name in the same context
// Vars in lower contexts shadow those in higher ones
void execute_decl(declcommand *c) {
  if (c == NULL) {
    msg_error_undefined_decl();
    abort();
  }
  if (exists_var(c->var)) {
    msg_error_var_already_exists(c->var);
    abort();
  }
  create_var(c->var, c->value);
}

// Performs an arithmetical operation on a var
void execute_op(opcommand *o) {
  int tmp, res;
  if (o == NULL) {
    msg_error_undefined_op();
    abort();
  }
  var* v = lookup(o->var1);
  if (v == NULL) {
    msg_error_unknown_var(o->var1);
    abort();
  }
  if (o->type == OP_VAR) {
    var* v2 = lookup(o->var2);
    if (v2 == NULL) {
      msg_error_unknown_var(o->var2);
      abort();
    }
    tmp = v2->value;
  } else // o->type = OP_IMM
    tmp = o->imm;
    
  switch (o->op) {
    case OP_ADD:   res = v->value + tmp; break;
    case OP_SUBS:  res = v->value - tmp; break;
    case OP_TIMES: res = v->value * tmp; break;
    case OP_EQ:    res = tmp;            break;
    case OP_DIV:
      if (tmp == 0) {
        msg_error_divbyzero(o->var1);
        abort();
      }
      res = v->value / tmp; break;
    default:
      msg_error_unknown_op();
      abort();
  }
  assign_var(o->var1, res);
}

// Checks the result of a boolean expression
int evaluate(condition* cd) {
  if (cd == NULL) {
    msg_error_undefined_cond();
    abort();
  }
  var* v = lookup(cd->var1);
  if (v == NULL) {
    msg_error_unknown_var(cd->var1);
    abort();
  }
  if (cd->type = CD_VAR) {
    var* v2 = lookup(cd->var2);
    if (v2 == NULL) {
      msg_error_unknown_var(cd->var2);
      abort();
    }
    return (v->value > v2->value);
  } else // o->type = OP_IMM
    return (v->value > cd->imm);
}

// Simulates a conditional branching
void execute_if(ifcommand* i) {
  if (i == NULL) {
    msg_error_undefined_if();
    abort();
  }
  push_context();
  if (evaluate(i->cond))
    execute(i->ifbranch);
  else
    execute(i->elsebranch);
  pop_context();
}

// Simulates a conditional loop
void execute_while(whilecommand* w) {
  sb_peek(w->block);
  if (w == NULL) {
    msg_error_undefined_while();
    abort();
  }
  push_context();
  while (evaluate(w->cond)) {
    execute(w->block);
    sb_reset(w->block); // back to the first sentence
  }
  pop_context();
}

// Terminates the program
void execute_quit() {
  msg_user_quit();
  abort();
}
