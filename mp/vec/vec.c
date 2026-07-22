#include "vec.h"

#include <stdint.h>
#include <stdlib.h>

static void *vec_raw_copy(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    size_t i;

    for (i = 0; i < n; ++i) {
        d[i] = s[i];
    }
    return dest;
}

static void *vec_raw_move(void *dest, const void *src, size_t n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    size_t i;

    if (d == s || n == 0) {
        return dest;
    }

    if (d < s) {
        for (i = 0; i < n; ++i) {
            d[i] = s[i];
        }
    } else {
        for (i = n; i > 0; --i) {
            d[i - 1] = s[i - 1];
        }
    }
    return dest;
}

static void vec_raw_swap(void *a, void *b, size_t n)
{
    unsigned char *pa = a;
    unsigned char *pb = b;
    unsigned char tmp;
    size_t i;

    for (i = 0; i < n; ++i) {
        tmp = pa[i];
        pa[i] = pb[i];
        pb[i] = tmp;
    }
}

static int vec_pointer_is_internal(const Vec *vec, const void *pointer)
{
    uintptr_t first;
    uintptr_t last;
    uintptr_t address;
    uintptr_t span;

    if (vec == NULL || vec->data == NULL || pointer == NULL) {
        return 0;
    }

    first = (uintptr_t)vec->data;
    address = (uintptr_t)pointer;
    span = (uintptr_t)vec->capacity * (uintptr_t)vec->elem_size;
    if (span > UINTPTR_MAX - first) {
        last = UINTPTR_MAX;
    } else {
        last = first + span;
    }
    return address >= first && address < last;
}

static int vec_grow(Vec *vec, size_t required_capacity)
{
    size_t new_capacity;
    size_t max_capacity;

    if (vec == NULL || vec->elem_size == 0) {
        return -1;
    }
    if (required_capacity <= vec->capacity) {
        return 0;
    }

    max_capacity = SIZE_MAX / vec->elem_size;
    if (required_capacity > max_capacity) {
        return -1;
    }

    new_capacity = vec->capacity == 0 ? 4 : vec->capacity;
    while (new_capacity < required_capacity) {
        if (new_capacity > max_capacity / 2) {
            new_capacity = required_capacity;
            break;
        }
        new_capacity *= 2;
    }

    return vec_reserve(vec, new_capacity);
}

int vec_init(Vec *vec, size_t elem_size)
{
    if (vec == NULL || elem_size == 0) {
        return -1;
    }

    vec->data = NULL;
    vec->length = 0;
    vec->capacity = 0;
    vec->elem_size = elem_size;
    return 0;
}

void vec_free(Vec *vec)
{
    if (vec == NULL) {
        return;
    }

    free(vec->data);
    vec->data = NULL;
    vec->length = 0;
    vec->capacity = 0;
    vec->elem_size = 0;
}

void vec_clear(Vec *vec)
{
    if (vec == NULL) {
        return;
    }

    vec->length = 0;
}

int vec_reserve(Vec *vec, size_t minimum_capacity)
{
    void *new_data;

    if (vec == NULL || vec->elem_size == 0) {
        return -1;
    }
    if (minimum_capacity <= vec->capacity) {
        return 0;
    }
    if (minimum_capacity > SIZE_MAX / vec->elem_size) {
        return -1;
    }

    new_data = realloc(vec->data, minimum_capacity * vec->elem_size);
    if (new_data == NULL) {
        return -1;
    }

    vec->data = new_data;
    vec->capacity = minimum_capacity;
    return 0;
}

int vec_shrink_to_fit(Vec *vec)
{
    void *new_data;

    if (vec == NULL) {
        return -1;
    }
    if (vec->length == vec->capacity) {
        return 0;
    }

    if (vec->length == 0) {
        free(vec->data);
        vec->data = NULL;
        vec->capacity = 0;
        return 0;
    }

    new_data = realloc(vec->data, vec->length * vec->elem_size);
    if (new_data == NULL) {
        return -1;
    }

    vec->data = new_data;
    vec->capacity = vec->length;
    return 0;
}

int vec_assign(Vec *vec, const void *data, size_t count)
{
    if (vec == NULL || vec->elem_size == 0 || (data == NULL && count != 0)) {
        return -1;
    }
    if (count > SIZE_MAX / vec->elem_size) {
        return -1;
    }
    if (count == 0) {
        vec->length = 0;
        return 0;
    }

    if (vec_pointer_is_internal(vec, data)) {
        vec_raw_move(vec->data, data, count * vec->elem_size);
        vec->length = count;
        return 0;
    }

    if (vec_reserve(vec, count) != 0) {
        return -1;
    }

    vec_raw_copy(vec->data, data, count * vec->elem_size);
    vec->length = count;
    return 0;
}

int vec_clone(Vec *dest, const Vec *src)
{
    if (dest == NULL || src == NULL || dest->elem_size != src->elem_size) {
        return -1;
    }
    if (dest == src) {
        return 0;
    }

    return vec_assign(dest, src->data, src->length);
}

void *vec_at(Vec *vec, size_t index)
{
    if (vec == NULL || index >= vec->length) {
        return NULL;
    }
    return (unsigned char *)vec->data + index * vec->elem_size;
}

const void *vec_at_const(const Vec *vec, size_t index)
{
    if (vec == NULL || index >= vec->length) {
        return NULL;
    }
    return (const unsigned char *)vec->data + index * vec->elem_size;
}

int vec_get(const Vec *vec, size_t index, void *out)
{
    const void *slot = vec_at_const(vec, index);

    if (slot == NULL || out == NULL) {
        return -1;
    }

    vec_raw_copy(out, slot, vec->elem_size);
    return 0;
}

