// Due to the peculiarities of the hashed symbol table
// it's required to have a garbage collector keeping track
// of all malloc addresses to prevent memory leaks.


void msg_error_theres_no_garbage();
void msg_error_too_much_garbage();


void gc_makeempty(gcollector *my) {
  my->start = 0;
  my->end = -1;
  my->size = 0;
}

int gc_isempty(gcollector *my) {
  return (my->size == 0);
}

int gc_isfull(gcollector *my) {
  return (my->size == MAXQUEUE);
}

void gc_push(gcollector *my, void* sy) {
  if (gc_isfull(my)) {
    msg_error_too_much_garbage();
    exit(EXIT_FAILURE);
  }
  my->end = (my->end+1) % MAXQUEUE;
  my->size++;
  my->queue[my->end] = sy;
}

void* gc_peek(gcollector *my) {
  if (gc_isempty(my)) {
    msg_error_theres_no_garbage();
    exit(EXIT_FAILURE);
  }
  return my->queue[my->start];
}

void* gc_pop(gcollector *my) {
  void* sy = gc_peek(my);
  my->size--;
  my->start = (my->start+1) % MAXQUEUE;
  return sy;
}
