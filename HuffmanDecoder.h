#include <iostream>
#include <fstream>

class HuffmanDecoder{
    std::string		 **table;
    unsigned int tableLength;
public:
	std::string message;
	std::string filename;
	HuffmanDecoder();
	HuffmanDecoder(std::string);
  ~HuffmanDecoder();
	std::string decode();
	std::string decode(std::string);
private:
  char *contents;
	void decodeHeader(std::ifstream&);
	void decodeContent(std::ifstream&, int);
	unsigned int uBinToDec(char*, int);

//Info on how the message components are stored
    int fileLengthBitCount;
    int headerLengthBitCount;
    int valueBitCount;
    int charBitCount;
    int tableSizeBitCount;
};
