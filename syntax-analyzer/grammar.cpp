#include "grammar.h"

Grammar::Grammar() {
  this->s.push("$");
  this->s.push("Start");
  this->table = {
    { "",     "$",      "(",      ")",      "i",    "+",      "-",      "*",    "/",    "="   },
    { "Start",    "",       "",       "",       "i=E",  "",       "",       "",     "",     "",   },
    { "E",    "",       "TE\'",   "",       "TE\'", "",       "",       "",     "",     ""    },
    { "E\'",  "epsilon","",       "epsilon","",     "+TE\'",  "-TE\'",  "",     "",     ""    },
    { "T",    "",       "FT\'",   "",       "FT\'", "",       "",       "",     "",     ""    },
    { "T\'",  "epsilon","",       "epsilon","",     "epsilon","epsilon","*FT\'","/FT'", ""    },
    { "F",    "",       "(E)",    "",       "i",    "",       "",       "",     "",     ""    }
  };
}

Grammar::~Grammar() {

}

bool Grammar::check(std::vector<Token>& tokens) {
  // Display the string we are validating
  for (unsigned i = 0; i< tokens.size(); i++) {
    std::cout << tokens[i].getLexeme();
  }
  std::cout << std::endl << std::endl;

  // This for-loop acts as the character pointer for our string
  for (unsigned i = 0; i < tokens.size();) {
    std::string nextPush;   // variable to store the rule we will push onto the stack
    int row = -1;

    // Get the top of the stack and pop
    std::string currentPop = this->s.top();
    this->s.pop();
    std::cout << "<" << currentPop << "> ---> ";

    // Get the current character
    std::string currLexeme = tokens[i].getLexeme();
    int currentType = tokens[i].getTokenTypeInt();

    // If both values from the stack and the token match '$', then it's valid 
    if (currentPop == "$" && currLexeme == "$") {
      std::cout << "CODE VALID" << std::endl;
      return true;
    }
    else if (currentPop == "$" && currLexeme != "$") {
      std::cout << "Stack just popped $ and lexeme is not $" << std::endl;
      return false;
    }

    // Check if the top of the stack matches a terminal character
    //// Check if terminal character is an identifier
    if (currentType == identifier) {
      if (currentPop == "i") {
        i++;  // move the pseudo-character pointer to the next character
        std::cout << "Matched identifier to stack" << std::endl << std::endl;
        continue;
      }
    }
    //// Check if terminal char besides identifier
    if (isTerminating(currentPop, currentType)) {
      i++;   // move the pseudo-character pointer to the next character
      std::cout << "Matched terminating char to stack" << std::endl << std::endl;
      continue;
    }
    
    // Get the corresponding row number based off the current symbol from stack
    if (currentPop == "Start") row = start;
    else if (currentPop == "E") row = e;
    else if (currentPop == "E\'") row = ePrime;
    else if (currentPop == "T") row = t;
    else if (currentPop == "T\'") row = tPrime;
    else if (currentPop == "F") row = f;
    else if (currentPop == "epsilon") {
      continue; // Will use the next value in the stack to reference the table without moving pseudo-character pointer
    }
    else {
      std::cout << "Element in rules stack not valid.Exiting." << std::endl;
      break;
    }

    // Retrieve appropriate rule to push onto stack from table
    if (currentType == identifier) { 
      nextPush = this->table[row][id];
    } // TODO: handle keywords
    else {
      if (currLexeme == "(") nextPush = this->table[row][openP];
      else if (currLexeme == ")") nextPush = this->table[row][closeP];
      else if (currLexeme == "+") nextPush = this->table[row][plus];
      else if (currLexeme == "-") nextPush = this->table[row][minus];
      else if (currLexeme == "*") nextPush = this->table[row][star];
      else if (currLexeme == "/") nextPush = this->table[row][slash];
      else if (currLexeme == "$") nextPush = this->table[row][dollarSign];
      else if (currLexeme == "=") nextPush = this->table[row][equal];
      else { std::cout << "Did not find appropriate char" << std::endl; break; }
    }
    std::cout << "<" << nextPush << ">" << std::endl;

    // If the value returned from the table is empty, then the line of code is invalid
    if (nextPush.empty()) {
      std::cout << "No appropriate rule to push onto stack." << std::endl;
      break;
    }

    // Push onto stack in reverse order of string
    if (nextPush == "epsilon") this->s.push(nextPush);
    else {
      for (int m = nextPush.length()-1; m >= 0; m--) {
        if (nextPush[m] == '\'') {
          std::string primeString = std::string(1,nextPush[m-1]) +  std::string(1,nextPush[m]);
          this->s.push(primeString);
          m--;
        }
        else { 
          this->s.push(std::string(1,nextPush[m]));
        }
      }
    }
  }
  return false; // dummy return
}

bool Grammar::isTerminating(std::string& currentPop, int currentType) {
  for (unsigned m = 1; m < this->table[0].size(); m++) {
      if (currentPop == this->table[0][m] && currentType != identifier) {
        return true;
      }
  }
  return false;
}