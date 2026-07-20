#ifndef STR_H
#define STR_H

#include <stddef.h>

#define STR_NPOS ((size_t)-1)

typedef struct {
    char *data;
    size_t length;
    size_t capacity;
} Str;

int str_init(Str *str);
void str_free(Str *str);
void str_clear(Str *str);

int str_reserve(Str *str, size_t minimum_capacity);

int str_assign(Str *str, const char *source);
int str_copy(Str *dest, const Str *src);

int str_insert(Str *str, size_t position, const char *text);
int str_append(Str *str, const char *suffix);
int str_append_char(Str *str, char c);
int str_prepend(Str *str, const char *prefix);
int str_erase(Str *str, size_t position, size_t count);
int str_replace(Str *str, const char *old_substring, const char *new_substring);

int str_compare(const Str *str1, const Str *str2);

size_t str_find(const Str *str, const char *substring);
size_t str_find_from(const Str *str, const char *substring, size_t start);

size_t str_find_char(const Str *str, char c);

int str_find_all(const Str *str, const char *substring, size_t **positions, size_t *match_count);

#endif
