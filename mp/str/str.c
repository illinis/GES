#include "str.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int str_ensure_buffer(Str *str)
{
    if (str == NULL) {
        return -1;
    }

    if (str->data != NULL) {
        return 0;
    }

    str->data = malloc(1);
    if (str->data == NULL) {
        str->length = 0;
        str->capacity = 0;
        return -1;
    }

    str->data[0] = '\0';
    str->length = 0;
    str->capacity = 0;
    return 0;
}

static int str_grow(Str *str, size_t required_capacity)
{
    size_t new_capacity;

    if (str == NULL) {
        return -1;
    }
    if (required_capacity <= str->capacity && str->data != NULL) {
        return 0;
    }
    if (required_capacity == SIZE_MAX) {
        return -1;
    }

    new_capacity = str->capacity == 0 ? 16 : str->capacity;
    while (new_capacity < required_capacity) {
        if (new_capacity > (SIZE_MAX - 1) / 2) {
            new_capacity = required_capacity;
            break;
        }
        new_capacity *= 2;
    }

    return str_reserve(str, new_capacity);
}

static int str_pointer_is_internal(const Str *str, const char *pointer)
{
    uintptr_t first;
    uintptr_t last;
    uintptr_t address;

    if (str == NULL || str->data == NULL || pointer == NULL) {
        return 0;
    }

    first = (uintptr_t)str->data;
    address = (uintptr_t)pointer;
    if (str->capacity > UINTPTR_MAX - first) {
        last = UINTPTR_MAX;
    } else {
        last = first + str->capacity;
    }
    return address >= first && address <= last;
}

int str_init(Str *str)
{
    if (str == NULL) {
        return -1;
    }

    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
    return str_ensure_buffer(str);
}

void str_free(Str *str)
{
    if (str == NULL) {
        return;
    }

    free(str->data);
    str->data = NULL;
    str->length = 0;
    str->capacity = 0;
}

void str_clear(Str *str)
{
    if (str == NULL) {
        return;
    }

    str->length = 0;
    if (str->data != NULL) {
        str->data[0] = '\0';
    }
}

int str_reserve(Str *str, size_t minimum_capacity)
{
    char *new_data;

    if (str == NULL || minimum_capacity == SIZE_MAX) {
        return -1;
    }
    if (minimum_capacity <= str->capacity && str->data != NULL) {
        return 0;
    }

    new_data = realloc(str->data, minimum_capacity + 1);
    if (new_data == NULL) {
        return -1;
    }

    str->data = new_data;
    str->capacity = minimum_capacity;
    if (str->length > minimum_capacity) {
        str->length = minimum_capacity;
    }
    str->data[str->length] = '\0';
    return 0;
}

int str_assign(Str *str, const char *source)
{
    size_t source_length;

    if (str == NULL || source == NULL) {
        return -1;
    }

    source_length = strlen(source);
    if (source_length == SIZE_MAX) {
        return -1;
    }

    if (str_pointer_is_internal(str, source)) {
        memmove(str->data, source, source_length + 1);
        str->length = source_length;
        return 0;
    }

    if (str_grow(str, source_length) != 0) {
        return -1;
    }
    memcpy(str->data, source, source_length + 1);
    str->length = source_length;
    return 0;
}

int str_copy(Str *dest, const Str *src)
{
    if (dest == NULL || src == NULL || src->data == NULL) {
        return -1;
    }
    if (dest == src) {
        return 0;
    }

    return str_assign(dest, src->data);
}

int str_insert(Str *str, size_t position, const char *text)
{
    char *text_copy = NULL;
    size_t text_length;
    size_t new_length;

    if (str == NULL || text == NULL || str->data == NULL ||
        position > str->length) {
        return -1;
    }

    text_length = strlen(text);
    if (text_length == 0) {
        return 0;
    }
    if (text_length > SIZE_MAX - str->length) {
        return -1;
    }

    if (str_pointer_is_internal(str, text)) {
        text_copy = malloc(text_length + 1);
        if (text_copy == NULL) {
            return -1;
        }
        memcpy(text_copy, text, text_length + 1);
        text = text_copy;
    }

    new_length = str->length + text_length;
    if (str_grow(str, new_length) != 0) {
        free(text_copy);
        return -1;
    }

    memmove(str->data + position + text_length,
            str->data + position,
            str->length - position + 1);
    memcpy(str->data + position, text, text_length);
    str->length = new_length;
    free(text_copy);
    return 0;
}

int str_append(Str *str, const char *suffix)
{
    if (str == NULL) {
        return -1;
    }
    return str_insert(str, str->length, suffix);
}

int str_append_char(Str *str, char c)
{
    size_t new_length;

    if (str == NULL || str->data == NULL || c == '\0' ||
        str->length == SIZE_MAX) {
        return -1;
    }

    new_length = str->length + 1;
    if (str_grow(str, new_length) != 0) {
        return -1;
    }

    str->data[str->length] = c;
    str->data[new_length] = '\0';
    str->length = new_length;
    return 0;
}

int str_prepend(Str *str, const char *prefix)
{
    return str_insert(str, 0, prefix);
}

