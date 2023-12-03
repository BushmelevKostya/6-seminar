#include "vector.h"

int main() {
    struct vector* v = array.create(2);
    array.add(v,1);
    array.add(v,2);

    printf("array: ");
    array.print(v, stdout);

    printf("\nfirst element: %lld", array.get(v, 0));
    printf("\ncount: %lld", array.get_count(v));
    printf("\ncapacity: %lld", array.get_capacity(v));

    array.add(v, 3);
    printf("\n\narray: ");
    array.print(v, stdout);
    printf("\ncapacity: %lld", array.get_capacity(v));

    struct vector* ov = array.create(10);
    array.add(ov, 5);
    array.add(ov, 5);
    array.add(ov, 5);
    array.add(ov, 5);
    array.add(ov, 5);
    array.add(ov, 5);
    array.add(ov, 5);
    array.add(ov, 5);
    array.add(ov, 5);
    array.add(ov, 5);
    printf("\n\nother array: ");
    array.print(ov, stdout);
    array.add_array(v, ov);
    printf("\narray after extension: ");
    array.print(v, stdout);
    printf("\ncapacity: %lld", array.get_capacity(v));
    return 0;
}
