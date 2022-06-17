#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = strdup(key);
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    if((map->size / (double) map->capacity) >= 0.7)
        enlarge(map);

    long pos = hash(key, map->capacity);
    while(map->buckets[pos] && map->buckets[pos]->key) {
        pos++;
        if(pos == map->capacity) pos = 0;
    }
    map->buckets[pos] = createPair(key, value);
    map->size++;
}

void enlarge(HashMap * map) {
    Pair **old_buckets = map->buckets;
    long old_size = map->size;
    map->capacity *= 2;
    map->buckets = (Pair **) calloc(map->capacity, sizeof(Pair *));
    map->size = 0;

    int i = 0;
    while(map->size < old_size) {
        if(old_buckets[i])
            insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
        i++;
    }
}


HashMap * createMap(long capacity) {
    HashMap *map = (HashMap *) malloc(sizeof(HashMap));
    map->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
    map->capacity = capacity;
    map->current = -1;
    map->size = 0;
    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    Pair *pair = searchMap(map, key);
    if(pair) {
        pair->key = NULL;
        map->size--;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    long pos = hash(key, map->capacity);
    while(map->buckets[pos] && map->buckets[pos]->key) {
        if(is_equal(map->buckets[pos]->key, key)) {
            map->current = pos;
            return map->buckets[pos];
        }
        pos++;
        if(pos == map->capacity) pos = 0;
    }    
    return NULL;
}

Pair * firstMap(HashMap * map) {
    if(map->size == 0) return NULL;
    map->current = 0;
    while(map->buckets[map->current] == NULL || map->buckets[map->current]->key == NULL) {
        map->current++;
    }
    return map->buckets[map->current];
}

Pair * nextMap(HashMap * map) {
    if(map->size <= 1) return NULL;
  
    map->current++;
    if(map->current == map->capacity) return NULL;
    while(map->buckets[map->current] == NULL || map->buckets[map->current]->key == NULL) {
        map->current++;
        if(map->current == map->capacity) return NULL;
    }
    return map->buckets[map->current];
}

long getMapCurrent(HashMap* map){
    return map->current;
}