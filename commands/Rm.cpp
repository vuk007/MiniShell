#include "Rm.h"
#include "../utils/Exceptions.h"
#include <iostream>
#include <cstdio>

using namespace std;

string Rm::execute() {
    if (filename_.empty()) {
        throw RmMissingFilenameError();
    }
    if (remove(filename_.c_str()) != 0) {
        throw FileRemoveError(filename_);
    }
    return "";
}

void Rm::addParameters(const vector<string>& tokens) {
    filename_.clear();
    if (tokens.size() >= 2)
        filename_ = tokens[1];
}