int vec_set(Vec *vec, size_t index, const void *element)
{
    void *slot;

    if (element == NULL) {
        return -1;
    }

    slot = vec_at(vec, index);
    if (slot == NULL) {
        return -1;
    }
    if (slot == element) {
        return 0;
    }

    vec_raw_copy(slot, element, vec->elem_size);
    return 0;
}

int vec_push_back(Vec *vec, const void *element)
{
    unsigned char *element_copy = NULL;
    const void *source = element;
    size_t new_length;

    if (vec == NULL || element == NULL || vec->elem_size == 0 ||
        vec->length == SIZE_MAX) {
        return -1;
    }

    if (vec_pointer_is_internal(vec, element)) {
        element_copy = malloc(vec->elem_size);
        if (element_copy == NULL) {
            return -1;
        }
        vec_raw_copy(element_copy, element, vec->elem_size);
        source = element_copy;
    }

    new_length = vec->length + 1;
    if (vec_grow(vec, new_length) != 0) {
        free(element_copy);
        return -1;
    }

    vec_raw_copy((unsigned char *)vec->data + vec->length * vec->elem_size,
                source, vec->elem_size);
    vec->length = new_length;
    free(element_copy);
    return 0;
}

int vec_pop_back(Vec *vec, void *out)
{
    if (vec == NULL || vec->length == 0) {
        return -1;
    }

    vec->length -= 1;
    if (out != NULL) {
        vec_raw_copy(out,
                    (unsigned char *)vec->data + vec->length * vec->elem_size,
                    vec->elem_size);
    }
    return 0;
}

int vec_insert(Vec *vec, size_t position, const void *element)
{
    unsigned char *element_copy = NULL;
    const void *source = element;
    size_t new_length;

    if (vec == NULL || element == NULL || vec->elem_size == 0 ||
        position > vec->length || vec->length == SIZE_MAX) {
        return -1;
    }

    if (vec_pointer_is_internal(vec, element)) {
        element_copy = malloc(vec->elem_size);
        if (element_copy == NULL) {
            return -1;
        }
        vec_raw_copy(element_copy, element, vec->elem_size);
        source = element_copy;
    }

    new_length = vec->length + 1;
    if (vec_grow(vec, new_length) != 0) {
        free(element_copy);
        return -1;
    }

    vec_raw_move((unsigned char *)vec->data + (position + 1) * vec->elem_size,
                (unsigned char *)vec->data + position * vec->elem_size,
                (vec->length - position) * vec->elem_size);
    vec_raw_copy((unsigned char *)vec->data + position * vec->elem_size,
                source, vec->elem_size);
    vec->length = new_length;
    free(element_copy);
    return 0;
}

int vec_erase(Vec *vec, size_t position, size_t count)
{
    size_t erased;

    if (vec == NULL || position > vec->length) {
        return -1;
    }
    if (count == 0 || position == vec->length) {
        return 0;
    }

    erased = count;
    if (erased > vec->length - position) {
        erased = vec->length - position;
    }

    vec_raw_move((unsigned char *)vec->data + position * vec->elem_size,
                (unsigned char *)vec->data + (position + erased) * vec->elem_size,
                (vec->length - position - erased) * vec->elem_size);
    vec->length -= erased;
    return 0;
}

size_t vec_find(const Vec *vec, const void *element,
                int (*compare)(const void *a, const void *b))
{
    size_t index;

    if (vec == NULL || element == NULL || compare == NULL) {
        return VEC_NPOS;
    }

    for (index = 0; index < vec->length; ++index) {
        const void *slot = (const unsigned char *)vec->data + index * vec->elem_size;

        if (compare(slot, element) == 0) {
            return index;
        }
    }
    return VEC_NPOS;
}

/*
 * Three-way (Dutch national flag) partitioning: elements equal to the pivot
 * are grouped in the middle and excluded from both recursive calls. Without
 * this, a run of duplicate keys degenerates a two-way partition to O(n^2).
 */
static void vec_quicksort(unsigned char *base, size_t low, size_t high,
                          size_t elem_size,
                          int (*compare)(const void *a, const void *b),
                          unsigned char *pivot)
{
    size_t mid;
    size_t lt, gt, i;

    if (low >= high) {
        return;
    }

    mid = low + (high - low) / 2;
    vec_raw_copy(pivot, base + mid * elem_size, elem_size);

    lt = low;
    gt = high;
    i = low;

    while (i <= gt) {
        int cmp = compare(base + i * elem_size, pivot);

        if (cmp < 0) {
            vec_raw_swap(base + lt * elem_size, base + i * elem_size, elem_size);
            ++lt;
            ++i;
        } else if (cmp > 0) {
            vec_raw_swap(base + i * elem_size, base + gt * elem_size, elem_size);
            if (gt == low) {
                break;
            }
            --gt;
        } else {
            ++i;
        }
    }

    if (lt > low) {
        vec_quicksort(base, low, lt - 1, elem_size, compare, pivot);
    }
    if (gt < high) {
        vec_quicksort(base, gt + 1, high, elem_size, compare, pivot);
    }
}

void vec_sort(Vec *vec, int (*compare)(const void *a, const void *b))
{
    unsigned char *pivot;

    if (vec == NULL || compare == NULL || vec->length < 2) {
        return;
    }

    pivot = malloc(vec->elem_size);
    if (pivot == NULL) {
        return;
    }

    vec_quicksort(vec->data, 0, vec->length - 1, vec->elem_size, compare, pivot);
    free(pivot);
}
