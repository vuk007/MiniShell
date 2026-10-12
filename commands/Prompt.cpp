#include "Prompt.h"
#include "../core/Machine.h"

string Prompt::execute() {
    if (argument_was_quoted_)
        Machine::getInstance().set_prompt(new_prompt_);
    return "";
}

void Prompt::addParameters(const vector<string>& tokens) {
    new_prompt_.clear();
    argument_was_quoted_ = false;
    if (tokens.size() >= 2) {
        const string& arg = tokens[1];
        if (arg.size() >= 2 && arg[0] == '\"' && arg.back() == '\"') {
            new_prompt_ = arg.substr(1, arg.size() - 2);
            argument_was_quoted_ = true;
        }
    }
}
