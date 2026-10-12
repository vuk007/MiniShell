#ifndef TIME_H
#define TIME_H

#include "Command.h"
#include <string>
#include <vector>

class Time : public Command {
public:
    Time() : Command("time") {}
    static Command* commandCreate() { return new Time(); }
    string execute() override;
    void addParameters(const vector<string>& tokens) override;
private:
    string output_stream_;
    bool output_append_ = false;
};

#endif // TIME_H
