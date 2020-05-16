#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include "FSM.h"
#include "Token.h"


std::vector<Token> lexer(std::string);
std::string recognizeKeywordOrIdentifier(std::string&);
std::string recognizeRealNumber(std::string&);
int findDelimiterIndex(std::string);
bool checkKeywords(std::string&);

int main() {
	
	std::string filePath = "testSourceCode.txt";
	std::fstream sourceCode(filePath);
	std::string line;
	if (sourceCode.is_open()) {
		while (std::getline(sourceCode, line)) {
			std::vector<Token> newTokens = lexer(line);
			 for (auto token : newTokens) {
				 std::cout << "Token:" << token.getTokenType() << "\t\tLexeme: " << token.getLexeme() << std::endl;
			 }
		}
	}
	else { std::cout << "File not open" << std::endl; }

	system("PAUSE");
	return 0;
}

std::vector<Token> lexer(std::string s) {
	std::vector<Token> tokens;
	std::string currentChar, possibleKeywordOrIdentifier, possibleRealNumber, line = s;
	for (unsigned i = 0; i < line.length(); i++) {
		std::string currentChar = std::string(1, line[i]);	// Convert char to string to match regex_match parameter
		// Seperators
		if (std::regex_match(currentChar, separatorRegex)) { tokens.push_back(Token(separator, currentChar)); }
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
				line.erase(0, delimiterIndex);	// possibly might not need +1
				i = -1;	//reset for loop to account for erasing
			}
			else if (possibleKeywordOrIdentifier != "") {
				tokens.push_back(Token(identifier, possibleKeywordOrIdentifier));
				line.erase(0, delimiterIndex+1);
				i = -1;	//reset for loop to account for erasing
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
			/*i += substringFromCurrentIndex.length();*/
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
	const int keywordsArraySize = 4;
	for (int j = 0; j < keywordsArraySize; j++) {
		if (s == keywords[j]) {
			return true;
		}
	}
	return false;
}