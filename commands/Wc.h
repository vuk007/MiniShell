#ifndef WC_H
#define WC_H

#include "Command.h"
#include <string>
#include <vector>

class Wc : public Command {
public:
    Wc() : Command("wc") {}
    static Command* commandCreate() { return new Wc(); }

    string execute() override;
    void addParameters(const vector<string>& tokens) override;

private:
    enum class Mode { Words, Chars } mode_;
    string input_arg_;
    bool option_given_ = false;
    bool input_from_quoted_ = false;
    bool input_from_stdin_ = false;
};

#endif // WC_H
