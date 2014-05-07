#include <iostream>
#include "tokenizer/Tokenizer.h"

int main(int argc, char **argv)
{
	// Buffer to analyze, if content is into a file it must be read and
	// allocated in memory before tokenization
	std::string buffer("an example to tokenize");

	// Allocate a tokenizer without wiki synthax support (second
	// argument) which generate single word (with a minimal size of 3)
	Tokenizer::Tokenizer tokenizer(1, 3);
	tokenizer.setBufferToAnalyze(buffer.c_str(), buffer.size());

	while (!tokenizer.eof()){
		Tokenizer::Token tok = tokenizer.getNextToken();
		std::cout << std::string(tok.getContent(), tok.getSize()) << std::endl;
	}
}
