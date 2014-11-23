#include "HuffmanTree.h"
#include <string>

//---------------------------------------------------------------------------
//Public functions
HuffmanTree::HuffmanTree() //Constructor
{//Constructs a priority queue
    pQueue();
}

HuffmanTree::HuffmanTree(std::string input, std::string fileName) //Constructor
{//Compounds constructor and compress into one statement
	HuffmanTree();
	compress(input, fileName);
}

HuffmanTree::~HuffmanTree() //Deconstructor
{//Deallocates all nodes in the tree and assigns NULL to the front pointer so the
	//parent class (pQueue) deconstructor knows that the queue has already been deallocated
	HuffmanTreeNode *tDelete = static_cast<HuffmanTreeNode*>(front);
	if (front != NULL)
	{//If front is NULL there is nothing to deallocate
		deleteTree(tDelete);
		front = NULL;
	}
}

void HuffmanTree::compress(std::string input, std::string fileName)
{//Compresses the string and stores the result into the file fileName by
	//making a tree from the string then using the tree to create a table
	//of characters and their compressed binary values. This table is then
	//used translate the string and store both the table and the compressed
	//message into the file.
	makeTree(input);
	fillFile(input, fileName);
}

//---------------------------------------------------------------------------

void HuffmanTree::makeTree(std::string message)
{//
	parse(message);

	HuffmanTreeNode *left, *right;
	while (length > 1)
	{
		left = dequeue();
		right = dequeue();
		enqueue(left, right);
	}
	HuffmanTreeNode *me = static_cast<HuffmanTreeNode*>(front);
    pos = 0;
    if (me->left == NULL && me->right == NULL)
    {
        table[0][0] = me->contents;
        table[1][0] = "0";
        tableBitLength[0] = 1;
    }
	if (me->left != NULL)
	{
		fillValues(me->left, 1, '0', NULL);
	}
	if (me->right != NULL)
	{
		fillValues(me->right, 1, '1', NULL);
	}
}

void HuffmanTree::parse(std::string input)
{
	if (!input.empty())
	{
		bool found;
		int enqueueSize = 1;
        const char *arInput = input.c_str();
		enqueueList *enqueueFront = new enqueueList;
		enqueueFront->next = NULL;
		enqueueFront->priority = 1;
		enqueueList *enqueueTemp;
		enqueueFront->contents = arInput[0];
		for (int i = 1; i < (int)input.size(); i = i + 1)
		{
			enqueueTemp = enqueueFront;
			found = false;
			for (int n = 1; n < enqueueSize && !found; n = n + 1)
			{
				if (arInput[i] == enqueueTemp->contents)
				{
					enqueueTemp->priority = enqueueTemp->priority + 1;
					found = true;
				}
				enqueueTemp = enqueueTemp->next;
			}
			if (arInput[i] == enqueueTemp->contents)
			{
				enqueueTemp->priority = enqueueTemp->priority + 1;
				found = true;
			}

			if (!found)
			{
				enqueueTemp = enqueueFront;
				for (int n = 1; n < enqueueSize; n = n + 1)
				{
					enqueueTemp = enqueueTemp->next;
				}
				enqueueTemp->next = new enqueueList;
				enqueueTemp->next->contents = arInput[i];
				enqueueTemp->next->priority = 1;
				enqueueSize = enqueueSize + 1;

			}
		}
		std::string contents;
		tableSize = enqueueSize;
		table[0] = new std::string[tableSize];
		table[1] = new std::string[tableSize];
		tableBitLength = new int[tableSize];
		for (int i = 1; i < enqueueSize; i = i + 1)
		{
			enqueue(std::string(1, enqueueFront->contents), enqueueFront->priority);
			enqueueTemp = enqueueFront;
			enqueueFront = enqueueFront->next;
			delete enqueueTemp;
		}
		enqueue(std::string(1, enqueueFront->contents), enqueueFront->priority);
		delete enqueueFront;
	}

}

void HuffmanTree::enqueue(std::string contents, int priority)
{
	HuffmanTreeNode *tNew;
	tNew = new HuffmanTreeNode;
	tNew->contents = contents;
	tNew->priority = priority;
	tNew->left = NULL;
	tNew->right = NULL;
	pQueue::insert(tNew);
	length = length + 1;
}

HuffmanTree::HuffmanTreeNode* HuffmanTree::dequeue()
{
	if (length > 0)
	{
		HuffmanTreeNode* tReturn = static_cast<HuffmanTreeNode*>(front);
		if (length > 1)
			front = front->next;
		length = length - 1;
		return tReturn;
	}
	else
		return NULL;
}

