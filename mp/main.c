/*
 * Validation suite for the micro-projects living under mp/.
 * Build and run with `make run` from this directory.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "str/str.h"
#include "vec/vec.h"
#include "dll/dll.h"

static int g_failures = 0;
static int g_checks = 0;

#define CHECK(cond) \
    do { \
        ++g_checks; \
        if (!(cond)) { \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures; \
        } \
    } while (0)

/* ------------------------------------------------------------------ */
/* str                                                                  */
/* ------------------------------------------------------------------ */

static void test_str_init_and_assign(void)
{
    Str s;

    CHECK(str_init(&s) == 0);
    CHECK(s.length == 0);
    CHECK(strcmp(s.data, "") == 0);

    CHECK(str_assign(&s, "hello") == 0);
    CHECK(s.length == 5);
    CHECK(strcmp(s.data, "hello") == 0);

    CHECK(str_assign(&s, "hi") == 0);
    CHECK(s.length == 2);
    CHECK(strcmp(s.data, "hi") == 0);

    str_free(&s);
}

static void test_str_insert_append_erase(void)
{
    Str s;

    str_init(&s);
    str_assign(&s, "Hello World");

    CHECK(str_insert(&s, 5, ",") == 0);
    CHECK(strcmp(s.data, "Hello, World") == 0);

    CHECK(str_append(&s, "!") == 0);
    CHECK(strcmp(s.data, "Hello, World!") == 0);

    CHECK(str_prepend(&s, ">> ") == 0);
    CHECK(strcmp(s.data, ">> Hello, World!") == 0);

    CHECK(str_append_char(&s, '?') == 0);
    CHECK(strcmp(s.data, ">> Hello, World!?") == 0);

    CHECK(str_erase(&s, 0, 3) == 0);
    CHECK(strcmp(s.data, "Hello, World!?") == 0);

    CHECK(str_erase(&s, s.length - 2, 2) == 0);
    CHECK(strcmp(s.data, "Hello, World") == 0);

    str_free(&s);
}

static void test_str_self_referential(void)
{
    Str s;

    /* Appending a string to itself must not be corrupted by reallocation. */
    str_init(&s);
    str_assign(&s, "hello");
    CHECK(str_append(&s, s.data) == 0);
    CHECK(strcmp(s.data, "hellohello") == 0);

    /* Assigning from an internal substring (a suffix of itself). */
    CHECK(str_assign(&s, s.data + 5) == 0);
    CHECK(strcmp(s.data, "hello") == 0);

    /* Inserting the whole current buffer into the middle of itself. */
    str_assign(&s, "AB");
    CHECK(str_insert(&s, 1, s.data) == 0);
    CHECK(strcmp(s.data, "AABB") == 0);

    str_free(&s);
}

static void test_str_replace(void)
{
    Str s;

    str_init(&s);
    str_assign(&s, "the cat sat on the mat");
    CHECK(str_replace(&s, "at", "og") == 0);
    CHECK(strcmp(s.data, "the cog sog on the mog") == 0);

    str_assign(&s, "aaa");
    CHECK(str_replace(&s, "a", "bb") == 0);
    CHECK(strcmp(s.data, "bbbbbb") == 0);

    str_assign(&s, "nothing here matches");
    CHECK(str_replace(&s, "xyz", "?") == 0);
    CHECK(strcmp(s.data, "nothing here matches") == 0);

    str_free(&s);
}

static void test_str_compare_and_find(void)
{
    Str a, b;

    str_init(&a);
    str_init(&b);
    str_assign(&a, "abc");
    str_assign(&b, "abc");
    CHECK(str_compare(&a, &b) == 0);

    str_assign(&b, "abd");
    CHECK(str_compare(&a, &b) < 0);
    CHECK(str_compare(&b, &a) > 0);

    str_assign(&a, "the quick brown fox");
    CHECK(str_find(&a, "quick") == 4);
    CHECK(str_find(&a, "slow") == STR_NPOS);
    CHECK(str_find_from(&a, "o", 5) == 12);
    CHECK(str_find_char(&a, 'q') == 4);
    CHECK(str_find_char(&a, 'z') == STR_NPOS);

    str_free(&a);
    str_free(&b);
}

