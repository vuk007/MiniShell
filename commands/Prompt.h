#ifndef PROMPT_H
#define PROMPT_H

#include "Command.h"
#include <string>
#include <vector>

class Prompt : public Command {
public:
    Prompt() : Command("prompt") {}
    static Command* commandCreate() { return new Prompt(); }

    string execute() override;
    void addParameters(const vector<string>& tokens) override;

private:
    string new_prompt_;
    bool argument_was_quoted_ = false;
};
#endif // PROMPT_H
