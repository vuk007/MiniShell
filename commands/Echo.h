#ifndef ECHO_H
#define ECHO_H
#include "Command.h"
#include <string>
#include <vector>

class Echo : public Command {
public:
    Echo() : Command("echo") {}
    static Command* commandCreate() { return new Echo(); }

    void addParameters(const vector<string>& tokens) override;
    string execute() override;

private:
    string input_stream_;
    string output_stream_;
    bool output_append_ = false;
};
#endif // ECHO_H
