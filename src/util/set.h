#ifndef SET_H
#define SET_H

#include <stdlib.h>

typedef struct {
  int *data;
  int length;
  int capacity;
} SetStack;

// more efficient removal than array list due to Setstack of available
// indices...
// more overhead though
typedef struct {
  void **data;
  int length;
  int capacity;
  int num_elements;

  void (*elem_free)(void *p);

  // a list of indices that are available to use
  SetStack *available_indices;
} Set;

Set *set_new(void (*elem_free)(void *p));
void set_free(Set *self);

int set_add(Set *self, void *elem);
void set_remove_at(Set *self, int i);

#endif
