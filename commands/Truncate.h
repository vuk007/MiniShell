#ifndef TRUNCATE_H
#define TRUNCATE_H

#include "Command.h"
#include <string>
#include <vector>

class Truncate : public Command {
public:
    Truncate() : Command("truncate") {}
    static Command* commandCreate() { return new Truncate(); }

    string execute() override;
    void addParameters(const vector<string>& tokens) override;

private:
    string filename_;
};

#endif // TRUNCATE_H
