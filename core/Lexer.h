#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include "../commands/Command.h"
using namespace std;
class Lexer {

private:
    unordered_map<string, Command* (*)()> commandCreateMap;
public:
    Lexer();
    pair<vector<string>, vector<int>> lex(const string& commandString);
    Command* createCommand(const string& commandString, bool inputPipeline = false, bool outputPipeline = false);
    bool runCommand(const string& commandString, bool inputPipeline = false, bool outputPipeline = false);
};
#endif //LEXER_H
