/************************************************************************
(Copyright (c) 2009-2010 )
*	File Name		: ConcurQueue.h
*	Version Number	:
*	Description		: 并行容器
*	Date			: 
*	Chage log		:                                       
************************************************************************/

#include <assert.h>
//#include <windows.h>
#include <list>
#pragma once

typedef struct tagNodeInfor   
{   
	WORD			wStatus;
	tagNodeInfor*	pNext;
#define NODE_WAIT		0
#define NODE_RUN		1
}NODE_INFOR,*PNODE_INFOR;

template<class TYPE>
class CConcurQueue
{
public:
	CConcurQueue()
	{
		m_pHeadDataQueu		= NULL;
		m_pTailDataQueu		= NULL;
		m_pHeadFreeList		= NULL;
		m_hHeap				= NULL;
		m_dwCurApplyTime	= 0;
		m_nTotalSize		= 100;
		m_dwMemoryType		= 0;

		::InitializeCriticalSection(&m_QueueLock);
//		::SetCriticalSectionSpinCount(&m_QueueLock, 4000);
	}
	virtual ~CConcurQueue()
	{
		::DeleteCriticalSection(&m_QueueLock);

		std::list<NODE_INFOR* >::iterator iter;   
		for(iter = m_memoryHeadList.begin(); iter != m_memoryHeadList.end(); ++iter)   
		{   
			NODE_INFOR* block =  *iter;
			HeapFree(m_hHeap,  HEAP_NO_SERIALIZE, block);
		}   
	}
public:
	void SetMemoryType(DWORD dwMemoryType)
	{
		m_dwMemoryType = dwMemoryType;
	}
	void SetTotalSize(DWORD dwTotalSize)
	{
		m_nTotalSize = dwTotalSize;
	}
	void* MemoryHeapAlloc(size_t size)
	{
		if (NULL == m_hHeap)
		{
			m_hHeap = HeapCreate(HEAP_NO_SERIALIZE, 0, 0);
			if (NULL == m_hHeap)
				return NULL;
		}
		void* pHeap = HeapAlloc(m_hHeap, 0, size);
	
		return (pHeap);
	}
	bool IsEmpty()
	{
		return m_pHeadDataQueu==NULL && m_pTailDataQueu==NULL;
	}
	
	bool Enqueue(TYPE* Value)
	{
		::EnterCriticalSection(&m_QueueLock);

		NODE_INFOR* pNodeInfor = NULL; 
		if(NULL != m_pHeadFreeList)  
		{
			pNodeInfor		= m_pHeadFreeList;
			m_pHeadFreeList = pNodeInfor->pNext;
		}
		else
		{
			m_ObjectSize = sizeof(NODE_INFOR) + sizeof(TYPE);   
			// 申请空间   
			NODE_INFOR* pNewBlock = NULL;
			
			try
			{
				pNewBlock = static_cast<NODE_INFOR* >(MemoryHeapAlloc(m_nTotalSize*m_ObjectSize)); 
				memset(pNewBlock, 0, m_nTotalSize*m_ObjectSize);
			}
			catch (...)
			{
				::LeaveCriticalSection(&m_QueueLock);
				return false;  	
			}

			m_dwCurApplyTime++;
			// 初始化空闲空间列表   
			NODE_INFOR* pCurr = NULL;
			NODE_INFOR* pNext = NULL;
			for(int i=0; i<m_nTotalSize-1; i++)   
			{
				pCurr = (NODE_INFOR* )((char* )pNewBlock+i*m_ObjectSize);
				pNext = (NODE_INFOR* )((char* )pNewBlock+(i+1)*m_ObjectSize);

				pCurr->pNext	= pNext;
				//pCurr->wSeq		= m_dwCurApplyTime;
				//pCurr->wIdx		= i;
				pCurr->wStatus	= NODE_WAIT;
			}
			pNext->pNext	= NULL;
			pNext->wStatus	= NODE_WAIT;

			//if (m_dwMemoryType!=0)
			//	::PostMessage(m_hParentWnd, WM_USER+6103, MAKEWPARAM(m_dwMemoryType, m_dwCurApplyTime), m_nTotalSize*m_ObjectSize);
			// 保存首址   
			m_memoryHeadList.push_back(pNewBlock);  

			pNodeInfor		= pNewBlock;  
			m_pHeadFreeList = pNodeInfor->pNext;
		}
		
		assert(pNodeInfor->wStatus == NODE_WAIT);
		pNodeInfor->pNext	= NULL;
		pNodeInfor->wStatus	= NODE_RUN;
		memcpy((char*)pNodeInfor+sizeof(NODE_INFOR), Value, sizeof(TYPE));

		// 添入队列
		if (IsEmpty())
			m_pTailDataQueu = m_pHeadDataQueu = pNodeInfor;
		else
		{
			m_pTailDataQueu->pNext	= pNodeInfor;
			m_pTailDataQueu			= pNodeInfor;
		}
		::LeaveCriticalSection(&m_QueueLock);
		return true;
	}
	bool Dequeue(TYPE* Value)
	{
		::EnterCriticalSection(&m_QueueLock);
		if (IsEmpty())
		{
			::LeaveCriticalSection(&m_QueueLock);
			return false;
		}

		NODE_INFOR* pFreeBlock	= m_pHeadDataQueu;
		m_pHeadDataQueu			= pFreeBlock->pNext;
		if (pFreeBlock == m_pTailDataQueu)					// 原队中只有一个结点,删去后队列变空,此时队头指针已为空
			m_pTailDataQueu = NULL;

		memcpy(Value, (char*)pFreeBlock+sizeof(NODE_INFOR), sizeof(TYPE));

		assert(pFreeBlock->wStatus == NODE_RUN);
		pFreeBlock->wStatus	= NODE_WAIT;
		// 回收内存
		pFreeBlock->pNext = m_pHeadFreeList;   
		m_pHeadFreeList   = pFreeBlock;  

		::LeaveCriticalSection(&m_QueueLock);
		return true;
	}
public:
	NODE_INFOR*		m_pHeadFreeList;
	NODE_INFOR*		m_pHeadDataQueu;
	NODE_INFOR*		m_pTailDataQueu;
	HANDLE			m_hHeap;
	DWORD			m_dwCurApplyTime;
	DWORD			m_dwMemoryType;	
	size_t			m_ObjectSize;
	size_t			m_nTotalSize;

	CRITICAL_SECTION		m_QueueLock;		
	std::list<NODE_INFOR* >	m_memoryHeadList;		// 申请空间首址向量 
};