#include "hashtable.h"
#define HASH_CREATE_ERROR "Error: previous made hashtable is not destoryed yet\n"
#define HASH_ERROR "Error: Hashtable doesn't exist --> (create_Hash)\n"
#define HASH_SIZE_ERROR "Error: requested size of a hashtable is too small\n"
#define THROW_ERROR(X) {fprintf(stderr, X); exit(1);} 

hash **_hash_table = NULL; // NOT_FOR_USE
size_t _hash_size; // NOT_FOR_USE

static unsigned int __index__(long long data){
    unsigned int inx = (unsigned int)(data % (long long)_hash_size); 
    return inx;
} 

void create_hash(size_t _size)
{
    if(_hash_table) THROW_ERROR(HASH_CREATE_ERROR)
    if(!_size) THROW_ERROR(HASH_SIZE_ERROR)
    _hash_table = calloc(sizeof(hash *), _size + 1); // +1 extra space for proper allocation
    _hash_size = _size;
}

void insert_in_hash(long long data)
{
    if(!_hash_table) THROW_ERROR(HASH_ERROR)
    unsigned int inx = __index__(data);
    hash *tmp = (hash *)malloc(sizeof(hash));
    tmp->data = data;
    tmp->next = _hash_table[inx];
    _hash_table[inx] = tmp;
}

bool search_in_hash(long long data)
{
    if(!_hash_table) THROW_ERROR(HASH_ERROR)
    unsigned int inx = __index__(data);
    hash *tmp = _hash_table[inx];
    while (tmp)
    {
        if (tmp->data == data)
            return true;
        tmp = tmp->next;
    }
    return false;
}

void destroy_hash(){
    if(!_hash_table) THROW_ERROR(HASH_ERROR)
    for (size_t i = 0; i < (_hash_size + 1); i++)
    {   
        hash *tmp = _hash_table[i];
        while(_hash_table[i]){
            tmp = _hash_table[i];
            _hash_table[i] = _hash_table[i]->next;
            free(tmp);
            tmp = NULL;
        }
    }
    free(_hash_table);
    _hash_table = NULL;
    _hash_size = 0;
}