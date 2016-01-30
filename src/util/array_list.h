#ifndef ARRAY_LIST_H_
#define ARRAY_LIST_H_

#include <stdlib.h>
#include "mat4.h"

typedef struct {
    void **data;
    void (*free_element)(void *element);
    int capacity;
    int length;
} ArrayList;

typedef struct {
    float *data;

    int capacity;
    int length;
} ArrayList_f;

typedef struct {
    int *data;

    int capacity;
    int length;
} ArrayList_i;

typedef struct {
    short *data;

    int capacity;
    int length;
} ArrayList_s;

typedef struct {
    Mat4 *data;

    int capacity;
    int length;
} ArrayList_m4;

ArrayList *array_list_new(void (*free_element)(void *element));
ArrayList_f *array_list_new_f();
ArrayList_i *array_list_new_i();
ArrayList_s *array_list_new_s();
ArrayList_m4 *array_list_new_m4();

void array_list_free(void *data);
void array_list_free_f(void *data);
void array_list_free_i(void *data);
void array_list_free_s(void *data);
void array_list_free_m4(void *data);

void array_list_push(ArrayList *array, void *data);
void array_list_push_f(ArrayList_f *array, float data);
void array_list_push_i(ArrayList_i *array, int data);
void array_list_push_s(ArrayList_s *array, short data);
void array_list_push_m4(ArrayList_m4 *array, Mat4 data);

void array_list_remove_at(ArrayList *array, int index);
void array_list_remove_at_f(ArrayList_f *array, int index);
void array_list_remove_at_i(ArrayList_i *array, int index);
void array_list_remove_at_s(ArrayList_s *array, int index);
void array_list_remove_at_m4(ArrayList_m4 *array, int index);

#endif