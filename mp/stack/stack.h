#ifndef STACK_H
#define STACK_H

#include <stddef.h>

typedef struct stack_ {
    void *data;
    size_t size;      
    size_t capacity;  
    size_t element_size;
} Stack;

int stack_init(Stack *s, size_t element_size);
int stack_clear(Stack *s);
int stack_destroy(Stack *s);

int stack_push(Stack *s, const void *data);
int stack_pop(Stack *s, void *out);
int stack_peek(const Stack *s, void *out);
int stack_is_empty(const Stack *s);

#endif