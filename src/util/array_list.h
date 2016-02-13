#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include <stdlib.h>
#include "mat4.h"

#define DEFINE_ARRAY_LIST(T, name) \
typedef struct { \
    T *data; \
    int capacity; \
    int length; \
} ArrayList_ ## name; \
ArrayList_##name *array_list_new_##name##(); \
void array_list_free_##name##(void *data); \
void array_list_push_##name##(ArrayList_##name *array, T data); \
void array_list_remove_at_##name##(ArrayList_##name *array, int index);

// array list of void pointers... needs a free element function pointer
typedef struct {
    void **data;
    void (*free_element)(void *element);
    int capacity;
    int length;
} ArrayList;

ArrayList *array_list_new(void (*free_element)(void *element));
void array_list_free(void *data);
void array_list_push(ArrayList *array, void *data);
void array_list_remove_at(ArrayList *array, int index);

DEFINE_ARRAY_LIST(float, f)
DEFINE_ARRAY_LIST(int, i)
DEFINE_ARRAY_LIST(short, s)
DEFINE_ARRAY_LIST(Mat4, m4)

#endif