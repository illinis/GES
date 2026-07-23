#include "stack.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int stack_init(Stack *s, size_t element_size){
    if (s == NULL || element_size == 0) return -1;        

    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
    s->element_size = element_size;

    return 0;
}

int stack_clear(Stack *s){
    if(s == NULL) return -1;
    free(s->data);
    s->data = NULL;
    s->size = 0;
    s->capacity = 0;
    return 0;
}

int stack_destroy(Stack *s){
    if(s == NULL) return -1;
    stack_clear(s);
    s->element_size = 0;
    return 0;
}

int stack_push(Stack *s, const void *data){
    if(s == NULL || data == NULL || s->element_size == 0) return -1;

    if (s->size == s->capacity) {
      size_t new_capacity;

      if (s->capacity == 0) {
          new_capacity = 1;
      } else {
          if (s->capacity > SIZE_MAX / 2) return -1;

          new_capacity = s->capacity * 2;
      }

      if (new_capacity > SIZE_MAX / s->element_size) return -1;

      size_t allocation_size = new_capacity * s->element_size;
      void *new_data = realloc(s->data, allocation_size);

      if (new_data == NULL) return -1;

      s->data = new_data;
      s->capacity = new_capacity;
    }


    memcpy((char*)s->data + s->size * s->element_size, data, s->element_size);
    s->size++;
    return 0;
}

int stack_pop(Stack *s, void *out){
    if(s == NULL || s->size == 0) return -1;

    if(out != NULL) memcpy(out, (char*)s->data + (s->size - 1) * s->element_size, s->element_size);
    s->size--;
    return 0;
}

int stack_peek(const Stack *s, void *out){
    if(s == NULL || s->size == 0) return -1;

    if(out != NULL) memcpy(out, (char*)s->data + (s->size - 1) * s->element_size, s->element_size);
    return 0;
}

int stack_is_empty(const Stack *s){
    if(s == NULL) return -1;
    return s->size == 0;
}
