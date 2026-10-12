#ifndef TOUCH_H
#define TOUCH_H

#include "Command.h"
#include <string>
#include <vector>

class Touch : public Command {
public:
    Touch() : Command("touch") {}
    static Command* commandCreate() { return new Touch(); }
    string execute() override;
    void addParameters(const vector<string>& tokens) override;

private:
    string filename_;
};

#endif // TOUCH_H
