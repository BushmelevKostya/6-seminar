/* heap-1.c */

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_BLOCKS 16
#define BLOCK_CAPACITY 1024

enum block_status { BLK_FREE = 0, BLK_ONE, BLK_FIRST, BLK_CONT, BLK_LAST };

struct heap {
    struct block {
        char contents[BLOCK_CAPACITY];
    } blocks[HEAP_BLOCKS];
    enum block_status status[HEAP_BLOCKS];
} global_heap = {0};

struct block_id {
    size_t       value;
    bool         valid;
    struct heap* heap;
};

struct block_id block_id_new(size_t value, struct heap* from) {
    return (struct block_id){.valid = true, .value = value, .heap = from};
}
struct block_id block_id_invalid() {
    return (struct block_id){.valid = false};
}

bool block_id_is_valid(struct block_id bid) {
    return bid.valid && bid.value < HEAP_BLOCKS;
}

/* Find block */

bool block_is_free(struct block_id bid) {
    if (!block_id_is_valid(bid))
        return false;
    return bid.heap->status[bid.value] == BLK_FREE;
}

/* Allocate */
size_t get_free_id(struct heap* heap, size_t count) {
    size_t i = 0;
    size_t start = 0;
    size_t current = 0;
    for (; current < count ; i++) {
        if (i == HEAP_BLOCKS) {
            break;
        }
        if (heap->status[i] == BLK_FREE) {
            if (!current) start = i;
            current += 1;
        }
        else {
            current = 0;
            start = 0;
        }
    }
    if (i < HEAP_BLOCKS || current == count) { return start; }
    return 17;
}

struct block_id block_allocate_one(struct heap* heap) {
    size_t id = get_free_id(heap, 1);
    if (id != -1) {
        heap->status[id] = BLK_ONE;
        return block_id_new(id, heap);
    }
    return block_id_invalid();
}

struct block_id block_allocate_by_id(struct heap* heap, enum block_status status, size_t id) {
    if (id < HEAP_BLOCKS) {
        heap->status[id] = status;
        return block_id_new(id, heap);
    }
    return block_id_invalid();
}

struct block_id block_allocate(struct heap* heap, size_t size) {
    if (size < 1) return (struct block_id) {0};
    size_t count = size / BLOCK_CAPACITY + 1;
    if (!(size % BLOCK_CAPACITY)) count -= 1;
    struct block_id bid = block_id_invalid();
    if (count > HEAP_BLOCKS) return bid;

    if (count == 1) bid = block_allocate_one(heap);
    else {
        size_t id = get_free_id(heap, count);
        bid = block_allocate_by_id(heap, BLK_FIRST, id);
        id += 1;
        for (size_t c = 0; c < count - 2; c++) {
            block_allocate_by_id(heap, BLK_CONT, id);
            id += 1;
        }
        block_allocate_by_id(heap, BLK_LAST, id);
    }
    return bid;
}

void block_free_one(struct block_id b) {
    b.heap->status[b.value] = BLK_FREE;
}

void block_free_by_id(struct block_id b, size_t id) {
    b.heap->status[id] = BLK_FREE;
}

/* Free */
void block_free(struct block_id b) {
    if (!block_is_free(b)) {
        if (b.heap->status[b.value] == BLK_ONE) {
            block_free_one(b);
        }
        else {
            block_free_one(b);
            size_t c = b.value + 1;
            for (; b.heap->status[c] == BLK_CONT; c++) {
                block_free_by_id(b, c);
            }
            block_free_by_id(b, c);
        }
    }
}

/* Printer */
const char* block_repr(struct block_id b) {
    static const char* const repr[] = {[BLK_FREE] = " .",
            [BLK_ONE] = " *",
            [BLK_FIRST] = "[=",
            [BLK_LAST] = "=]",
            [BLK_CONT] = " ="};
    if (b.valid)
        return repr[b.heap->status[b.value]];
    else
        return "INVALID";
}

void block_debug_info(struct block_id b, FILE* f) {
    fprintf(f, "%s", block_repr(b));
}

void block_foreach_printer(struct heap* h, size_t count,
                           void printer(struct block_id, FILE* f), FILE* f) {
    for (size_t c = 0; c < count; c++)
        printer(block_id_new(c, h), f);
}

void heap_debug_info(struct heap* h, FILE* f) {
    block_foreach_printer(h, HEAP_BLOCKS, block_debug_info, f);
    fprintf(f, "\n");
}
/*  -------- */

int main() {
    printf("test 1: allocate {(1), (2), (3)}, free {(2)}\n");
    struct block_id bid1 =block_allocate(&global_heap, 1000);
    struct block_id bid2 = block_allocate(&global_heap, 1000);
    struct block_id bid3 =block_allocate(&global_heap, 1000);
    heap_debug_info(&global_heap, stdout);

    block_free(bid2);
    heap_debug_info(&global_heap, stdout);
    block_free(bid1);
    block_free(bid3);

    printf("test 2: allocate {(1), (2-5), (6)}, free {(1), (2-5)}\n");
    bid1 =block_allocate(&global_heap, 1000);
    bid2 = block_allocate(&global_heap, 4000);
    bid3 =block_allocate(&global_heap, 1000);
    heap_debug_info(&global_heap, stdout);

    block_free(bid2);
    block_free(bid1);
    heap_debug_info(&global_heap, stdout);
    block_free(bid3);

    printf("test 3: allocate {(1-17)}\n");
    block_allocate(&global_heap, 17000);
    heap_debug_info(&global_heap, stdout);

    printf("test 4: allocate {(1-8), (9-15), (16-17)}\n");
    bid1 = block_allocate(&global_heap, 8000);
    bid2 = block_allocate(&global_heap, 7000);
    bid3 = block_allocate(&global_heap, 2000);
    heap_debug_info(&global_heap, stdout);

    block_free(bid1);
    block_free(bid2);
    heap_debug_info(&global_heap, stdout);

    printf("test 5: allocate {(1-8), (9-14), (15-16)}, free {(9-14)}, allocate {(9-15)}\n");
    bid1 = block_allocate(&global_heap, 8000);
    bid2 = block_allocate(&global_heap, 6000);
    bid3 = block_allocate(&global_heap, 2000);
    heap_debug_info(&global_heap, stdout);

    block_free(bid2);
    heap_debug_info(&global_heap, stdout);

    heap_debug_info(&global_heap, stdout);
    block_free(bid1);
    block_free(bid3);

    printf("test 6: allocate {(1) - size 1024, (2-3) - size 1025}\n");
    bid1 = block_allocate(&global_heap, 1024);
    bid2 = block_allocate(&global_heap, 1025);
    heap_debug_info(&global_heap, stdout);

    block_free(bid1);
    block_free(bid2);
    return 0;
}
