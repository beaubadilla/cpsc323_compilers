#include "Token.h"



Token::Token()
{
}


Token::~Token()
{
}

std::string Token::getTokenType() {
	switch (this->type) {
	case 0:
		return "keyword\t";
	case 1:
		return "identifier";
	case 2:
		return "real\t";
	case 3:
		return "relop\t";
	case 4:
		return "arithop\t";
	case 5:
		return "separator\t";
	case 6:
		return "error\t";
	// case 7:
		// return "dollarSign\t";
	}
	return "Invalid Token Type";
}

int Token::getTokenTypeInt() {
	return this->type;
}