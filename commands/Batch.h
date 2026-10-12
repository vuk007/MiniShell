#ifndef BATCH_H
#define BATCH_H

#include "Command.h"
#include <string>
#include <vector>

class Batch : public Command {
public:
    Batch() : Command("batch") {}
    static Command* commandCreate() { return new Batch(); }

    string execute() override;
    void addParameters(const vector<string>& tokens) override;

private:
    string filename_;
    string output_stream_;
    bool output_append_ = false;
};

#endif // BATCH_H
