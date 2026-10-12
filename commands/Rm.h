#ifndef RM_H
#define RM_H

#include "Command.h"
#include <string>
#include <vector>

class Rm : public Command {
public:
    Rm() : Command("rm") {}
    static Command* commandCreate() { return new Rm(); }
    string execute() override;
    void addParameters(const vector<string>& tokens) override;
private:
    string filename_;
};

#endif // RM_H
