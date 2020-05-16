#pragma once
#include <stack>
#include <string>
#include <vector>
#include "Token.h"
#include <iostream>

const enum tableRowFields {
  dollarSign=1, openP, closeP, id, plus, minus, star, slash, equal
};

const enum tableColFields {
  start=1, e, ePrime, t, tPrime, f
};

class Grammar {
private:
  std::stack<std::string> s;
  std::string currentLexeme;
  std::vector<std::vector<std::string>> table;
  // std::vector<std::string> terminatingChars;
public:
  Grammar();
  ~Grammar();
  bool check(std::vector<Token>&);
  bool isTerminating(std::string&, int);
  // bool e();
  // bool ePrime();
  // bool t();
  // bool tPrime();
  // bool f();
  void reportError();
};