#include "deque.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static size_t wrapped_index(size_t index, size_t distance, size_t capacity){
    size_t until_end = capacity - index;

    if (distance >= until_end) return distance - until_end;
    return index + distance;
}

static size_t previous_index(size_t index, size_t capacity){
    return index == 0 ? capacity - 1 : index - 1;
}

static int deque_is_valid(const Deque *d){
    return d != NULL
        && d->data != NULL
        && d->capacity != 0
        && d->element_size != 0
        && d->front < d->capacity
        && d->back < d->capacity
        && d->length <= d->capacity
        && d->back == wrapped_index(d->front, d->length, d->capacity);
}

int deque_init(Deque *d, size_t capacity, size_t element_size){
    size_t allocation_size;
    unsigned char *data;

    if (d == NULL || capacity == 0 || element_size == 0) return -1;
    if (capacity > SIZE_MAX / element_size) return -1;

    allocation_size = capacity * element_size;
    data = malloc(allocation_size);
    if (data == NULL) return -1;

    d->data = data;
    d->front = 0;
    d->back = 0;
    d->length = 0;
    d->capacity = capacity;
    d->element_size = element_size;
    return 0;
}

int deque_destroy(Deque *d){
    if (d == NULL) return -1;

    free(d->data);
    d->data = NULL;
    d->front = 0;
    d->back = 0;
    d->length = 0;
    d->capacity = 0;
    d->element_size = 0;
    return 0;
}

int deque_clear(Deque *d){
    if (!deque_is_valid(d)) return -1;

    d->front = 0;
    d->back = 0;
    d->length = 0;
    return 0;
}

int deque_push_back(Deque *d, const void *data){
    if (!deque_is_valid(d) || data == NULL) return -1;
    if (d->length == d->capacity) return -1;

    memcpy(d->data + d->back * d->element_size, data, d->element_size);
    d->back = wrapped_index(d->back, 1, d->capacity);
    d->length++;
    return 0;
}

int deque_push_front(Deque *d, const void *data){
    size_t new_front;

    if (!deque_is_valid(d) || data == NULL) return -1;
    if (d->length == d->capacity) return -1;

    /*
     * front always names the first occupied slot, so make room before
     * writing the new first element.
     */
    new_front = previous_index(d->front, d->capacity);
    memcpy(d->data + new_front * d->element_size, data, d->element_size);
    d->front = new_front;
    d->length++;
    return 0;
}

int deque_pop_back(Deque *d, void *out){
    if (!deque_is_valid(d)) return -1;
    if (d->length == 0) return -1;

    d->back = previous_index(d->back, d->capacity);
    if (out != NULL) {
        memcpy(out, d->data + d->back * d->element_size, d->element_size);
    }
    d->length--;
    return 0;
}

int deque_pop_front(Deque *d, void *out){
    if (!deque_is_valid(d)) return -1;
    if (d->length == 0) return -1;

    if (out != NULL) {
        memcpy(out, d->data + d->front * d->element_size, d->element_size);
    }
    d->front = wrapped_index(d->front, 1, d->capacity);
    d->length--;
    return 0;
}

int deque_peek_back(const Deque *d, void *out){
    size_t back;

    if (!deque_is_valid(d) || out == NULL) return -1;
    if (d->length == 0) return -1;

    back = previous_index(d->back, d->capacity);
    memcpy(out, d->data + back * d->element_size, d->element_size);
    return 0;
}

int deque_peek_front(const Deque *d, void *out){
    if (!deque_is_valid(d) || out == NULL) return -1;
    if (d->length == 0) return -1;

    memcpy(out, d->data + d->front * d->element_size, d->element_size);
    return 0;
}

int deque_is_empty(const Deque *d){
    if (!deque_is_valid(d)) return -1;
    return d->length == 0;
}

int deque_is_full(const Deque *d){
    if (!deque_is_valid(d)) return -1;
    return d->length == d->capacity;
}

int deque_reserve(Deque *d, size_t new_capacity){
    unsigned char *new_data;
    size_t first_count;
    size_t remaining_count;

    if (!deque_is_valid(d) || new_capacity == 0) return -1;
    if (new_capacity <= d->capacity) return 0;
    if (new_capacity > SIZE_MAX / d->element_size) return -1;

    new_data = malloc(new_capacity * d->element_size);
    if (new_data == NULL) return -1;

    first_count = d->capacity - d->front;
    if (first_count > d->length) first_count = d->length;
    if (first_count != 0) memcpy(new_data, d->data + d->front * d->element_size, first_count * d->element_size);

    remaining_count = d->length - first_count;
    if (remaining_count != 0) memcpy(new_data + first_count * d->element_size, d->data, remaining_count * d->element_size);

    free(d->data);
    d->data = new_data;
    d->front = 0;
    d->back = d->length;
    d->capacity = new_capacity;

    return 0;
}
