INCLUDE_DIRECTORIES(
	${WIKICLUST_SOURCE_DIR}
)

ADD_LIBRARY(tokenizer
	ITokenizer.cpp
	ITokenizer.h
	KeepLastTokens.cpp
	KeepLastTokens.h
	Token.cpp
	Token.h
	Tokenizer.cpp
	Tokenizer.h
)
