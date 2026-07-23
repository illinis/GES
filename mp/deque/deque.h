#ifndef DEQUE_H
#define DEQUE_H

#include <stddef.h>

typedef struct deque_ {
    unsigned char *data;
    
    size_t front;
    size_t back;

    size_t length;
    size_t capacity;
    size_t element_size;
} Deque;

int deque_init(Deque *d, size_t capacity, size_t element_size);
int deque_destroy(Deque *d);
int deque_clear(Deque *d);

int deque_push_back(Deque *d, const void *data);
int deque_push_front(Deque *d, const void *data);

int deque_pop_back(Deque *d, void *out);
int deque_pop_front(Deque *d, void *out);

int deque_peek_back(const Deque *d, void *out);
int deque_peek_front(const Deque *d, void *out);

int deque_is_empty(const Deque *d); 
int deque_is_full(const Deque *d);

int deque_reserve(Deque *d, size_t new_capacity);

#endif
