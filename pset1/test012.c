#include "m61.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
// Realloc.

const char data[10] = "1234567890";

int main() {
    char* p = (char*) realloc(NULL, 10);
    printf("%p a", p);
    assert(p != NULL);
    memcpy(p, data, 10);
    printf("%p b", p);
    p = (char*) realloc(p, 12);
    assert(p != NULL);
    printf("%p c", p);
    assert(memcmp(data, p, 10) == 0);
    p = (char*) realloc(p, 0);
    assert(p == NULL);
    m61_printstatistics();
}

//! malloc count: active          0   total          2   fail          0
//! malloc size:  active          0   total         22   fail          0
