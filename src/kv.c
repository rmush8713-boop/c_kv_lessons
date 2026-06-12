#include <kv.h>
#include <stdlib.h>
#include <string.h>

size_t hash(char *value, int capacity){
    size_t hash = 0x6942069;

    while (*value)
    {
        hash ^= *value;
        hash = hash << 8;
        hash += *value;

        value++;
    }

    return hash % capacity;
    
}

int kv_free(kv_t *db){
    if (!db) return -1;

    for (int i = 0; i < db->capacity; i++){
        kv_entry_t *entry = &db->entries[i];
        if (entry->key && entry->key != (void*)TOMBSTONE){
            free(entry->key);
            free(entry->value);
            entry->key = NULL;
            entry->value = NULL;
        }
    }
    free(db->entries);
    free(db);
    return 0;   
}

int kv_delete(kv_t *db, char *key){
    if (!db || !key) return -1;

    size_t indx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity - 1; i++){
        size_t real_indx = (indx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_indx];

        if (entry->key == NULL) return -1;

        if (entry->key != TOMBSTONE && !strcmp(entry->key, key)){
            free(entry->key);
            free(entry->value);
            entry->key = TOMBSTONE;
            db->count--;
            return real_indx;
        }

    }
    return -1;
}

int kv_put(kv_t *db, char *key, char *value){
    if (!db || !key || !value) return -1;

    size_t indx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity - 1; i++){
        size_t real_indx = (indx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_indx];

        if (entry->key && entry->key != (void*)TOMBSTONE && !strcmp(entry->key, key)){
            char *newval = strdup(value);
            if (!newval) return -1;
            entry->value = newval;
            return 0;
        }

        if (!entry->key || entry->key == (void*)TOMBSTONE) {
            char *newval = strdup(value);
            char *newkey = strdup(key);
            if (!newkey || !newval){
                free(newkey);
                free(newval);
                return -1;
            }
            entry->key = newkey;
            entry->value = newval;
            db->count++;
            return 0;
        }

    }
    return -2;
}

char  *kv_get(kv_t *db, char *key){
    if (!db|| !key) return NULL;
    size_t indx = hash(key, db->capacity);

    for (int i = 0; i < db->capacity - 1; i++){
        size_t real_indx = (indx + i) % db->capacity;

        kv_entry_t *entry = &db->entries[real_indx];
        if (entry->key == NULL) return NULL;

        if (entry->key && entry->key != (void*)TOMBSTONE && !strcmp(entry->key, key)){
            return entry->value;
        }
    }
    return NULL;
}

kv_t *kv_init(size_t capacity){
    if (capacity == 0) return NULL;

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL) return NULL;

    table->capacity = capacity;
    table->count = 0;

    table->entries = calloc(sizeof(kv_entry_t), capacity);
    if (table->entries == NULL) return NULL;
    
    return table;
}