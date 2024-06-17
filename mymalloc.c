#include "mymalloc.h"
#include <stddef.h>

#define MAX_MEMORY 250000
#define METADATA_SIZE sizeof(struct block)

struct block{
  size_t size;
  int free;
  struct block* next;
};

char memory[MAX_MEMORY];
struct block* freeList = (struct block*)memory;

void initialize(){
  freeList->size = MAX_MEMORY - METADATA_SIZE;
  freeList->free = 1;
  freeList->next = NULL;
}

void split(struct block* slot, size_t size){
  struct block* newone = (struct block*)((void*)slot + size + METADATA_SIZE);
  newone->size = slot->size - size - METADATA_SIZE;
  newone->free = 1;
  newone->next = slot->next;
  slot->size = size;
  slot->free = 0;
  slot->next = newone;
}

void* MyMalloc(size_t noOfBytes){
  struct block* current = freeList;

  if (!((void*)memory <= (void*)current && (void*)current <= (void*)(memory + MAX_MEMORY))) {
        initialize();
        current = freeList;
    }

  while ((((current->size) < noOfBytes) || ((current->free) == 0)) && (current->next != NULL)){
    current = current->next;
  }

  if((current->size) == noOfBytes){
    current->free = 0;
    return (void*)(current + 1);
  } else if((current->size) > (noOfBytes + METADATA_SIZE)){
    split(current, noOfBytes);
    return (void*)(current + 1);
  }
  return NULL; 
}

void merge(){
  struct block* current = freeList;

  while(current->next != NULL){
    if((current->free) && (current->next->free)){
      current->size += (current->next->size) + METADATA_SIZE;
      current->next = current->next->next;
    }
    current = current->next;
  }
}

void MyFree(void* ptr){
  if(((void*)memory <= ptr) && (ptr  <= (void*)(memory + MAX_MEMORY))){
    struct block* current = (struct block*)ptr;

    --current;
    current->free = 1;
    merge();
  }
}
