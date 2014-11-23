#include "HuffmanDecoder.h"

HuffmanDecoder::HuffmanDecoder()
{
	message = "";
	filename = "";
    fileLengthBitCount		= 16;
    headerLengthBitCount	= 16;
    valueBitCount			= 4;
    charBitCount			= 8;
    tableSizeBitCount		= 8;

    table = new std::string*[2];
    table[0] = NULL;
    table[1] = NULL;

    contents = new char;
}

HuffmanDecoder::HuffmanDecoder(std::string fileName)
{
	HuffmanDecoder();
	decode(fileName);
}

HuffmanDecoder::~HuffmanDecoder()
{
    if (table[0] != NULL)
    {
        delete [] table[0];
        delete [] table[1];
    }
    delete [] table;
}

std::string HuffmanDecoder::decode()
{
	if (filename != "")
		decode(filename);
	else
		message = "No filename was given!";

	return message;
}
std::string HuffmanDecoder::decode(std::string fileName)
{
	std::ifstream fileIn(fileName.c_str());

    fileIn.read(contents, fileLengthBitCount);
    unsigned int fileSize = uBinToDec(contents, fileLengthBitCount);

    fileIn.get(contents, headerLengthBitCount + 1);
    unsigned int headerSize = uBinToDec(contents, headerLengthBitCount);
    unsigned int messageBitLength = fileSize - headerSize;

	decodeHeader(fileIn);
    decodeContent(fileIn, messageBitLength);
    fileIn.close();

	return message;
}

void HuffmanDecoder::decodeHeader(std::ifstream& fileIn)
{//Decodes the header segment of the file.
	unsigned int maxBitLength = 0;
	unsigned int currentBitLength;

    //Reads the tableSize from the file and creates a 2d array
    //of size 2xtableLength.
	fileIn.get(contents, tableSizeBitCount + 1);
	tableLength = uBinToDec(contents, tableSizeBitCount);
	table[0] = new std::string[tableLength];
	table[1] = new std::string[tableLength];

	for (unsigned int i = 0; i < tableLength; i = i + 1)
	{
		fileIn.get(contents, valueBitCount + 1);
        currentBitLength = uBinToDec(contents, valueBitCount) + 1;

		if (currentBitLength > maxBitLength)
            maxBitLength = currentBitLength;
		fileIn.get(contents, currentBitLength + 1);
		table[1][i] = contents;

		fileIn.get(contents, charBitCount + 1);
        table[0][i] = (char)uBinToDec(contents, charBitCount);
    }
}

void HuffmanDecoder::decodeContent(std::ifstream& fileIn, int length)
{//Decodes the content portion of the file
	std::string value = "";
    bool found;
    for (int i = 0; i < length; i = i + 1)
    {//Reads one value from the file into value until value matches a value
        //from the table read from the header.
		found = false;
		fileIn.get(contents, 2);
		value.append(contents);
        for (unsigned int n = 0; n < tableLength; n = n + 1)
		{
			if (value == table[1][n])
            {//When the program finds a match for value, the contents of value
                //are reset and the matching character is added to the resultant string.
				value = "";
				message.append(table[0][n]);
			}
		}
    }
}

unsigned int HuffmanDecoder::uBinToDec(char* bin, int bitLength)
{//Translates binary bytestring into an unsigned int value.
	unsigned int dec = 0;
	unsigned int exp = 1;
	for (int i = 1; i < bitLength; i = i + 1)
    {//Finds value of MSB for given bitLength
		exp = exp * 2;
	}
	for (int i = 0; i < bitLength; i = i + 1)
	{
		if (bin[i] == '1')
			dec = dec + exp;
		exp = exp / 2;
	}
	return dec;
}
