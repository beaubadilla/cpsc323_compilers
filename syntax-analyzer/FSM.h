#pragma once
#include <set>
#include <vector>
#include <regex>

extern const std::string keywords[14];
extern const std::regex separatorRegex;
extern const std::regex arithopRegex;
extern const std::regex relopRegex;
extern const std::regex letterRegex;
extern const std::regex digitRegex;

class FSM {
private:
	std::set<int> states;
	std::vector<int> acceptingStates;
	std::vector<std::vector<int>> stateTransitionTable;
	int initialState;
	int currentState;
public:
	FSM();
	//FSM(std::set<int>, int, std::set<int>);
	~FSM();
	void nextState(char);
	bool validState();
	bool isAcceptingState();
	int getCurrentState() { return currentState; }
};

