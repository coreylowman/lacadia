#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include <stdlib.h>
#include "mat4.h"
#include "terrain.h"

#define DEFINE_ARRAY_LIST(T, name) \
typedef struct { \
    T *data; \
    int capacity; \
    int length; \
} ArrayList_ ## name; \
ArrayList_##name *array_list_new_##name##(); \
void array_list_free_##name##(void *data); \
void array_list_push_##name##(ArrayList_##name *array, T data); \
void array_list_remove_at_##name##(ArrayList_##name *array, int index); \
void array_list_grow_##name##(ArrayList_##name *array); \
void array_list_grow_to_capacity_##name##(ArrayList_##name *array, size_t size); \
void array_list_shrink_to_fit_##name##(ArrayList_##name *array);

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
void array_list_grow(ArrayList *array);
void array_list_grow_to_capactiy(ArrayList *array, size_t size);
void array_list_shrink_to_fit(ArrayList *array);

DEFINE_ARRAY_LIST(float, f)
DEFINE_ARRAY_LIST(int, i)
DEFINE_ARRAY_LIST(short, s)
DEFINE_ARRAY_LIST(Mat4, m4)
DEFINE_ARRAY_LIST(TerrainVertex, tv)
DEFINE_ARRAY_LIST(Vec3, v3)

#endif