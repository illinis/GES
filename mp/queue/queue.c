#include "queue.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static size_t wrapped_index(size_t index, size_t distance, size_t capacity){
    size_t until_end = capacity - index;

    if (distance >= until_end) return distance - until_end;
    return index + distance;
}

static int queue_is_valid(const Queue *q){
    return q != NULL
        && q->data != NULL
        && q->capacity != 0
        && q->element_size != 0
        && q->front < q->capacity
        && q->length <= q->capacity;
}

int queue_init(Queue *q, size_t capacity, size_t element_size){
    size_t allocation_size;
    unsigned char *data;

    if (q == NULL || capacity == 0 || element_size == 0) return -1;
    if (capacity > SIZE_MAX / element_size) return -1;

    allocation_size = capacity * element_size;
    data = malloc(allocation_size);
    if (data == NULL) return -1;

    q->data = data;
    q->front = 0;
    q->length = 0;
    q->capacity = capacity;
    q->element_size = element_size;
    return 0;
}

int queue_destroy(Queue *q){
    if (q == NULL) return -1;

    free(q->data);
    q->data = NULL;
    q->front = 0;
    q->length = 0;
    q->capacity = 0;
    q->element_size = 0;

    return 0;
}

int queue_clear(Queue *q){
    if (!queue_is_valid(q)) return -1;

    q->front = 0;
    q->length = 0;

    return 0;
}

int queue_enqueue(Queue *q, const void *data){
    size_t rear;

    if (!queue_is_valid(q) || data == NULL) return -1;
    if (q->length == q->capacity) return -1;

    rear = wrapped_index(q->front, q->length, q->capacity);
    memcpy(q->data + rear * q->element_size, data, q->element_size);
    q->length++;

    return 0;
}

int queue_dequeue(Queue *q, void *out){
    if (!queue_is_valid(q)) return -1;
    if (q->length == 0) return -1;

    if (out != NULL) {
        memcpy(out, q->data + q->front * q->element_size, q->element_size);
    }
    q->front = wrapped_index(q->front, 1, q->capacity);
    q->length--;

    return 0;
}

int queue_peek(const Queue *q, void *out){
    if (!queue_is_valid(q) || out == NULL) return -1;
    if (q->length == 0) return -1;

    memcpy(out, q->data + q->front * q->element_size, q->element_size);
    return 0;
}

int queue_is_empty(const Queue *q){
    if (!queue_is_valid(q)) return -1;
    return q->length == 0;
}

int queue_is_full(const Queue *q){
    if (!queue_is_valid(q)) return -1;
    return q->length == q->capacity;
}

int queue_reserve(Queue *q, size_t new_capacity){
    unsigned char *new_data;
    size_t first_count;
    size_t remaining_count;

    if (!queue_is_valid(q) || new_capacity == 0) return -1;
    if (new_capacity <= q->capacity) return 0;
    if (new_capacity > SIZE_MAX / q->element_size) return -1;

    new_data = malloc(new_capacity * q->element_size);
    if (new_data == NULL) return -1;

    first_count = q->capacity - q->front;
    if (first_count > q->length) first_count = q->length;
    if (first_count != 0) memcpy(new_data, q->data + q->front * q->element_size, first_count * q->element_size);

    remaining_count = q->length - first_count;
    if (remaining_count != 0) memcpy(new_data + first_count * q->element_size, q->data, remaining_count * q->element_size);

    free(q->data);
    q->data = new_data;
    q->front = 0;
    q->capacity = new_capacity;

    return 0;
}
