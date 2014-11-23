#include <iostream>
#include <string>

class pQueue{
protected:
	struct pQueueMember{
		pQueueMember *next;
		std::string contents;
		int priority;
	};
	pQueueMember *front, *back;
	void insert(pQueueMember*);
public:
	int length;
	pQueue();
    virtual ~pQueue();
	virtual void enqueue(std::string, int);
	void dequeue(std::string&, int&);
	void print();
};
