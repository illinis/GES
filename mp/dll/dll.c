#include "dll.h"

#include <stdlib.h>
#include <string.h>

int dll_init(DLL *list, size_t elem_size){
    if(list == NULL || elem_size == 0){
        return -1;
    }
    list->head = NULL;
    list->tail = NULL;
    list->elem_size = elem_size;
    list->size = 0;

    return 0;
}   

void dll_clear(DLL *list){
    if(list == NULL){
        return;
    }
    while(list->head != NULL){
        DLLNode *temp = list->head;
        list->head = list->head->next;
        free(temp->data);
        free(temp);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void dll_destroy(DLL *list){
    if(list == NULL){
        return; 
    }

    dll_clear(list);
    list->elem_size = 0;
}


int dll_push_front(DLL *list, const void *data){
    if(list == NULL || data == NULL || list->elem_size == 0) return -1;

    DLLNode *new_node = malloc(sizeof(DLLNode));
    if(new_node == NULL) return -1;
    

    new_node->data = malloc(list->elem_size);
    if(new_node->data == NULL){
        free(new_node);
        return -1;
    }

    memcpy(new_node->data, data, list->elem_size);
    new_node->next = list->head;
    new_node->prev = NULL;

    if(list->head != NULL) list->head->prev = new_node;
    else list->tail = new_node;

    list->head = new_node;
    list->size++;
    return 0;
}

int dll_push_back(DLL *list, const void *data){
    if(list == NULL || data == NULL || list->elem_size == 0) return -1;

    DLLNode *new_node = malloc(sizeof(DLLNode));
    if(new_node == NULL) return -1;

    new_node->data = malloc(list->elem_size);
    if(new_node->data == NULL){
        free(new_node);
        return -1;
    }

    memcpy(new_node->data, data, list->elem_size);
    new_node->next = NULL;
    new_node->prev = list->tail;

    if(list->tail != NULL) list->tail->next = new_node;    
    else list->head = new_node;
    
    list->tail = new_node;
    list->size++;
    return 0;
}

int dll_insert(DLL *list, size_t index, const void *data){
    if(list == NULL || data == NULL || list->elem_size == 0) return -1;

    if(index > list->size) return -1;

    if(index == 0) return dll_push_front(list, data);

    if(index == list->size) return dll_push_back(list, data);

    DLLNode *new_node = malloc(sizeof(DLLNode));
    if(new_node == NULL) return -1;

    new_node->data = malloc(list->elem_size);
    if(new_node->data == NULL){
        free(new_node);
        return -1;
    }

    memcpy(new_node->data, data, list->elem_size);
    new_node->next = NULL;
    new_node->prev = NULL;

    DLLNode *current = list->head;
    for(size_t i = 0; i < index; i++){
        current = current->next;
    }

    new_node->next = current;
    new_node->prev = current->prev;
    current->prev->next = new_node;
    current->prev = new_node;

    list->size++;
    return 0;
}

int dll_pop_front(DLL *list, void *out){
    if(list == NULL || list->head == NULL) return -1;

    DLLNode *temp = list->head;
    if(out != NULL) memcpy(out, temp->data, list->elem_size);

    list->head = list->head->next;
    
    if(list->head != NULL) list->head->prev = NULL;
    else list->tail = NULL;
    
    free(temp->data);
    free(temp);

    list->size--;
    return 0;
}

int dll_pop_back(DLL *list, void *out){
    if(list == NULL || list->tail == NULL) return -1;

    DLLNode *temp = list->tail;
    if(out != NULL) memcpy(out, temp->data, list->elem_size);

    list->tail = list->tail->prev;

    if(list->tail != NULL) list->tail->next = NULL;
    else list->head = NULL;

    free(temp->data);
    free(temp);

    list->size--;
    return 0;
}

int dll_erase(DLL *list, size_t index, void* out){
    if(list == NULL || index >= list->size) return -1;

    if(index == 0) return dll_pop_front(list, out);

    if(index == list->size - 1) return dll_pop_back(list, out);

    DLLNode *current = list->head;
    for(size_t i = 0; i < index; i++){
        current = current->next;
    }

    if(out != NULL) memcpy(out, current->data, list->elem_size);

    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current->data);
    free(current);

    list->size--;
    return 0;

}

size_t dll_find(
    const DLL *list,
    const void *data,
    int (*compare)(const void *a, const void *b)
){
    if(list == NULL || data == NULL || compare == NULL) return DLL_NPOS;

    size_t index = 0;
    for(DLLNode *current = list->head; current != NULL; current = current->next, index++){
        if(compare(current->data, data) == 0) return index;
    }

    return DLL_NPOS;
}

size_t dll_find_all(
    const DLL *list,
    const void *data,
    int (*compare)(const void *a, const void *b),
    size_t *indices,
    size_t max_indices
){
    if(list == NULL || data == NULL || compare == NULL) return 0;

    size_t index = 0;
    size_t found = 0;
    for(DLLNode *current = list->head; current != NULL; current = current->next, index++){
        if(compare(current->data, data) == 0){
            if(indices != NULL && found < max_indices) indices[found] = index;
            found++;
        }
    }

    return found;
}
