#include "FSM.h"

const std::string keywords[4] = {
	"if", "while", "whileend", "else"
};
const std::regex separatorRegex("[(\(|\)|{|}]");
const std::regex arithopRegex("[(\+|\-|\/|\*)]");
const std::regex relopRegex("[(<|>|=)]");
const std::regex letterRegex("[a-zA-Z]");
const std::regex digitRegex("[0-9]");

enum firstRowTransitionTable {
	digit, letter, period, dollarSign
};

FSM::FSM() {
	std::set<int> allStates = { 1, 2, 3, 4 };
	std::vector<int> acceptStates = { 2, 4 };
	this->states = allStates;
	this->acceptingStates = acceptStates;
	this->stateTransitionTable = {
		{NULL,	NULL,	NULL,	NULL},
		{3,		2,		NULL,	NULL},
		{2,		2,		NULL,	2},
		{3,		NULL,	4,		NULL},
		{4,		NULL,	NULL,	NULL}
	};
	this->initialState = 1;
	this->currentState = initialState;
}


FSM::~FSM()
{
}

void FSM::nextState(char input) {
	std::string inputAsString = std::string(1, input);
	if (std::regex_match(inputAsString, digitRegex)) {
		this->currentState = this->stateTransitionTable[this->currentState][digit];
	}
	else if (std::regex_match(inputAsString, letterRegex)) {
		this->currentState = this->stateTransitionTable[this->currentState][letter];
	}
	else if (inputAsString == ".") {
		this->currentState = this->stateTransitionTable[this->currentState][period];
	}
	else if (inputAsString == "$") {
		this->currentState = this->stateTransitionTable[this->currentState][dollarSign];
	}
}

bool FSM::validState() {
	if (this->currentState == NULL) { return false; }
	return true;
}

bool FSM::isAcceptingState() {
	for (int state : this->acceptingStates) {
		if (this->currentState == state) { return true; }
	}

	//if (this->acceptingStates.find(this->currentState)) { return true; }
	return false;
}