static void test_str_find_all(void)
{
    Str s;
    size_t *positions = NULL;
    size_t count = 0;

    str_init(&s);
    str_assign(&s, "aaaa");

    /* Overlapping matches: "aa" occurs at offsets 0, 1 and 2. */
    CHECK(str_find_all(&s, "aa", &positions, &count) == 0);
    CHECK(count == 3);
    if (count == 3) {
        CHECK(positions[0] == 0);
        CHECK(positions[1] == 1);
        CHECK(positions[2] == 2);
    }

    free(positions);
    str_free(&s);
}

static void test_str_copy_independence(void)
{
    Str a, b;

    str_init(&a);
    str_init(&b);
    str_assign(&a, "original");

    CHECK(str_copy(&b, &a) == 0);
    CHECK(strcmp(b.data, "original") == 0);
    CHECK(a.data != b.data);

    str_assign(&b, "changed");
    CHECK(strcmp(a.data, "original") == 0);

    str_free(&a);
    str_free(&b);
}

/* ------------------------------------------------------------------ */
/* vec                                                                  */
/* ------------------------------------------------------------------ */

static int cmp_int(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

static void test_vec_push_get_set_pop(void)
{
    Vec v;
    int value;
    int out;

    CHECK(vec_init(&v, sizeof(int)) == 0);

    for (value = 0; value < 5; ++value) {
        CHECK(vec_push_back(&v, &value) == 0);
    }
    CHECK(v.length == 5);

    CHECK(vec_get(&v, 2, &out) == 0);
    CHECK(out == 2);

    value = 99;
    CHECK(vec_set(&v, 2, &value) == 0);
    CHECK(vec_get(&v, 2, &out) == 0);
    CHECK(out == 99);

    CHECK(vec_pop_back(&v, &out) == 0);
    CHECK(out == 4);
    CHECK(v.length == 4);

    vec_free(&v);
}

static void test_vec_insert_erase_find(void)
{
    Vec v;
    int values[] = {10, 20, 30, 40};
    int inserted = 15;
    size_t i;
    size_t found;

    vec_init(&v, sizeof(int));
    for (i = 0; i < 4; ++i) {
        vec_push_back(&v, &values[i]);
    }

    CHECK(vec_insert(&v, 1, &inserted) == 0);
    CHECK(v.length == 5);
    CHECK(*(int *)vec_at(&v, 1) == 15);
    CHECK(*(int *)vec_at(&v, 2) == 20);

    found = vec_find(&v, &inserted, cmp_int);
    CHECK(found == 1);

    CHECK(vec_erase(&v, 1, 1) == 0);
    CHECK(v.length == 4);
    CHECK(*(int *)vec_at(&v, 1) == 20);

    vec_free(&v);
}

static void test_vec_at_const_correctness(void)
{
    Vec v;
    int value = 42;
    const Vec *cv = &v;

    vec_init(&v, sizeof(int));
    vec_push_back(&v, &value);

    /* Read-only access through a const Vec* only ever yields a const ptr. */
    CHECK(*(const int *)vec_at_const(cv, 0) == 42);

    /* Mutable access still works through a non-const Vec*. */
    *(int *)vec_at(&v, 0) = 100;
    CHECK(*(const int *)vec_at_const(cv, 0) == 100);

    vec_free(&v);
}

static void test_vec_sort_duplicates(void)
{
    Vec v;
    int values[] = {5, 3, 5, 1, 5, 5, 2, 5, 5, 0, 5};
    size_t n = sizeof(values) / sizeof(values[0]);
    size_t i;
    int count;
    clock_t start;
    double elapsed;

    vec_init(&v, sizeof(int));
    for (i = 0; i < n; ++i) {
        vec_push_back(&v, &values[i]);
    }

    vec_sort(&v, cmp_int);
    for (i = 1; i < v.length; ++i) {
        CHECK(*(int *)vec_at(&v, i - 1) <= *(int *)vec_at(&v, i));
    }

    /* Heavily duplicated data: correctness (and speed) under a pathological
     * two-way-partition case, where every element equals the pivot. */
    vec_clear(&v);
    for (count = 0; count < 20000; ++count) {
        int val = 7;
        vec_push_back(&v, &val);
    }

    start = clock();
    vec_sort(&v, cmp_int);
    elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    printf("  (sorted %d duplicate elements in %.4fs)\n", count, elapsed);

    for (i = 0; i < v.length; ++i) {
        CHECK(*(int *)vec_at(&v, i) == 7);
    }

    vec_free(&v);
}

static void test_vec_clone_assign(void)
{
    Vec a, b;
    int i;
    int newval = 999;
    int raw[] = {10, 20, 30};
    const void *slice;

    vec_init(&a, sizeof(int));
    for (i = 0; i < 10; ++i) {
        vec_push_back(&a, &i);
    }

    vec_init(&b, sizeof(int));
    CHECK(vec_clone(&b, &a) == 0);
    CHECK(b.length == a.length);
    CHECK(b.data != a.data);
    for (i = 0; i < (int)a.length; ++i) {
        CHECK(*(int *)vec_at(&a, (size_t)i) == *(int *)vec_at(&b, (size_t)i));
    }

    /* Independent backing allocation: mutating one leaves the other intact. */
    vec_set(&b, 0, &newval);
    CHECK(*(int *)vec_at(&a, 0) == 0);
    CHECK(*(int *)vec_at(&b, 0) == 999);

    /* vec_assign from a plain array. */
    CHECK(vec_assign(&a, raw, 3) == 0);
    CHECK(a.length == 3);
    CHECK(*(int *)vec_at(&a, 1) == 20);

    /* Self-referential assign: keep only a slice of the vector's own data. */
    vec_clear(&a);
    for (i = 0; i < 6; ++i) {
        vec_push_back(&a, &i);
    }
    slice = vec_at_const(&a, 2);
    CHECK(vec_assign(&a, slice, 3) == 0);
    CHECK(a.length == 3);
    CHECK(*(int *)vec_at(&a, 0) == 2);
    CHECK(*(int *)vec_at(&a, 1) == 3);
    CHECK(*(int *)vec_at(&a, 2) == 4);

    vec_free(&a);
    vec_free(&b);
}

typedef struct {
    int id;
    const char *label; /* referenced, never owned by the vector */
} Tagged;

static void test_vec_does_not_own_referenced_resources(void)
{
    Vec original, clone;
    Tagged items[2];
    const Tagged *o0;
    const Tagged *c0;

    items[0].id = 1;
    items[0].label = "alpha";
    items[1].id = 2;
    items[1].label = "beta";

    vec_init(&original, sizeof(Tagged));
    vec_push_back(&original, &items[0]);
    vec_push_back(&original, &items[1]);

    vec_init(&clone, sizeof(Tagged));
    CHECK(vec_clone(&clone, &original) == 0);
    CHECK(clone.data != original.data);

    /* Bytewise copy means both vectors' elements alias the very same
     * external string - the vector duplicates neither ends nor owns it. */
    o0 = (const Tagged *)vec_at_const(&original, 0);
    c0 = (const Tagged *)vec_at_const(&clone, 0);
    CHECK(o0->label == c0->label);

    /* Freeing the clone's backing storage must not disturb the original's,
     * nor the strings neither vector owns. */
    vec_free(&clone);
    o0 = (const Tagged *)vec_at_const(&original, 0);
    CHECK(o0->id == 1);
    CHECK(strcmp(o0->label, "alpha") == 0);

    vec_free(&original);
}

/* ------------------------------------------------------------------ */
/* dll                                                                  */
/* ------------------------------------------------------------------ */

static void test_dll_push_pop_front_back(void)
{
    DLL list;
    int value;
    int out;

    CHECK(dll_init(&list, sizeof(int)) == 0);

    for (value = 0; value < 5; ++value) {
        CHECK(dll_push_back(&list, &value) == 0);
    }
    CHECK(list.size == 5);
    CHECK(*(int *)list.head->data == 0);
    CHECK(*(int *)list.tail->data == 4);

    value = -1;
    CHECK(dll_push_front(&list, &value) == 0);
    CHECK(list.size == 6);
    CHECK(*(int *)list.head->data == -1);

    CHECK(dll_pop_front(&list, &out) == 0);
    CHECK(out == -1);
    CHECK(list.size == 5);

    CHECK(dll_pop_back(&list, &out) == 0);
    CHECK(out == 4);
    CHECK(list.size == 4);
    CHECK(*(int *)list.tail->data == 3);

    dll_destroy(&list);
}

static void test_dll_insert_erase_find(void)
{
    DLL list;
    DLLNode *node;
    int values[] = {10, 20, 30, 40};
    int inserted = 15;
    int missing = 99;
    int out;
    size_t i;
    size_t found;

    dll_init(&list, sizeof(int));
    for (i = 0; i < 4; ++i) {
        dll_push_back(&list, &values[i]);
    }

    CHECK(dll_insert(&list, 1, &inserted) == 0);
    CHECK(list.size == 5);

    found = dll_find(&list, &inserted, cmp_int);
    CHECK(found == 1);
    CHECK(dll_find(&list, &values[3], cmp_int) == 4);
    CHECK(dll_find(&list, &missing, cmp_int) == DLL_NPOS);

    CHECK(dll_erase(&list, 1, &out) == 0);
    CHECK(out == 15);
    CHECK(list.size == 4);

    /* Walk forward then backward to confirm prev/next links stay
     * consistent after the insert/erase pair. */
    node = list.head;
    for (i = 0; i < 4; ++i) {
        CHECK(*(int *)node->data == values[i]);
        if (i < 3) node = node->next;
    }
    for (i = 4; i-- > 0; ) {
        CHECK(*(int *)node->data == values[i]);
        if (i > 0) node = node->prev;
    }

    dll_destroy(&list);
}

static void test_dll_find_all(void)
{
    DLL list;
    int values[] = {2, 1, 2, 3, 2};
    int target = 2;
    size_t all_indices[3];
    size_t some_indices[2];
    size_t i;
    size_t count;

    dll_init(&list, sizeof(int));
    for (i = 0; i < 5; ++i) {
        dll_push_back(&list, &values[i]);
    }

    /* Buffer large enough for every match. */
    count = dll_find_all(&list, &target, cmp_int, all_indices, 3);
    CHECK(count == 3);
    CHECK(all_indices[0] == 0);
    CHECK(all_indices[1] == 2);
    CHECK(all_indices[2] == 4);

    /* Undersized buffer: the true match count is still reported, but
     * only as many indices as fit are written. */
    count = dll_find_all(&list, &target, cmp_int, some_indices, 2);
    CHECK(count == 3);
    CHECK(some_indices[0] == 0);
    CHECK(some_indices[1] == 2);

    dll_destroy(&list);
}

static void test_dll_clear_and_reuse(void)
{
    DLL list;
    int value;
    int out;

    dll_init(&list, sizeof(int));
    for (value = 0; value < 3; ++value) {
        dll_push_back(&list, &value);
    }
    CHECK(list.size == 3);

    dll_clear(&list);
    CHECK(list.size == 0);
    CHECK(list.head == NULL);
    CHECK(list.tail == NULL);

    /* A cleared (but not destroyed) list keeps its element size and
     * must still accept new elements. */
    value = 7;
    CHECK(dll_push_back(&list, &value) == 0);
    CHECK(list.size == 1);
    CHECK(dll_pop_front(&list, &out) == 0);
    CHECK(out == 7);

    dll_destroy(&list);
}

static void test_dll_destroy_rejects_further_use(void)
{
    DLL list;
    int value;

    dll_init(&list, sizeof(int));
    value = 1; dll_push_back(&list, &value);
    value = 2; dll_push_back(&list, &value);

    dll_destroy(&list);
    CHECK(list.head == NULL);
    CHECK(list.tail == NULL);
    CHECK(list.size == 0);
    CHECK(list.elem_size == 0);

    /* A destroyed list must reject every mutating call instead of
     * silently reviving itself. */
    value = 99;
    CHECK(dll_push_back(&list, &value) == -1);
    CHECK(dll_push_front(&list, &value) == -1);
    CHECK(dll_insert(&list, 0, &value) == -1);
    CHECK(list.size == 0);
    CHECK(list.head == NULL);
}

/* ------------------------------------------------------------------ */

int main(void)
{
    test_str_init_and_assign();
    test_str_insert_append_erase();
    test_str_self_referential();
    test_str_replace();
    test_str_compare_and_find();
    test_str_find_all();
    test_str_copy_independence();

    test_vec_push_get_set_pop();
    test_vec_insert_erase_find();
    test_vec_at_const_correctness();
    test_vec_sort_duplicates();
    test_vec_clone_assign();
    test_vec_does_not_own_referenced_resources();

    test_dll_push_pop_front_back();
    test_dll_insert_erase_find();
    test_dll_find_all();
    test_dll_clear_and_reuse();
    test_dll_destroy_rejects_further_use();

    if (g_failures == 0) {
        printf("all %d checks passed\n", g_checks);
        return 0;
    }

    fprintf(stderr, "%d/%d checks failed\n", g_failures, g_checks);
    return 1;
}
