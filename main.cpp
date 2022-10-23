#include <iostream>

#include "Allocator.h"
#include <stdio.h>
#include <iostream>
#include <new>
#include <assert.h>

//using namespace std;


class alok
{
    GET_ALLOCATOR // определение аллокарота
};
IMP_ALLOCATOR(alok, 0, 0)


Allocator allocatorHeapBlocks(100);
Allocator allocatorHeapPool(100, 20);
char staticMemoryPool[100 * 20];
Allocator allocatorStaticPool(100, 20, staticMemoryPool);
AllocatorPot<alok, 20> allocatorStaticPool2;
static const int MAX_BLOCKS = 100000;
static const int MAX_BLOCK_SIZE = 1024;
void* memoryPtrs[MAX_BLOCKS];
void* memoryPtrs2[MAX_BLOCKS];
AllocatorPot<char[MAX_BLOCK_SIZE], MAX_BLOCKS*2> allocatorStaticPoolBenchmark;
Allocator allocatorHeapBlocksBenchmark(MAX_BLOCK_SIZE);

static void out_of_memory()
{
    // new-handler function called by Allocator when pool is out of memory
    assert(0);
}

typedef void* (*AllocFunc)(int size);
typedef void (*DeallocFunc)(void* ptr);
void Benchmark(const char* name, AllocFunc allocFunc, DeallocFunc deallocFunc);
void* AllocHeap(int size);
void DeallocHeap(void* ptr);
void* AllocStaticPool(int size);
void DeallocStaticPool(void* ptr);
void* AllocHeapBlocks(int size);
void DeallocHeapBlocks(void* ptr);



int main()
{
//    cout << "Hello World!" << endl;
//    return 0;

    std::set_new_handler(out_of_memory);

    // Allocate MyClass using fixed block allocator
    alok* al = new alok();
    delete al;

    void* memory1 = allocatorHeapBlocks.allocate(100);
    allocatorHeapBlocks.dellocate(memory1);

    void* memory2 = allocatorHeapBlocks.allocate(100);
    allocatorHeapBlocks.dellocate(memory2);


    Benchmark("Heap (Run 1)", AllocHeap, DeallocHeap);
    Benchmark("Static Pool (Run 1)", AllocStaticPool, DeallocStaticPool);
    Benchmark("Heap Blocks (Run 1)", AllocHeapBlocks, DeallocHeapBlocks);

    return 0;
}



void* AllocHeap(int size)
{
    return new char[size];
}

//------------------------------------------------------------------------------
// DeallocHeap
//------------------------------------------------------------------------------
void DeallocHeap(void* ptr)
{
    delete [] ptr;
}

//------------------------------------------------------------------------------
// AllocStaticPool
//------------------------------------------------------------------------------
void* AllocStaticPool(int size)
{
    return allocatorStaticPoolBenchmark.allocate(size);
}

//------------------------------------------------------------------------------
// DeallocStaticPool
//------------------------------------------------------------------------------
void DeallocStaticPool(void* ptr)
{
    allocatorStaticPoolBenchmark.dellocate(ptr);
}

//------------------------------------------------------------------------------
// AllocHeapBlocks
//------------------------------------------------------------------------------
void* AllocHeapBlocks(int size)
{
    return allocatorHeapBlocksBenchmark.allocate(size);
}

//------------------------------------------------------------------------------
// DeallocHeapBlocks
//------------------------------------------------------------------------------
void DeallocHeapBlocks(void* ptr)
{
    allocatorHeapBlocksBenchmark.dellocate(ptr);
}

//------------------------------------------------------------------------------
// Benchmark
//------------------------------------------------------------------------------
void Benchmark(const char* name, AllocFunc allocFunc, DeallocFunc deallocFunc)
{
//#if WIN32
    LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds, TotalElapsedMicroseconds= {0};
    LARGE_INTEGER Frequency;

    SetProcessPriorityBoost(getCurrentProcess(), true);

    QueryPerformanceFrequency(&Frequency);

    // Allocate MAX_BLOCKS blocks MAX_BLOCK_SIZE / 2 sized blocks
    QueryPerformanceCounter(&StartingTime);
    for (int i=0; i<MAX_BLOCKS; i++)
        memoryPtrs[i] = allocFunc(MAX_BLOCK_SIZE / 2);
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
    std::cout << name << " allocate time: " << ElapsedMicroseconds.QuadPart << std::endl;
    TotalElapsedMicroseconds.QuadPart += ElapsedMicroseconds.QuadPart;

    // Deallocate MAX_BLOCKS blocks (every other one)
    QueryPerformanceCounter(&StartingTime);
    for (int i=0; i<MAX_BLOCKS; i+=2)
        deallocFunc(memoryPtrs[i]);
    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
    std::cout << name << " deallocate time: " << ElapsedMicroseconds.QuadPart << std::endl;
    TotalElapsedMicroseconds.QuadPart += ElapsedMicroseconds.QuadPart;



    std::cout << name << " TOTAL TIME: " << TotalElapsedMicroseconds.QuadPart << std::endl;

    SetProcessPriorityBoost(GetCurrentProcess(), false);
#endif
}


