#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

using namespace std;

class Command {
public:
    virtual ~Command() = default;
    virtual string execute() = 0;
    virtual void addParameters(const vector<string>& tokens) = 0;

    string getCommandName() const { return commandName_; }
    bool getInputPipeline() const { return inputPipeline_; }
    bool getOutputPipeline() const { return outputPipeline_; }
    void setPipelineFlags(bool inputPipeline, bool outputPipeline) {
        inputPipeline_ = inputPipeline;
        outputPipeline_ = outputPipeline;
    }

protected:
    explicit Command(const string& commandName) : commandName_(commandName) {}

    const string commandName_;
    bool inputPipeline_ = false;
    bool outputPipeline_ = false;
};
#endif // COMMAND_H
