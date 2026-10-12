#include "Lexer.h"
#include "../commands/Echo.h"
#include "../commands/Prompt.h"
#include "../commands/Time.h"
#include "../commands/Date.h"
#include "../commands/Touch.h"
#include "../commands/Truncate.h"
#include "../commands/Rm.h"
#include "../commands/Wc.h"
#include "../commands/Tr.h"
#include "../commands/Head.h"
#include "../commands/Batch.h"

#define REGISTER_CMD(name, Cmd)  commandCreateMap[name] = &Cmd::commandCreate

//konstruktor
Lexer::Lexer() {
    REGISTER_CMD("echo", Echo);
    REGISTER_CMD("prompt", Prompt);
    REGISTER_CMD("time", Time);
    REGISTER_CMD("date", Date);
    REGISTER_CMD("touch", Touch);
    REGISTER_CMD("truncate", Truncate);
    REGISTER_CMD("rm", Rm);
    REGISTER_CMD("wc", Wc);
    REGISTER_CMD("tr", Tr);
    REGISTER_CMD("head", Head);
    REGISTER_CMD("batch", Batch);
}

pair<vector<string>, vector<int> > Lexer::lex(const string &commandString) {
    vector<string> Token_List;
    vector<int> TokenPos;
    int i = 0;
    bool QuoteFlag = false, SpaceFlag = true;

    for (int j = 0; j < commandString.size(); j++) {
        char curr_char = commandString[j];
        if (curr_char == '\"') {
            QuoteFlag = !QuoteFlag;
            if (SpaceFlag) {
                SpaceFlag = false;
                Token_List.push_back("");
                TokenPos.push_back(j);
                i = (int)Token_List.size() - 1;
                Token_List[i] += curr_char;
            }
            else {
                Token_List[i] += curr_char;
            }
        }
        else if (QuoteFlag) {
            Token_List[i] += curr_char;
        }
        else if (isspace(curr_char)) {
            SpaceFlag = true;
        }
        else if (SpaceFlag) {
            SpaceFlag = false;
            Token_List.push_back("");
            TokenPos.push_back(j);
            i = (int)Token_List.size() - 1;
            Token_List[i] += curr_char;
        }
        else {
            if (Token_List[i] == "<" || (Token_List[i] == ">" && curr_char != '>') || Token_List[i] == ">>") {
                Token_List.push_back("");
                TokenPos.push_back(j);
                i = (int)Token_List.size() - 1;
                Token_List[i] += curr_char;
            }
            else {
                Token_List[i] += curr_char;
            }
        }
    }

    return make_pair(Token_List, TokenPos);
}

Command* Lexer::createCommand(const string& commandString, bool inputPipeline, bool outputPipeline) {
    auto [Token_List, TokenPos] = lex(commandString);
    (void)TokenPos;
    if (Token_List.empty())
        return nullptr;
    auto it = commandCreateMap.find(Token_List[0]);
    if (it == commandCreateMap.end())
        return nullptr;
    Command* cmd = it->second();
    cmd->setPipelineFlags(inputPipeline, outputPipeline);
    cmd->addParameters(Token_List);
    return cmd;
}

bool Lexer::runCommand(const string& commandString, bool inputPipeline, bool outputPipeline) {
    Command* cmd = createCommand(commandString, inputPipeline, outputPipeline);
    if (!cmd)
        return false;
    cmd->execute();
    delete cmd;
    return true;
}
