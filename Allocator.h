#ifndef ALLOCATOR_H
#define ALLOCATOR_H


#include <stddef.h>
#include <stdfix.h>
#include <new>
#include <assert.h>

//#if WIN32
//    #include "windows.h"
//#else

////////////////////////////////////
/// Allocator class
///////////////////////////////////


class Allocator
{
public:
    Allocator(size_t size, int objects=0, char* memory = NULL, const char* name=NULL);
    ~Allocator();

    // метод алокации
    void* allocate(size_t size);

    // метод делокации
    void dellocate(void* size);

    // пуееукы
    const char* getName() { return m_name; }
    size_t      getBlockSize() { return m_blockSize; }
    int         getBlockCount() { return m_blockCnt; }
    int         getBlocksInUse() { return m_blocksInUse; }
    int         getAllocations() { return m_allocations; }
    int         getDeallocations() { return m_deallocations; }

private: // полу класса

    // функции для выделения памяти
    void Push(void* Memory);

    // выдает блок памяти
    void* getBlok();

    // выделяем структуря для сохранения блока памяти
    struct Block
    {
        Block* pNext;
    };

    // перечисление для vjldv
    enum AllocatorPeremen { HEAP_BLOCKS, HEAP_POOL, STATIC_POOL };

    const size_t m_blockSize;
    const size_t m_objectSize;
    const int m_maxObjects;
    AllocatorPeremen m_allocatorMode;
    Block* m_pHead;
    char* m_pPool;
    int m_poolIndex;
    int m_blockCnt;
    int m_blocksInUse;
    int m_allocations;
    int m_deallocations;
    const char* m_name;
};



////////////////////////////////////
/// AllocatorPool class
///////////////////////////////////

template <class T, int obj>
class AllocatorPot : public Allocator
{
public:
    AllocatorPot() : Allocator(sizeof(T), obj, m_memory)
    {
    }
private:
    char m_memory[sizeof(T) * obj];
};


#define GET_ALLOCATOR \
    public: \
        void* operator new(size_t size) { \
            return _allocator.allocate(size); \
        } \
        void operator delete(void* pObject) { \
            _allocator.dellocate(pObject); \
        } \
    private: \
        static Allocator _allocator;
//#endif

/// не знаю, зачем это надо, но без этого не работает(((
#define IMP_ALLOCATOR(class, objects, memory) \
    Allocator class::_allocator(sizeof(class), objects, memory, #class);

#endif



#endif // ALLOCATOR_H
