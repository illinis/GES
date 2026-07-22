/*
 * Usage reference for the micro-projects living under mp/.
 * Run with `make demo` (see Makefile). Each library gets its own
 * self-contained segment below, walked in the order a caller would
 * typically reach for its API.
 */
#include <stdio.h>
#include <stdlib.h>

#include "str/str.h"
#include "vec/vec.h"

static void section(const char *title)
{
    printf("\n=== %s ===\n", title);
}

/* ================================================================== *
 * str - dynamic string                                                *
 * ================================================================== */

static void usage_str(void)
{
    Str s, other;

    section("str: init / assign");
    str_init(&s);
    str_assign(&s, "Hello, World!");
    printf("s = \"%s\" (length %zu, capacity %zu)\n", s.data, s.length, s.capacity);

    section("str: append / prepend / insert / append_char");
    str_append(&s, " Goodbye.");
    printf("after append:      \"%s\"\n", s.data);
    str_prepend(&s, ">> ");
    printf("after prepend:     \"%s\"\n", s.data);
    str_insert(&s, 3, "*** ");
    printf("after insert(3):   \"%s\"\n", s.data);
    str_append_char(&s, '!');
    printf("after append_char: \"%s\"\n", s.data);

    section("str: erase");
    /* strip the leading ">> *** " marker (7 chars) added above */
    str_erase(&s, 0, 7);
    printf("after erase(0,7): \"%s\"\n", s.data);

    section("str: self-referential operations are safe");
    str_assign(&s, "ab");
    str_append(&s, s.data); /* s aliases its own buffer; still correct */
    printf("\"ab\" appended to itself -> \"%s\"\n", s.data);

    section("str: replace");
    str_assign(&s, "the cat sat on the mat");
    str_replace(&s, "at", "og");
    printf("replace \"at\"->\"og\": \"%s\"\n", s.data);

    section("str: compare / find");
    str_init(&other);
    str_assign(&other, "the cog sog on the mog");
    printf("compare(s, other) == 0: %s\n", str_compare(&s, &other) == 0 ? "yes" : "no");
    printf("find(\"sog\"):     index %zu\n", str_find(&s, "sog"));
    printf("find(\"missing\"): %s\n",
           str_find(&s, "missing") == STR_NPOS ? "STR_NPOS (not found)" : "found");

    section("str: find_all (overlapping matches included)");
    {
        size_t *positions = NULL;
        size_t count = 0;
        size_t i;

        str_assign(&s, "banana");
        str_find_all(&s, "an", &positions, &count);
        printf("\"an\" in \"banana\" -> %zu match(es) at: ", count);
        for (i = 0; i < count; ++i) {
            printf("%zu ", positions[i]);
        }
        printf("\n");
        free(positions);
    }

    section("str: copy gives an independent buffer");
    str_assign(&s, "original");
    str_copy(&other, &s);
    str_assign(&other, "changed");
    printf("s = \"%s\", other = \"%s\"\n", s.data, other.data);

    str_free(&s);
    str_free(&other);
}

/* ================================================================== *
 * vec - dynamic generic vector                                        *
 * ================================================================== */

static int cmp_int(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

static void print_ints(const char *label, const Vec *v)
{
    size_t i;

    printf("%-24s[", label);
    for (i = 0; i < v->length; ++i) {
        printf("%d%s", *(const int *)vec_at_const(v, i), i + 1 < v->length ? ", " : "");
    }
    printf("]\n");
}

static void usage_vec(void)
{
    Vec v, clone;
    int value;
    int out;

    section("vec: init / push_back / get / set");
    vec_init(&v, sizeof(int));
    for (value = 10; value <= 50; value += 10) {
        vec_push_back(&v, &value);
    }
    print_ints("after push_back:", &v);

    vec_get(&v, 2, &out);
    printf("get(2) = %d\n", out);
    value = 999;
    vec_set(&v, 2, &value);
    print_ints("after set(2, 999):", &v);

    section("vec: insert / find / erase");
    value = 15;
    vec_insert(&v, 1, &value);
    print_ints("after insert(1, 15):", &v);
    printf("find(15) -> index %zu\n", vec_find(&v, &value, cmp_int));
    vec_erase(&v, 1, 1);
    print_ints("after erase(1, 1):", &v);

    section("vec: pop_back");
    vec_pop_back(&v, &out);
    printf("popped %d\n", out);
    print_ints("after pop_back:", &v);

    section("vec: sort (duplicate-heavy input stays fast)");
    vec_clear(&v);
    {
        int values[] = {5, 1, 5, 3, 5, 2, 5};
        size_t i;

        for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
            vec_push_back(&v, &values[i]);
        }
    }
    print_ints("before sort:", &v);
    vec_sort(&v, cmp_int);
    print_ints("after sort:", &v);

    section("vec: vec_at (mutable) vs vec_at_const (read-only)");
    {
        const Vec *cv = &v;

        printf("read via vec_at_const:  %d\n", *(const int *)vec_at_const(cv, 0));
        *(int *)vec_at(&v, 0) = -1; /* vec_at needs a non-const Vec* to write */
        printf("write via vec_at:       %d\n", *(int *)vec_at(&v, 0));
    }

    section("vec: clone gives an independent backing allocation");
    vec_init(&clone, sizeof(int));
    vec_clone(&clone, &v);
    value = 777;
    vec_set(&clone, 0, &value);
    print_ints("original untouched:", &v);
    print_ints("clone diverged:", &clone);

    section("vec: assign copies from a raw array, not another Vec");
    {
        int raw[] = {1, 2, 3, 4};

        vec_assign(&v, raw, 4);
        print_ints("after assign(raw, 4):", &v);
    }

    section("vec: elements are raw bytes only - referenced resources are\n"
            "     never duplicated or owned by the vector");
    {
        typedef struct {
            int id;
            const char *label;
        } Tagged;

        Vec tagged, tagged_clone;
        Tagged item;
        const Tagged *a;
        const Tagged *b;

        item.id = 1;
        item.label = "external string, not owned by the vector";

        vec_init(&tagged, sizeof(Tagged));
        vec_push_back(&tagged, &item);

        vec_init(&tagged_clone, sizeof(Tagged));
        vec_clone(&tagged_clone, &tagged);

        a = (const Tagged *)vec_at_const(&tagged, 0);
        b = (const Tagged *)vec_at_const(&tagged_clone, 0);
        printf("clone's label pointer == original's: %s (\"%s\")\n",
               a->label == b->label ? "yes" : "no", a->label);

        vec_free(&tagged_clone); /* does not touch the shared string */
        printf("original still readable after clone freed: \"%s\"\n",
               ((const Tagged *)vec_at_const(&tagged, 0))->label);

        vec_free(&tagged);
    }

    vec_free(&v);
    vec_free(&clone);
}

int main(void)
{
    section("str usage");
    usage_str();

    section("vec usage");
    usage_vec();

    return 0;
}
