#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct queue_ {
    unsigned char *data;
    
    size_t front;
    size_t length;
    size_t capacity;
    size_t element_size;
} Queue;

int queue_init(Queue *q, size_t capacity, size_t element_size);
int queue_destroy(Queue *q);
int queue_clear(Queue *q);

int queue_enqueue(Queue *q, const void *data);
int queue_dequeue(Queue *q, void *out);
int queue_peek(const Queue *q, void *out);

size_t queue_get_length(const Queue *q);
size_t queue_get_capacity(const Queue *q);
int queue_is_empty(const Queue *q);
int queue_is_full(const Queue *q);
int queue_reserve(Queue *q, size_t new_capacity);

#endif
