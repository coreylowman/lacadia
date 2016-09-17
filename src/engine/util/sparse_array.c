#include <stdlib.h>
#include "sparse_array.h"

static SparseArrayStack stack_init() {
  SparseArrayStack self;
  self.capacity = 16;
  self.length = 0;
  self.data = malloc(self.capacity * sizeof(*(self.data)));
  return self;
}

static int stack_pop(SparseArrayStack *self) {
  return self->data[--self->length];
}

static void stack_push(SparseArrayStack *self, int n) {
  if (self->length == self->capacity) {
    self->capacity *= 2;
    self->data = realloc(self->data, self->capacity * sizeof(*(self->data)));
  }
  self->data[self->length++] = n;
}

static void stack_free(SparseArrayStack *self) { free(self->data); }

SparseArray *sparse_array_new(void (*elem_free)(void *p)) {
  SparseArray *self = malloc(sizeof(*self));
  self->capacity = 16;
  self->length = 0;
  self->num_elements = 0;
  self->data = malloc(self->capacity * sizeof(*(self->data)));

  self->elem_free = elem_free;

  self->available_indices = stack_init();

  return self;
}

void sparse_array_free(SparseArray *self) {
  int i;
  if (self->elem_free != NULL) {
    for (i = 0; i < self->length; i++) {
      if (self->data[i] == NULL)
        continue;
      self->elem_free(self->data[i]);
    }
  } else {
    for (i = 0; i < self->length; i++) {
      self->data[i] = NULL;
    }
  }
  free(self->data);
  stack_free(&self->available_indices);
  free(self);
}

int sparse_array_add(SparseArray *self, void *elem) {
  self->num_elements++;
  if (self->available_indices.length > 0) {
    int index = stack_pop(&self->available_indices);
    self->data[index] = elem;
    return index;
  } else {
    if (self->length == self->capacity) {
      self->capacity *= 2;
      self->data = realloc(self->data, self->capacity * sizeof(*(self->data)));
    }
    self->data[self->length++] = elem;
    return self->length - 1;
  }
}

void sparse_array_remove_at(SparseArray *self, int i) {
  if (self->data[i] == NULL)
    return;
  self->num_elements--;
  if (self->elem_free != NULL)
    self->elem_free(self->data[i]);
  self->data[i] = NULL;
  stack_push(&self->available_indices, i);
}
