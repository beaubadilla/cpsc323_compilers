#pragma once
#include <string>
#include <iostream>

enum TokenType {
	keyword, identifier, real, relop, arithop, separator, error, dollSign
};

class Token {
private:
	TokenType type;
	std::string lexeme;
public:
	Token();
	Token(TokenType tT, std::string s) { this->type = tT; this->lexeme = s; }
	~Token();
	void print() { std::cout << this->type << "\t" << this->lexeme << std::endl; }
	std::string getTokenType();
	int getTokenTypeInt();
	std::string getLexeme() { return this->lexeme; }
};