void HuffmanTree::enqueue(HuffmanTreeNode* memA, HuffmanTreeNode* memB)
{
	HuffmanTreeNode* tNew = new HuffmanTreeNode;
	tNew->left = memA;
	tNew->right = memB;
	tNew->contents = memA->contents + memB->contents;
	tNew->priority = memA->priority + memB->priority;
	pQueue::insert(tNew);
	length = length + 1;
}

void HuffmanTree::fillValues(HuffmanTreeNode *me, int depth, char nextVal, char* parentVal)
{
	char* currentVal = new char[depth];
	for (int i = 0; i < depth - 1; i = i + 1)
	{
		currentVal[i] = parentVal[i];
	}
	currentVal[depth - 1] = nextVal;
    me->value = currentVal;

	if (me->left != NULL)
	{
		fillValues(me->left, depth + 1, '0', currentVal);
	}
	if (me->right != NULL)
	{
		fillValues(me->right, depth + 1, '1', currentVal);
    }
	if (me->left == NULL && me->right == NULL)
	{
		table[0][pos] = me->contents;
		table[1][pos] = std::string(me->value, depth);
		tableBitLength[pos] = depth;
		pos = pos + 1;
        /*
        std::cout << "'" << me->contents << "' is used " << me->priority << " time(s) and has the value " << std::string(me->value, depth) << std::endl
                  << "As opposed to (" << (int)me->contents.c_str()[0] << ")" << decTouBin((int)me->contents.c_str()[0], 8) << std::endl;
        */
	}
}

std::string HuffmanTree::GetTable()
{
    std::string myTable = "";

    for (int i = 0; i < tableSize; i = i + 1)
    {
        myTable.append("'");
        myTable.append(table[0][i]);
        myTable.append("' = (");
        myTable.append(decTouBin((int)table[0][i].c_str()[0], 8));
        myTable.append(")-> ");
        myTable.append(table[1][i]);
        myTable.append("\n");
    }

    return myTable;
}

std::string HuffmanTree::GetMessage()
{
    return file;
}

//---------------------------------------------------------------------------

void HuffmanTree::fillFile(std::string message, std::string fileName)
{
	std::string  content= makeContent(message);
    std::string header = makeHeader((int)content.length());
    file = header + content;

    unsigned int bitCount = 0;
    while (bitCount < file.length())
        bitCount = bitCount + 8;
    while (file.length() < bitCount)
        file.append("0");

	if (!(fileName == "nofile"))
	{
		std::ofstream fileOut;
		fileOut.open(fileName.c_str());
        fileOut << file;
		fileOut.close();
	}

    /*
	std::cout << std::endl << file << std::endl << std::endl
			  <<"Compression ratio:" << std::endl 
			  << file.length() << "b/" << message.length() * 8 << "b = " << (float)file.length() / ((float)message.length() * 8) << std::endl;
    */
	
}

std::string HuffmanTree::makeContent(std::string message)
{
	std::string content = "";
	std::string currentChar = "";
	bool found;
	const char *archar = message.c_str();
	for (unsigned int i = 0; i < message.length(); i = i + 1)
	{
		currentChar = std::string(1, archar[i]);
		found = false;
		for (int n = 0; n < tableSize && !found; n = n + 1)
		{
			if (currentChar == table[0][n])
			{
				found = true;
				content.append(table[1][n]);
			}
		}
	}
	return content;
}

std::string HuffmanTree::makeHeader(int contentLength)
{
	int charBitCount			= 8;
	int tableSizeBitCount		= 8;
	int fileLengthBitCount		= 16;
	int headerLengthBitCount	= 16;

	std::string headerTable = "";
	for (int i = 0; i < tableSize; i = i + 1)
	{
		headerTable.append(decTouBin(tableBitLength[i] - 1, 4));
		headerTable.append(table[1][i] + decTouBin((int)table[0][i].c_str()[0], charBitCount));
	}
    std::string header = decTouBin(contentLength + (int)headerTable.length()  + tableSizeBitCount, fileLengthBitCount)
                       + decTouBin((int)headerTable.length() + tableSizeBitCount, headerLengthBitCount)
					   + decTouBin(tableSize, tableSizeBitCount)
					   + headerTable;
	return header;
}

std::string HuffmanTree::decTouBin(unsigned int value, int bitLength)
{
	unsigned int exp = 1;
	for (int i = 1; i < bitLength; i = i + 1)
	{
		exp = exp * 2;
	}
	std::string bin = "";
	for (int i = 0; i < bitLength; i = i + 1)
	{
		if (value >= exp)
		{
			bin.append("1");
			value = value - exp;
		}
		else
			bin.append("0");
		exp = exp / 2;
	}
	return bin;
}

//---------------------------------------------------------------------------

void HuffmanTree::deleteTree(HuffmanTreeNode* me)
{
	if (me->left != NULL)
		deleteTree(me->left);
	if (me->right != NULL)
        deleteTree(me->right);
	delete me;
}
