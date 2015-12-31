#include "set.h"

static SetStack *set_stack_new(){
    SetStack *self = malloc(sizeof(*self));
    self->capacity = 16;
    self->length = 0;
    self->data = malloc(self->capacity * sizeof(*(self->data)));
    return self;
}

static size_t set_stack_pop(SetStack *self){
    return self->data[--self->length];
}
 
static void set_stack_push(SetStack *self, size_t n){
    if(self->length == self->capacity){
        self->capacity *= 2;
        self->data = realloc(self->data, self->capacity * sizeof(*(self->data)));
    }
    self->data[self->length++] = n;
}
 
static void set_stack_free(SetStack *self){
    free(self->data);
    free(self);
}

Set *set_new(void (*elem_free)(void *p)){
    Set *self = malloc(sizeof(*self));
    self->capacity = 16;
    self->length = 0;
    self->num_elements = 0;
    self->data = malloc(self->capacity * sizeof(*(self->data)));
 
    self->elem_free = elem_free;
 
    self->available_indices = set_stack_new();
 
    return self;
}

void set_free(Set *self){
    size_t i;
    for(i = 0;i < self->length;i++){
        self->elem_free(self->data[i]);
    }
    free(self->data);
    set_stack_free(self->available_indices);
    free(self);
}
 
void set_add(Set *self, void *elem){
    self->num_elements++;
    if(self->available_indices->length > 0){
        self->data[set_stack_pop(self->available_indices)] = elem;
    }else{
        if(self->length == self->capacity){
            self->capacity *= 2;
            self->data = realloc(self->data, self->capacity * sizeof(*(self->data)));
        }
        self->data[self->length++] = elem;
    }
}
 
void set_remove_at(Set *self, size_t i){
    if(self->data[i] == NULL) return;
    self->num_elements--;
    self->elem_free(self->data[i]);
    self->data[i] = NULL;
    set_stack_push(self->available_indices, i);
}
