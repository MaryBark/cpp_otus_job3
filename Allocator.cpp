#include "Allocator.h"

Allocator::Allocator(size_t size,
                     int objects,
                     char* memory,
                     const char* name):
    m_blockSize(size < sizeof(long*) ? sizeof(long*):size),
    m_objectSize(size),
    m_maxObjects(objects),
    m_pHead(NULL),
    m_poolIndex(0),
    m_blockCnt(0),
    m_blocksInUse(0),
    m_allocations(0),
    m_deallocations(0),
    m_name(name)
{
    if (m_maxObjects)
    {
        if (memory)
        {
            m_pPool = memory;
            m_allocatorMode = STATIC_POOL;
        }
        else
        {
            m_pPool = (char*)new char[m_blockSize * m_maxObjects];
            m_allocatorMode = HEAP_POOL;
        }
    }
    else
        m_allocatorMode = HEAP_BLOCKS;
}

//-------------------------------------------------------------
Allocator::~Allocator()
{

}

// метод алокации
void* Allocator::allocate(size_t size)
{
    assert(size <= m_objectSize);

//    if(size <= m_objectSize)
//        return;

    void* pBlock = getBlok();
    if (!pBlock)
    {
        if (m_maxObjects)
        {
            if(m_poolIndex < m_maxObjects)
            {
                pBlock = (void*)(m_pPool + (m_poolIndex++ * m_blockSize));
            }
            else
            {
                std::new_handler handler = std::set_new_handler(0);
                std::set_new_handler(handler);

                if (handler)
                    (*handler)();
                else
                    assert(0);
            }
        }
        else
        {
            m_blockCnt++;
            pBlock = (void*)new char[m_blockSize]; // преобразование типа
        }
    }

    m_blocksInUse++;
    m_allocations++;

    return pBlock;
}

// метод делокации
void Allocator::dellocate(void* size)
{
    Push(size);
    m_blocksInUse--;
    m_deallocations++; // добавляем следующий блок
}

void Allocator::Push(void *Memory)
{
    Block* pBlock = (Block*)Memory;
    pBlock->pNext = m_pHead;
    m_pHead = pBlock;
}

void *Allocator::getBlok()
{
    Block* pBlock = NULL;

    if (m_pHead)
    {
        pBlock = m_pHead;
        m_pHead = m_pHead->pNext;
    }

    return (void*)pBlock;
}
