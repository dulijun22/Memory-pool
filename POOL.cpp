#include<iostream>
#include<string>
/*
内存池：通过自己重载new/delete来实现自主内存管理，用队列来实现
new
		1.开辟内存
		2.初始化
delete 
		1.释放资源
		2.释放内存
*/

int* pool = NULL;
void* operator new(size_t size)   //重载new运算符
{
	if (pool == NULL)
	{
		pool = (int*)new char[(size + 4)* 10];
		int* pCur = pool;
		for (pCur; pCur < pool + 10 - 1; pCur = pCur + 1)
		{
			pCur->pnext = pCur + 1;
		}
		pCur->pnext = NULL;
	}
	void* prt = pool;
	pool = pool->pnext;
	return prt;
}

void operator delete(void* ptr);   //重载delete运算符
int main()
{
	int* p = new int;
	return 0;
}

class Queue;
const int POOL_SIZE = 10;
class QueueItem
{
public:
	QueueItem(int val = 0) :mdata(val), pnext(NULL){}
	void* operator new(size_t size)
	{
		if (pool == NULL)
		{
			pool =(QueueItem*)new char[size * POOL_SIZE];
			QueueItem* pCur = pool;
			for (pCur; pCur < pool + POOL_SIZE - 1; pCur = pCur + 1)
			{
				pCur->pnext = pCur + 1;
			}
			pCur->pnext = NULL;
		}
		void* prt = pool;
		pool = pool->pnext;
		return prt;
	}

	void operator delete(void* ptr)
	{
		if (ptr == NULL)
		{
			return;
		}
		QueueItem* pptr = (QueueItem*)ptr;
		pptr->pnext = pool;
		pool = pptr;
	}
	friend class Queue;
private:
	int mdata;
	QueueItem* pnext;
	static QueueItem* pool;
};
QueueItem* QueueItem::pool = NULL;
class Queue
{
public:
	Queue()
	{
		pfront = ptail = new QueueItem();
	}
	~Queue()
	{
		QueueItem* pCur = pfront;
		QueueItem* pNext = pCur;
		while (pCur != NULL)
		{
			pNext = pCur->pnext;
			delete pCur;
			pCur = pNext;
		}
		pfront = ptail = NULL;
	}

	void push(int val)
	{
		QueueItem* pnewitem = new QueueItem(val);
		ptail->pnext = pnewitem;
		ptail = ptail->pnext;
	}

	void pop()
	{
		if (empty())
		{
			std::exception("queue is empty!");
		}
		QueueItem* pCur = pfront->pnext;
		if (pCur == ptail)
		{
			pfront->pnext = pCur->pnext;
			ptail = pfront;
		}
		else
		{
			pfront->pnext = pCur->pnext;
		}
		delete pCur;
	}
	int front()
	{
		if (empty())
		{
			std::exception("queue is empty!");
		}
		return pfront->pnext->mdata;
	}
	int back()
	{
		if (empty())
		{
			std::exception("queue is empty!");
		}
		return ptail->mdata;
	}
	bool empty()
	{
		if (pfront == NULL)
		{
			throw std::exception("queue is empty!");
		}
		return pfront == ptail;
	}
private:
	QueueItem* pfront;
	QueueItem* ptail;
};



int main()
{
	Queue que;
	for (int i = 0; i < 1000; i++)
	{
		que.push(i + 1);
	}
	for (int i = 0; i < 1000; i++)
	{
		int rt = que.front();
		std::cout << rt << " ";
		que.pop();
	}
	std::cout << std::endl;
	return 0;
}
