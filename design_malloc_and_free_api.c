/*
NAME:ISHWAR M UPPAR
DATE:
DESCRIPTION:
SAMPLE INPUT:
SAMPLE OUTPUT:
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdint.h>

typedef struct Block 
{
    size_t size;
    int free;
    struct Block* next;
}Block;


Block* heapStart = NULL;
size_t heapSize = 128 * 1024;
size_t minBlockSize = 1024;

void initHeap() 
{
    heapStart = (Block*) malloc(heapSize);
    heapStart->size = heapSize;
    heapStart->free = 1;
    heapStart->next = NULL;
}

Block* findBestFit(size_t blockSize) 
{
    Block* current = heapStart;
    Block* bestFit = NULL;
    size_t bestFitSize = heapSize;

    while (current != NULL) 
    {
        if (current->free && current->size >= blockSize && current->size < bestFitSize) 
	{
            bestFit = current;
            bestFitSize = current->size;
        }
        current = current->next;
    }
    return bestFit;
}

void splitBlock(Block* block, size_t blockSize) 
{
    Block* newBlock = (Block*) ((char*) block + blockSize);
    newBlock->size = block->size - blockSize;
    newBlock->free = 1;
    newBlock->next = block->next;
    block->size = blockSize;
    block->free = 0;
    block->next = newBlock;
}

void* malloc(size_t blockSize) 
{
    if (blockSize < minBlockSize) 
    {
        blockSize = minBlockSize;
    }

    if (heapStart == NULL) 
    {
        initHeap();
    }

    Block* bestFit = findBestFit(blockSize);

    if (bestFit == NULL) 
    {
        printf("Out of memory\n");
        return NULL;
    }

    if (bestFit->size > blockSize) 
    {
        splitBlock(bestFit, blockSize);
    }

    bestFit->free = 0;

    return (void*) ((char*) bestFit + sizeof(Block));
}

void mergeBlocks(Block* block1, Block* block2) 
{
    block1->size += block2->size + sizeof(Block);
    block1->next = block2->next;
}


void free(void* ptr) 
{
    if (ptr == NULL) 
    {
        printf("Invalid pointer\n");
        return;
    }

    Block* block = (Block*) ((char*) ptr - sizeof(Block));

    if (block->free) 
    {
        printf("Block is already free\n");
        return;
    }

    block->free = 1;

    if (block->next != NULL && block->next->free) 
    {
        mergeBlocks(block, block->next);
    }

    if (block != heapStart && ((Block*) ((char*) block - sizeof(Block)))->free) 
    {
        mergeBlocks((Block*) ((char*) block - sizeof(Block)), block);
    }
}

int main() 
{
    int* ptr1 = (int*) malloc(sizeof(int) * 128);
    void* ptr2 = malloc(sizeof(uint8_t) * 1000);
    char* ptr3 = (char*) malloc(128 * 8 * 1024);

    free(ptr1);
    free(ptr2);
    free(ptr3);

    return 0;
}
