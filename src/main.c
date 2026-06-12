#include <stdio.h>
#include <kv.h>


int main(){
    kv_t *table = kv_init(2);
    printf("%ld\n", table->capacity);

    kv_put(table, "123", "cock");
    kv_put(table, "125", "cock");
    kv_put(table, "121", "clock");

    for (int i = 0; i < table->capacity; i++){
        if (table->entries[i].key){
            printf("%d: %s: %s\n",i, table->entries[i].key, table->entries[i].value);
        }
    }
}