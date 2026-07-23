#ifndef DLL_H
#define DLL_H

#include <stddef.h>

#define DLL_NPOS ((size_t)-1)

typedef struct dll_node_ {
    void *data;
    struct dll_node_ *next;
    struct dll_node_ *prev;
} DLLNode;

typedef struct dll_ {
    DLLNode *head;
    DLLNode *tail;
    size_t elem_size;
    size_t size;
} DLL;

int dll_init(DLL *list, size_t elem_size);
void dll_destroy(DLL *list);
void dll_clear(DLL *list);

int dll_push_front(DLL *list, const void *data);
int dll_push_back(DLL *list, const void *data);
int dll_insert(DLL *list, size_t index, const void *data);

int dll_pop_front(DLL *list, void *out);
int dll_pop_back(DLL *list, void *out);
int dll_erase(DLL *list, size_t index, void* out);


/* Returns the index of the first element matching data, or DLL_NPOS if
 * not found. */
size_t dll_find(
    const DLL *list,
    const void *data,
    int (*compare)(const void *a, const void *b)
);

/* Writes the indices of every matching element into indices (up to
 * max_indices entries) and returns the total number of matches found,
 * which may exceed max_indices if the array was too small. */
size_t dll_find_all(
    const DLL *list,
    const void *data,
    int (*compare)(const void *a, const void *b),
    size_t *indices,
    size_t max_indices
);


#endif