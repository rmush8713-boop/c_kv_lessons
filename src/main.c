#include <stdio.h>
#include <kv.h>


int main(){
    kv_t *hui = kv_init(123);
    printf("%ld\n", hui->capacity);
}