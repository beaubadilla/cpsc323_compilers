#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "FSM.h"
#include "Token.h"
#include "Grammar.h"

std::string promptUser();
std::vector<Token> lexer(std::string, bool&);
std::string recognizeKeywordOrIdentifier(std::string&);
std::string recognizeRealNumber(std::string&);
int findDelimiterIndex(std::string);
bool checkKeywords(std::string&);

int main() {
	std::string filePath = promptUser();
	std::fstream sourceCode(filePath);
	std::string line;
	std::vector<Token> tokens;
	// Grammar g;
	bool isMultiLineComment = false;
	if (sourceCode.is_open()) {
		while (std::getline(sourceCode, line)) {
			std::vector<Token> newTokens = lexer(line, isMultiLineComment);
			std::cout << std::endl;
			for (auto token : newTokens) {
				std::cout << "Token:" << token.getTokenType() << "\t\tLexeme: " << token.getLexeme() << std::endl;
				tokens.push_back(token);
			}
			// Syntax Analyzer
			newTokens.push_back(Token(dollSign, "$"));
			Grammar g;
			if(!g.check(newTokens)) {
				std::cout << "Source code invalid. Exiting" << std::endl;
				break;
			}
		}
	}
	else { std::cout << "File not open" << std::endl; }

	system("PAUSE");
	return 0;
}

std::string promptUser() {
	std::string filePath;
	std::cout << "Hi Professor Le!" << std::endl;
	std::cout << "Welcome to Beau's Lexer" << std::endl;
	std::cout << "Please enter file name: ";
	std::getline(std::cin, filePath);
	return filePath;
}

std::vector<Token> lexer(std::string s, bool& isMultiLineComment) {
	std::vector<Token> tokens;
	std::string possibleKeywordOrIdentifier, possibleRealNumber, line = s;
	for (int i = 0; i < int(line.length()); i++) {
		std::string currentChar = std::string(1, line[i]);	// Convert char to string to match regex_match parameter
		// Seperators
		if (line.find('!') < line.length() && isMultiLineComment == true) {	// closing comment seperator found on a different line than opening
			isMultiLineComment = false;
			line.erase(0, line.find('!')+1);
			i = -1;
		}
		else if (line.find('!') >= line.length() && isMultiLineComment == true) { // still looking for closing comment seperator; for more than 2 lines
			line.erase(0, line.length());
			i = -1;
		}
		else if (currentChar == "!") {	// opening comment seperator
			line.erase(0, 1);	// erase the currently found '!'
			if (line.find('!') < line.length()) {	// opening and closing comment seperator found on same line
				line.erase(0, line.find('!'));
				isMultiLineComment = false;
			}
			else {									// opening comment seperator JUST found, but not closing
				line.erase(0, line.length());
				i = -1;
				isMultiLineComment = true;
			}
		}
		else if (std::regex_match(currentChar, separatorRegex)) { tokens.push_back(Token(separator, currentChar)); }
		// Arithmetic Operators
		else if (std::regex_match(currentChar, arithopRegex)) { tokens.push_back(Token(arithop, currentChar)); }
		// Relational Operators
		else if (std::regex_match(currentChar, relopRegex)) { tokens.push_back(Token(relop, currentChar)); }
		// Letters
		else if (std::regex_match(currentChar, letterRegex)) {
			bool isKeyword = false;
			int delimiterIndex = findDelimiterIndex(line);
			std::string substringFromCurrentIndex = line.substr(i, delimiterIndex);
			possibleKeywordOrIdentifier = recognizeKeywordOrIdentifier(substringFromCurrentIndex); // rKoI() returns "" if not accepted
			isKeyword = checkKeywords(possibleKeywordOrIdentifier);
			if (isKeyword) {
				tokens.push_back(Token(keyword, possibleKeywordOrIdentifier));
				line.erase(0, delimiterIndex);
				i = -1;	//reset 'for loop' to account for erasing
			}
			else if (possibleKeywordOrIdentifier != "") {
				tokens.push_back(Token(identifier, possibleKeywordOrIdentifier));
				line.erase(0, delimiterIndex);
				i = -1;	//reset 'for loop' to account for erasing
			}
		}
		// Digits
		else if (std::regex_match(currentChar, digitRegex)) {
			int delimiterIndex = findDelimiterIndex(line);
			std::string substringFromCurrentIndex = line.substr(i, delimiterIndex);
			possibleRealNumber = recognizeRealNumber(substringFromCurrentIndex);
			if (possibleRealNumber != "") { tokens.push_back(Token(real, possibleRealNumber)); }
			line.erase(0, delimiterIndex+1);
			i = -1;
		}
		else { 
			line.erase(0, 1);
			i = -1;
		}
		
	}
	return tokens;
}

std::string recognizeKeywordOrIdentifier(std::string& s) {
	FSM fsm;
	for (unsigned i = 0; i < s.length(); i++) {
		if (fsm.getCurrentState() == NULL) { break; }
		fsm.nextState(s[i]);
	}
	if (fsm.isAcceptingState()) {
		return s;
	}
	return "";
}

std::string recognizeRealNumber(std::string& s) {
	FSM fsm;
	for (unsigned i = 0; i < s.length(); i++) {
		if (fsm.getCurrentState() == NULL) { break; }
		fsm.nextState(s[i]);
	}
	if (fsm.isAcceptingState()) {
		return s;
	}
	return "";
}

int findDelimiterIndex(std::string s) {
	for (unsigned i = 0; i < s.length(); i++) {
		if (s[i] == ' ' || s[i] == '(' || s[i] == ')' || s[i] == '{' || s[i] == '}' || s[i] == ',')
			return i;
	}
	return -2;
}

bool checkKeywords(std::string& s) {
	const int keywordsArraySize = 14;
	for (int j = 0; j < keywordsArraySize; j++) {
		if (s == keywords[j]) {
			return true;
		}
	}
	return false;
}