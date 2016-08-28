#include <stdlib.h>
#include "array_list.h"

ArrayList *array_list_new(void (*free_element)(void *element)){
    ArrayList *array = malloc(sizeof(*array));
    array->length = 0;
    array->capacity = 16;
    array->data = malloc(array->capacity * sizeof(*(array->data)));
    int i;
    for(i = 0;i < array->capacity;i++){
        array->data[i] = NULL;
    }
    array->free_element = free_element;
	return array;
}

void array_list_free(void *data){
    if (data == NULL) return;

    ArrayList *array = (ArrayList *)data;
    int i;
    for(i = 0;i < array->length;i++){
        array->free_element(array->data[i]);
    }
    free(array->data);
    free(array);
}

void array_list_push(ArrayList *array, void *data){
    if(array->length == array->capacity){
        int i = array->capacity;
        array->capacity = 2 * array->capacity;
        array->data = realloc(array->data,array->capacity * sizeof(*(array->data)));
        for(;i < array->capacity;i++){
            array->data[i] = NULL;
        }
    }
    array->data[array->length] = data;
    array->length += 1;
}

void array_list_remove_at(ArrayList *array, int index){
    if(index >= array->length) return;

    int i;
    array->free_element(array->data[index]);
    for(i = index + 1;i < array->length;i++){
        array->data[i - 1] = array->data[i];
    }
    array->data[array->length - 1] = NULL;
    array->length = array->length - 1;
}

void array_list_grow(ArrayList *array){
    int i = array->capacity;
    array->capacity = 2 * array->capacity;
    array->data = realloc(array->data,array->capacity * sizeof(*(array->data)));
    for(;i < array->capacity;i++){
        array->data[i] = NULL;
    }
}

void array_list_grow_to_capactiy(ArrayList *array, size_t size){
    int i = array->capacity;
    array->capacity = size;
    array->data = realloc(array->data, array->capacity * sizeof(*(array->data)));
    for(;i < array->capacity;i++){
        array->data[i] = NULL;
    }
}

void array_list_shrink_to_fit(ArrayList *array){
    int i = array->capacity;
    array->capacity = array->length;
    array->data = realloc(array->data,array->capacity * sizeof(*(array->data)));
}

#define IMPL_ARRAY_LIST(T, name, default_value) \
ArrayList_##name *array_list_new_##name##(){ \
    ArrayList_##name *array = malloc(sizeof(*array)); \
    array->length = 0; \
    array->capacity = 16; \
    array->data = malloc(array->capacity * sizeof(*(array->data))); \
    int i; \
    for(i = 0; i < array->capacity;i++){ \
        array->data[i] = default_value; \
    } \
    return array; \
} \
\
void array_list_free_##name##(void *data){ \
    if(data == NULL) return; \
\
    ArrayList_##name *array = (ArrayList_##name *)data; \
    free(array->data); \
    free(array); \
} \
\
void array_list_push_##name##(ArrayList_##name *array, T data){ \
    if(array->length == array->capacity){ \
        int i = array->capacity; \
        array->capacity = 2 * array->capacity; \
        array->data = realloc(array->data, array->capacity * sizeof(*(array->data))); \
        for(;i < array->capacity;i++){ \
            array->data[i] = default_value; \
        } \
    } \
    array->data[array->length] = data; \
    array->length += 1; \
} \
\
void array_list_remove_at_##name##(ArrayList_##name *array, int index){ \
    if(index >= array->length) return; \
\
    int i; \
    for(i = index + 1;i < array->length;i++){ \
        array->data[i - 1] = array->data[i]; \
    } \
    array->data[array->length - 1] = default_value; \
    array->length = array->length - 1; \
} \
\
void array_list_grow_##name##(ArrayList_##name *array){ \
    int i = array->capacity; \
    array->capacity = 2 * array->capacity; \
    array->data = realloc(array->data,array->capacity * sizeof(*(array->data))); \
    for(;i < array->capacity;i++){ \
        array->data[i] = default_value; \
    } \
} \
\
void array_list_grow_to_capactiy_##name##(ArrayList_##name *array, size_t size) { \
    int i = array->capacity; \
    array->capacity = size; \
    array->data = realloc(array->data, array->capacity * sizeof(*(array->data))); \
    for(;i < array->capacity;i++){ \
        array->data[i] = default_value; \
    } \
} \
\
void array_list_shrink_to_fit_##name##(ArrayList_##name *array) { \
    int i = array->capacity; \
    array->capacity = array->length; \
    array->data = realloc(array->data,array->capacity * sizeof(*(array->data))); \
}

IMPL_ARRAY_LIST(float, f, 0.0)
IMPL_ARRAY_LIST(int, i, 0)
IMPL_ARRAY_LIST(short, s, 0)
IMPL_ARRAY_LIST(Mat4, m4, MAT4_IDENT)
IMPL_ARRAY_LIST(TerrainVertex, tv, TERRAIN_VERTEX_DEFAULT)
IMPL_ARRAY_LIST(Vec3, v3, VEC3_ZERO)
