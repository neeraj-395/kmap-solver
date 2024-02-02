#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct __hash__
{
    long long data;
    struct __hash__ *next;
} hash;

void destroy_hash();
void insert_in_hash(long long data);
bool search_in_hash(long long data);
void create_hash(size_t _size);

#endif