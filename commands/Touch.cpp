#include "Touch.h"
#include "../utils/Exceptions.h"
#include <iostream>
#include <fstream>

using namespace std;

string Touch::execute() {
    if (filename_.empty()) {
        throw TouchMissingFilenameError();
    }
    ifstream in(filename_);
    if (in.is_open()) {
        in.close();
        throw FileExistsError(filename_);
    }
    in.close();
    ofstream out(filename_);
    if (!out) {
        throw FileCreateError(filename_);
    }
    out.close();
    return "";
}

void Touch::addParameters(const vector<string>& tokens) {
    filename_.clear();
    if (tokens.size() >= 2)
        filename_ = tokens[1];
}
