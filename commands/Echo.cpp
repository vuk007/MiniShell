#include "Echo.h"
#include "../utils/Constants.h"
#include "../core/File_Handler.h"
#include <iostream>

using namespace std;

void Echo::addParameters(const vector<string>& tokens) {
    input_stream_.clear();
    output_stream_.clear();
    output_append_ = false;
    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i] == "<") {
            if (i + 1 < tokens.size())
                input_stream_ = tokens[i + 1];
            i++;
        } else if (!tokens[i].empty() && tokens[i][0] == '<') {
            input_stream_ = tokens[i];
        } else if (tokens[i] == ">" || tokens[i] == ">>") {
            output_append_ = (tokens[i] == ">>");
            if (i + 1 < tokens.size() && !tokens[i + 1].empty())
                output_stream_ = tokens[i + 1];
            i++;
        } else if (!tokens[i].empty() && tokens[i][0] == '>') {
            output_append_ = (tokens[i].size() >= 2 && tokens[i][1] == '>');
            output_stream_ = tokens[i];
        } else if (input_stream_.empty()) {
            input_stream_ = tokens[i];
        }
    }
}

string Echo::execute() {
    vector<string> res;
    if (input_stream_.empty()) {
        string line;
        while (getline(cin, line)) {
            if (line == ";" || line == "EOF") break;
            if (line.size() > MaxLineSize)
                line.resize(MaxLineSize);
            res.push_back(line);
        }
    } else if (input_stream_[0] == '\"') {
        string s = input_stream_;
        if (s.size() >= 2) {
            s.erase(0, 1);
            s.pop_back();
        }
        res.push_back(s);
    } else {
        string fname = input_stream_;
        if (fname[0] == '<')
            fname.erase(0, 1);
        File_Handler fh;
        res = fh.ReadFile(fname);
    }

    string out;
    for (size_t i = 0; i < res.size(); i++) {
        if (i > 0) out += '\n';
        out += res[i];
    }

    if (output_stream_.empty()) {
        cout << out << endl;
    } else {
        string fname = output_stream_;
        while (!fname.empty() && fname[0] == '>') fname.erase(0, 1);
        File_Handler fh;
        fh.setContentToWrite(res);
        fh.WriteFile(fname, output_append_ ? wr_mode::Add : wr_mode::Clean);
    }
    return out;
}

