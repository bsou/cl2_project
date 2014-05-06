INCLUDE_DIRECTORIES(
	${WIKICLUST_SOURCE_DIR}
	${CPPUNIT_INCLUDE}
)

ADD_EXECUTABLE(TestSuite
	TestKeepLastTokens.cpp
	TestKeepLastTokens.h
	TestStringFactory.cpp
	TestStringFactory.h
	TestSuite.cpp
	TestTokenizer.cpp
	TestTokenizer.h
	TestToolBoxHash.cpp
	TestToolBoxHash.h
	TestToolBoxTrie.cpp
	TestToolBoxTrie.h
)

TARGET_LINK_LIBRARIES(TestSuite
	tokenizer
	tools
	cppunit
)

ADD_CUSTOM_COMMAND(
                TARGET TestSuite
                POST_BUILD
                COMMAND ${WIKICLUST_SOURCE_DIR}/tests/TestSuite
)
