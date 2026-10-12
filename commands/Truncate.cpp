#include "Truncate.h"
#include "../utils/Exceptions.h"
#include <iostream>
#include <fstream>

using namespace std;

string Truncate::execute() {
    if (filename_.empty()) {
        throw TruncateMissingFilenameError();
    }
    ifstream in(filename_);
    if (!in.is_open()) {
        throw FileNotFoundError(filename_);
    }
    in.close();
    ofstream out(filename_, ios::trunc);
    if (!out) {
        throw FileTruncateError(filename_);
    }
    out.close();
    return "";
}

void Truncate::addParameters(const vector<string>& tokens) {
    filename_.clear();
    if (tokens.size() >= 2)
        filename_ = tokens[1];
}
