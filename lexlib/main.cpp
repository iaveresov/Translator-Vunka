#include "lexer.h"
#include<gtest/gtest.h>
#include<iostream>

using namespace std;	

TEST(lex_test, id){
	Lexer l("test.txt");
	pair<string, string> lexem;
	
	lexem = l.get_next_lexem();
	EXPECT_EQ("id", lexem.first);
	EXPECT_EQ("kwint", lexem.second);
	
	lexem = l.get_next_lexem();
	EXPECT_EQ("id", lexem.first);
	EXPECT_EQ("main", lexem.second);

}

TEST(lex_test, kwwhile){
	pair<string, string>lexem;
	Lexer l("test2.txt");
	
	lexem = l.get_next_lexem();
	EXPECT_EQ("id", lexem.first);
}

int main(){
	::testing::InitGoogleTest();
	return RUN_ALL_TESTS();

}
