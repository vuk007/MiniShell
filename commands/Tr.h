#ifndef TR_H
#define TR_H

#include "Command.h"
#include <string>
#include <vector>

class Tr : public Command {
public:
    Tr() : Command("tr") {}
    static Command* commandCreate() { return new Tr(); }

    string execute() override;
    void addParameters(const vector<string>& tokens) override;

private:
    string input_source_;
    string what_;
    string with_;
    string output_stream_;
    bool output_append_ = false;
};

#endif // TR_H
