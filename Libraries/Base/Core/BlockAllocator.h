//**********************************************************************
// Copyright Patrick Sweeney 2015-2019
// All Rights Reserved
//**********************************************************************
#pragma once
#include "Core.h"
#include <vector>
#include <list>

namespace Caustic
{
    //**********************************************************************
    // \brief CBlockAllocator is a simple block allocator for creating lots
    // of objects that are the same size.
    //**********************************************************************
    template <typename T>
    class CBlockAllocator
    {
        template <typename T> struct CBlock;
        
        //**********************************************************************
        // \brief CItem defines a single item in our block
        //**********************************************************************
        template <typename T>
        struct CItem
        {
            bool m_inuse;
            CBlock<T> *m_parent;
            T m_data;
        };

        //**********************************************************************
        // \brief CBlock defines a block of objects.
        //**********************************************************************
        template <typename T>
        struct CBlock
        {
            std::vector<CItem<T>> m_items;  //!< List of items
            int m_itemsFree;                //!< Number of items in the list that are free
            int m_nextFree;                 //!< Next item that is free. If unsure of next item, this will be -1

            CBlock(int blockSize)
            {
                m_items.resize(blockSize);
                m_itemsFree = blockSize;
                for (int i = 0; i < blockSize; i++)
                {
                    m_items[i].m_inuse = false;
                    m_items[i].m_parent = nullptr;
                }
                m_nextFree = 0;
            }
        };

        std::vector<CBlock<T>*> m_blocks;   //!< List of blocks
        int m_defaultBlockSize;             //!< Size of each item
        CBlock<T> *m_curBlock;              //!< Current block to allocate items from
    public:
        CBlockAllocator(int defaultBlockSize)
        {
            m_defaultBlockSize = defaultBlockSize;
            m_curBlock = new CBlock<T>(m_defaultBlockSize);
            m_blocks.push_back(m_curBlock);
        }

        T *Allocate()
        {
            // Search the current block for a free item
            if (m_curBlock->m_itemsFree > 0)
            {
                if (m_curBlock->m_nextFree != -1)
                {
                    m_curBlock->m_items[m_curBlock->m_nextFree].m_data = T();
                    m_curBlock->m_items[m_curBlock->m_nextFree].m_inuse = true;
                    m_curBlock->m_items[m_curBlock->m_nextFree].m_parent = m_curBlock;
                    T *p = &m_curBlock->m_items[m_curBlock->m_nextFree].m_data;
                    m_curBlock->m_itemsFree--;
                    m_curBlock->m_nextFree++;
                    if (m_curBlock->m_nextFree == m_defaultBlockSize)
                        m_curBlock->m_nextFree = -1;
                    return p;
                }
                for (int i = 0; i < m_defaultBlockSize; i++)
                    if (!m_curBlock->m_items[i].m_inuse)
                    {
                        m_curBlock->m_items[i].m_data = T();
                        m_curBlock->m_items[i].m_inuse = true;
                        m_curBlock->m_items[i].m_parent = m_curBlock;
                        m_curBlock->m_itemsFree--;
                        return &m_curBlock->m_items[i].m_data;
                    }
            }

            // We didn't find a free item so allocate a new block
            m_curBlock = new CBlock<T>(m_defaultBlockSize);
            m_blocks.push_back(m_curBlock);
            T *p = &m_curBlock->m_items[m_curBlock->m_nextFree].m_data;
            m_curBlock->m_items[m_curBlock->m_nextFree].m_inuse = true;
            m_curBlock->m_items[m_curBlock->m_nextFree].m_parent = m_curBlock;
            m_curBlock->m_itemsFree--;
            m_curBlock->m_nextFree++;
            return p;
        }

        void Free(T *p)
        {
            uint64 offset = (uint64)(uint64*)(&((CItem<T>*)nullptr)->m_data);
            CItem<T> *pItem = (CItem<T>*)((char*)p - offset);
            pItem->m_inuse = false;
            pItem->m_parent->m_itemsFree++;
            pItem->m_data.~T();
            pItem->m_data = T();
        }
    };
}
