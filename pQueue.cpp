#include "pQueue.h"

pQueue::pQueue()
{
	front = NULL;
	length = 0;
}

pQueue::~pQueue()
{
	if (front != NULL)
	{
		pQueueMember *tDelete;
		while (length > 1)
		{
			tDelete = front;
			front = front->next;
			delete tDelete;
			length = length - 1;
		}
		tDelete = front;
		delete tDelete;
	}
}
void pQueue::enqueue(std::string contents, int priority)
{
	pQueueMember *tNew;
	tNew = new pQueueMember;
	tNew->contents = contents;
	tNew->priority = priority;
	insert(tNew);
	length = length + 1;
}

void pQueue::insert(pQueueMember *tNew)
{
	int tLength = length;
	if (length == 0)
	{
		tNew->next = NULL;
		front = tNew;
		back = tNew;
	}
	else
	{
		if (tNew->priority < front->priority)
		{
			tNew->next = front;
			front = tNew;
		}
		else
		{
			pQueueMember *tInsert = front;
			bool found = false;
			while (tLength > 1 && !found)
			{
				if (tNew->priority < tInsert->next->priority)
				{
					tNew->next = tInsert->next;
					tInsert->next = tNew;
					found = true;
				}
				else
				{
					tLength = tLength - 1;
					tInsert = tInsert->next;
				}
			}
			if (!found)
			{
				back->next = tNew;
				back = tNew;
				back->next = NULL;
			}
		}
    }
}

void pQueue::dequeue(std::string& contents, int& priority)
{
	if (length > 0)
	{
		pQueueMember *tDelete = front;
		contents = front->contents;
		priority = front->priority;
		front = front->next;
		delete tDelete;
		length = length - 1;
	}
	else
	{
		contents = "Empty Queue!!";
		priority = -1;
	}
}

void pQueue::print()
{
	pQueueMember *tPrint = front;
	for (int i = 1; i < length; i = i + 1)
	{
		std::cout << i << ".) " << tPrint->contents << ", " << tPrint->priority << std::endl;
		tPrint = tPrint->next;
	}
	std::cout << length << ".) " << tPrint->contents << ", " << tPrint->priority << std::endl;
}
