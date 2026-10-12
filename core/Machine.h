#ifndef MACHINE_H
#define MACHINE_H

#include <string>

using namespace std;

class Lexer;

class Machine {
public:
    static Machine& getInstance() {
        static Machine instance;
        return instance;
    }

    void set_prompt(const string& p) { prompt_ = p; }
    const string& get_prompt() const { return prompt_; }
    void start();

    Machine(const Machine&) = delete;
    Machine& operator=(const Machine&) = delete;

private:
    Machine() : prompt_("$ ") {}
    string prompt_;
};

#endif // MACHINE_H
