#include <stdlib.h>
#include "array_list.h"

extern Mat4 MAT4_IDENT;

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

ArrayList_f *array_list_new_f(){
    ArrayList_f *array = malloc(sizeof(*array));
    array->length = 0;
    array->capacity = 16;
    array->data = malloc(array->capacity * sizeof(*(array->data)));
    int i;
    for(i = 0;i < array->capacity;i++){
        array->data[i] = 0;
    }
    return array;
}

ArrayList_i *array_list_new_i(){
    ArrayList_i *array = malloc(sizeof(*array));
    array->length = 0;
    array->capacity = 16;
    array->data = malloc(array->capacity * sizeof(*(array->data)));
    int i;
    for(i = 0;i < array->capacity;i++){
        array->data[i] = 0;
    }
    return array;
}

ArrayList_s *array_list_new_s(){
    ArrayList_s *array = malloc(sizeof(*array));
    array->length = 0;
    array->capacity = 16;
    array->data = malloc(array->capacity * sizeof(*(array->data)));
    int i;
    for(i = 0;i < array->capacity;i++){
        array->data[i] = 0;
    }
    return array;
}

ArrayList_m4 *array_list_new_m4(){
    ArrayList_m4 *array = malloc(sizeof(*array));
    array->length = 0;
    array->capacity = 16;
    array->data = malloc(array->capacity * sizeof(*(array->data)));
    int i;
    for(i = 0;i < array->capacity;i++){
        array->data[i] = MAT4_IDENT;
    }
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

void array_list_free_f(void *data){
    if (data == NULL) return;

    ArrayList_f *array = (ArrayList_f *)data;
    free(array->data);
    free(array);
}

void array_list_free_i(void *data){
    if (data == NULL) return;

    ArrayList_i *array = (ArrayList_i *)data;
    free(array->data);
    free(array);
}

void array_list_free_s(void *data){
    if (data == NULL) return;

    ArrayList_s *array = (ArrayList_s *)data;
    free(array->data);
    free(array);
}

void array_list_free_m4(void *data){
    if (data == NULL) return;

    ArrayList_m4 *array = (ArrayList_m4 *)data;
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

void array_list_push_f(ArrayList_f *array, float data){
    if(array->length == array->capacity){
        int i = array->capacity;
        array->capacity = 2 * array->capacity;
        array->data = realloc(array->data,array->capacity * sizeof(*(array->data)));
        for(;i < array->capacity;i++){
            array->data[i] = 0;
        }
    }
    array->data[array->length] = data;
    array->length += 1;
}

void array_list_push_i(ArrayList_i *array, int data){
    if(array->length == array->capacity){
        int i = array->capacity;
        array->capacity = 2 * array->capacity;
        array->data = realloc(array->data,array->capacity * sizeof(*(array->data)));
        for(;i < array->capacity;i++){
            array->data[i] = 0;
        }
    }
    array->data[array->length] = data;
    array->length += 1;
}

void array_list_push_s(ArrayList_s *array, short data){
    if(array->length == array->capacity){
        int i = array->capacity;
        array->capacity = 2 * array->capacity;
        array->data = realloc(array->data,array->capacity * sizeof(*(array->data)));
        for(;i < array->capacity;i++){
            array->data[i] = 0;
        }
    }
    array->data[array->length] = data;
    array->length += 1;
}

void array_list_push_m4(ArrayList_m4 *array, Mat4 data){
    if(array->length == array->capacity){
        int i = array->capacity;
        array->capacity = 2 * array->capacity;
        array->data = realloc(array->data,array->capacity * sizeof(*(array->data)));
        for(;i < array->capacity;i++){
            array->data[i] = MAT4_IDENT;
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

void array_list_remove_at_f(ArrayList_f *array, int index){
    if(index >= array->length) return;

    int i;
    for(i = index + 1;i < array->length;i++){
        array->data[i - 1] = array->data[i];
    }
    array->data[array->length - 1] = 0;
    array->length = array->length - 1;
}

void array_list_remove_at_i(ArrayList_i *array, int index){
    if(index >= array->length) return;

    int i;
    for(i = index + 1;i < array->length;i++){
        array->data[i - 1] = array->data[i];
    }
    array->data[array->length - 1] = 0;
    array->length = array->length - 1;
}

void array_list_remove_at_s(ArrayList_s *array, int index){
    if(index >= array->length) return;

    int i;
    for(i = index + 1;i < array->length;i++){
        array->data[i - 1] = array->data[i];
    }
    array->data[array->length - 1] = 0;
    array->length = array->length - 1;
}

void array_list_remove_at_m4(ArrayList_m4 *array, int index){
    if(index >= array->length) return;

    int i;
    for(i = index + 1;i < array->length;i++){
        array->data[i - 1] = array->data[i];
    }
    array->data[array->length - 1] = MAT4_IDENT;
    array->length = array->length - 1;
}

