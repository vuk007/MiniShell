#ifndef DATE_H
#define DATE_H

#include "Command.h"
#include <string>
#include <vector>

class Date : public Command {
public:
    Date() : Command("date") {}
    static Command* commandCreate() { return new Date(); }

    string execute() override;
    void addParameters(const vector<string>& tokens) override;
};

#endif // DATE_H