int str_erase(Str *str, size_t position, size_t count)
{
    size_t erased;

    if (str == NULL || str->data == NULL || position > str->length) {
        return -1;
    }
    if (count == 0 || position == str->length) {
        return 0;
    }

    erased = count;
    if (erased > str->length - position) {
        erased = str->length - position;
    }

    memmove(str->data + position,
            str->data + position + erased,
            str->length - position - erased + 1);
    str->length -= erased;
    return 0;
}

int str_replace(Str *str, const char *old_substring,
                const char *new_substring)
{
    char *replacement;
    char *output;
    const char *cursor;
    size_t old_length;
    size_t new_length;
    size_t match_count = 0;
    size_t result_length;
    size_t output_offset = 0;

    if (str == NULL || str->data == NULL || old_substring == NULL ||
        new_substring == NULL) {
        return -1;
    }

    old_length = strlen(old_substring);
    new_length = strlen(new_substring);
    if (old_length == 0) {
        return -1;
    }

    cursor = str->data;
    while ((cursor = strstr(cursor, old_substring)) != NULL) {
        ++match_count;
        cursor += old_length;
    }
    if (match_count == 0) {
        return 0;
    }

    result_length = str->length;
    if (new_length > old_length) {
        size_t increase = new_length - old_length;
        if (match_count > (SIZE_MAX - result_length) / increase) {
            return -1;
        }
        result_length += match_count * increase;
    } else {
        result_length -= match_count * (old_length - new_length);
    }
    if (result_length == SIZE_MAX) {
        return -1;
    }

    replacement = malloc(result_length + 1);
    if (replacement == NULL) {
        return -1;
    }

    cursor = str->data;
    for (;;) {
        const char *match = strstr(cursor, old_substring);
        size_t prefix_length;

        if (match == NULL) {
            prefix_length = strlen(cursor);
            memcpy(replacement + output_offset, cursor, prefix_length + 1);
            output_offset += prefix_length;
            break;
        }

        prefix_length = (size_t)(match - cursor);
        memcpy(replacement + output_offset, cursor, prefix_length);
        output_offset += prefix_length;
        memcpy(replacement + output_offset, new_substring, new_length);
        output_offset += new_length;
        cursor = match + old_length;
    }

    if (result_length <= str->capacity) {
        memcpy(str->data, replacement, result_length + 1);
        free(replacement);
    } else {
        output = str->data;
        str->data = replacement;
        str->capacity = result_length;
        free(output);
    }
    str->length = result_length;
    return 0;
}

int str_compare(const Str *str1, const Str *str2)
{
    if (str1 == str2) {
        return 0;
    }
    if (str1 == NULL || str1->data == NULL) {
        return -1;
    }
    if (str2 == NULL || str2->data == NULL) {
        return 1;
    }
    return strcmp(str1->data, str2->data);
}

size_t str_find_from(const Str *str, const char *substring, size_t start)
{
    const char *match;

    if (str == NULL || str->data == NULL || substring == NULL ||
        start > str->length) {
        return STR_NPOS;
    }

    match = strstr(str->data + start, substring);
    if (match == NULL) {
        return STR_NPOS;
    }
    return (size_t)(match - str->data);
}

size_t str_find(const Str *str, const char *substring)
{
    return str_find_from(str, substring, 0);
}

size_t str_find_char(const Str *str, char c)
{
    const char *match;

    if (str == NULL || str->data == NULL || c == '\0') {
        return STR_NPOS;
    }

    match = strchr(str->data, (unsigned char)c);
    if (match == NULL) {
        return STR_NPOS;
    }
    return (size_t)(match - str->data);
}

int str_find_all(const Str *str, const char *substring, size_t **positions,
                 size_t *match_count)
{
    size_t substring_length;
    size_t count = 0;
    size_t index;
    size_t *matches;

    if (positions == NULL || match_count == NULL) {
        return -1;
    }
    *positions = NULL;
    *match_count = 0;

    if (str == NULL || str->data == NULL || substring == NULL) {
        return -1;
    }

    substring_length = strlen(substring);
    if (substring_length == 0) {
        if (str->length == SIZE_MAX ||
            str->length + 1 > SIZE_MAX / sizeof(*matches)) {
            return -1;
        }
        count = str->length + 1;
    } else if (substring_length <= str->length) {
        for (index = 0; index <= str->length - substring_length; ++index) {
            if (memcmp(str->data + index, substring, substring_length) == 0) {
                ++count;
            }
        }
    }

    if (count == 0) {
        return 0;
    }
    if (count > SIZE_MAX / sizeof(*matches)) {
        return -1;
    }

    matches = malloc(count * sizeof(*matches));
    if (matches == NULL) {
        return -1;
    }

    count = 0;
    if (substring_length == 0) {
        for (index = 0; index <= str->length; ++index) {
            matches[count++] = index;
        }
    } else {
        for (index = 0; index <= str->length - substring_length; ++index) {
            if (memcmp(str->data + index, substring, substring_length) == 0) {
                matches[count++] = index;
            }
        }
    }

    *positions = matches;
    *match_count = count;
    return 0;
}
