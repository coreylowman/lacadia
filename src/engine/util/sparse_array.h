#ifndef SPARSE_ARRAY_H
#define SPARSE_ARRAY_H

#define SPARSE_ARRAY_FOREACH(type, name, arr, block)                           \
  {                                                                            \
    type name;                                                                 \
    int i;                                                                     \
    for (i = 0; i < arr->length; i++) {                                        \
      if (arr->data[i] == NULL)                                                \
        continue;                                                              \
      name = arr->data[i];                                                     \
      block                                                                    \
    }                                                                          \
  }

typedef struct {
  int *data;
  int length;
  int capacity;
} SparseArrayStack;

// more efficient removal than array list due to SparseArraystack of available
// indices...
// more overhead though
typedef struct {
  void **data;
  int length;
  int capacity;
  int num_elements;

  void (*elem_free)(void *p);

  // a list of indices that are available to use
  SparseArrayStack available_indices;
} SparseArray;

SparseArray *sparse_array_new(void (*elem_free)(void *p));
void sparse_array_free(SparseArray *self);

int sparse_array_add(SparseArray *self, void *elem);
void sparse_array_remove_at(SparseArray *self, int i);

#endif
