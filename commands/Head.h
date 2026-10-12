#ifndef HEAD_H
#define HEAD_H

#include "Command.h"
#include <string>
#include <vector>

class Head : public Command {
public:
    Head() : Command("head") {}
    static Command* commandCreate() { return new Head(); }

    string execute() override;
    void addParameters(const vector<string>& tokens) override;

private:
    size_t line_count_ = 0;
    string input_source_;
    bool input_from_quoted_ = false;
    bool input_from_stdin_ = false;
    string output_stream_;
    bool output_append_ = false;
};

#endif // HEAD_H
