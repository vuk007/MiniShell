//
// Created by vuk00 on 2/28/2026.
//

#include "File_Handler.h"
#include <fstream>

const vector<string> File_Handler::ReadFile(const string& FileName) {
    vector<string> result;
    ifstream f(FileName);
    if (!f.is_open())
        return result;
    string line;
    while (getline(f, line)) {
        if (line.size() > MaxLineSize)
            line.resize(MaxLineSize);
        result.push_back(line);
    }
    return result;
}

void File_Handler::WriteFile(const string& FileName, wr_mode mode) {
    ofstream f;
    if (mode == wr_mode::Add)
        f.open(FileName, ios::app);
    else
        f.open(FileName);
    if (!f.is_open())
        return;
    for (const auto& line : contentToWrite_)
        f << line << '\n';
    contentToWrite_.clear();
}
