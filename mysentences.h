// This file deals with sentences and blocks of them.
// They behave basically like any other queue.


// Initialize. Never use a SB before calling this function!
void sb_makeempty(senblock *my) {
  my->start = 0;
  my->end = -1;
  my->size = 0;
}

// Create a new, empty block of sentences. Use this as a factory.
senblock* sb_new() {
  senblock* s = (senblock*) malloc(sizeof(senblock));
  sb_makeempty(s);
  return s;
}

// Check if there's any content within.
int sb_isempty(senblock *my) {
  return (my->size == 0);
}

// Check for free slots.
int sb_isfull(senblock *my) {
  return (my->end == MAXQUEUE);
}

// Add an element.
void sb_push(senblock *my, sentence* sy) {
  if (sb_isfull(my)) {
    msg_error_too_many_sentences();
    abort();
  }
  my->end = (my->end+1);
  my->size++;
  my->queue[my->end] = sy;
}

// Look at the top without removing it.
sentence* sb_peek(senblock *my) {
  if (sb_isempty(my)) {
    msg_error_there_are_no_sentences();
    abort();
  }
  return my->queue[my->start];
}

// Remove and return the top.
sentence* sb_pop(senblock *my) {
  sentence* sy = sb_peek(my);
  my->size--;
  my->start = (my->start+1);
  return sy;
}

// Resets the header to the beginning.
// WARNING: Be careful with the contents, the pointers may lead to unsafe content.
// Use with care and only you know what you're doing.
void sb_reset(senblock *my) {
  my->start = 0;
  my->size = (my->end+1 - my->start);
}

// Easy and quick way to create sentences
sentence* newsen(sentype type, void* comm) {
  sentence* s = (sentence*) malloc(sizeof(sentence));
  s->type = type;
  s->command = comm;
  return s;
}
