#include <fstream>
#include "pQueue.h"

class HuffmanTree : pQueue{
	struct HuffmanTreeNode : pQueueMember{
		HuffmanTreeNode *left, *right;
		char* value;
	};
	struct enqueueList{
		char contents;
		int priority;
		enqueueList *next;
	};
public:
	HuffmanTree();
	HuffmanTree(std::string, std::string);
    ~HuffmanTree();
	void compress(std::string, std::string);

    //Returns the lookup table as a string.
    std::string GetTable();
    //Returns the compressed message where each bit is
    //represented as '1' or '0' as a string.
    std::string GetMessage();
private:
	std::string *table[2];	//Table storage
    int *tableBitLength;    //# of bits to represent a char in table(used in an array)
    int tableSize;          //# of members in the table
    int pos;                //Current position in the table

    std::string file;       //compressed message where each bit is
                            //represented as '1' or '0' as a string.

	void makeTree(std::string);	//Tree creation
	void parse(std::string);
	void enqueue(std::string, int);
	HuffmanTreeNode* dequeue();
	void enqueue(HuffmanTreeNode*, HuffmanTreeNode*);
	void fillValues(HuffmanTreeNode*, int, char, char*);

	void fillFile(std::string, std::string);	//File creation
	std::string makeContent(std::string);
	std::string makeHeader(int);
	std::string decTouBin(unsigned int, int);

	void deleteTree(HuffmanTreeNode*);
};
