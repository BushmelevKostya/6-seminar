#include <inttypes.h>
#include <stdio.h>

#ifndef INC_6_SEMINAR_VECTOR_H
#define INC_6_SEMINAR_VECTOR_H

#define PRINT_INT64_T "lld"

struct vector;

struct vector* create(size_t capacity);

int64_t get_by_id(struct vector *s, size_t id);

void set_by_id(struct vector *s, size_t id, int64_t val);

size_t get_count(struct vector *s);

size_t get_capacity(struct vector *s);

void square_array(struct vector *v);

void print_array(struct vector *v, FILE *file);

int64_t *change_size(struct vector *s, size_t elements);

void add(struct vector *s, int64_t val);

void add_array(struct vector *target, struct vector *other);

void free_array(struct vector *s);

static struct array_interface {
    struct vector* (*create)(size_t capacity);
    int64_t (*get)(struct vector *s, size_t id);
    void (*set)(struct vector *s, size_t id, int64_t val);
    size_t (*get_count)(struct vector *s);
    size_t (*get_capacity)(struct vector *s);
    void (*square)(struct vector *v);
    void (*print)(struct vector *v, FILE *file);
    int64_t* (*change_size)(struct vector *s, size_t elements);
    void (*add)(struct vector *s, int64_t val);
    void (*add_array)(struct vector *target, struct vector *other);
    void (*free)(struct vector *s);
} const array = {
        create,
        get_by_id,
        set_by_id,
        get_count,
        get_capacity,
        square_array,
        print_array,
        change_size,
        add,
        add_array,
        free_array
};

#endif //INC_6_SEMINAR_VECTOR_H
