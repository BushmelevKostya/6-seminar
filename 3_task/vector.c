/* vector.c */

#include <inttypes.h>
#include <malloc.h>
#include <stdio.h>

#define PRINT_INT64_T "lld"

struct vector {
    int64_t* array;
    size_t capacity;
    size_t count;
};

struct vector* create(size_t capacity) {
    struct vector* vector = malloc(sizeof(struct vector));
    vector->array = malloc(sizeof(int64_t) * capacity);
    vector->count = 0;
    vector->capacity = capacity;
    return vector;
}

int64_t get_by_id(struct vector* s, size_t id) {
    return s->array[id];
}

void set_by_id(struct vector* s, size_t id, int64_t val) {
    s->array[id] = val;
}

size_t get_count(struct vector* s) {
    return s->count;
}

size_t get_capacity(struct vector* s) {
    return s->capacity;
}

static int64_t square(int64_t val) {
    return val * val;
}

static void print(int64_t val, FILE* file) {
    fprintf(file, "%" PRINT_INT64_T " ", val);
}

static void foreach(struct vector* v, int64_t (f)(int64_t)) {
    for (size_t i = 0; i < v->count; i++) {
        v->array[i] = f(v->array[i]);
    }
}

static void foreach_void(struct vector* v, FILE* file, void (f)(int64_t , FILE*)) {
    for (size_t i = 0; i < v->count; i++) {
        f(v->array[i], file);
    }
}

void square_array(struct vector* v) {
    foreach(v, square);
}

void print_array(struct vector* v, FILE* file) {
    foreach_void(v, file, print);
}

static void foreach_add(struct vector* target, struct vector* other, void (f)(struct vector* s, int64_t val)) {
    for (size_t i = 0; i < other->count; i++) {
        f(target, other->array[i]);
    }
}
//elements set change of array max size
//elements > 0 -> enlarge
//elements = 0 -> save
//elements < 0 -> reduce
int64_t* change_size(struct vector* s, size_t elements) {
    s->capacity = s->capacity + elements;
    if (!elements) return s->array;
    return realloc(s->array, sizeof(int64_t) * s->capacity);
}

void add(struct vector* s, int64_t val) {
    if (s->capacity == s->count) s->array = change_size(s, 10);
    s->array[s->count++] = val;
}

void add_array(struct vector* target, struct vector* other) {
    if (target->capacity < target->count + other->count) target->array = change_size(target, other->count + 10);
    foreach_add(target, other, add);
    target->count += other->count;
}

void free_array(struct vector* s) {
    free(s->array);
    free(s);
}
