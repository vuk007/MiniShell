#include "Time.h"
#include "../core/File_Handler.h"
#include <iostream>
#include <ctime>
#include <cstring>

using namespace std;

string Time::execute() {
    time_t t = time(nullptr);
    tm* tm = localtime(&t);
    if (!tm)
        return "";
    char buf[32];
    if (strftime(buf, sizeof(buf), "%H:%M:%S", tm) == 0)
        return "";
    string result(buf);
    if (output_stream_.empty()) {
        cout << result << endl;
    } else {
        string fname = output_stream_;
        while (!fname.empty() && fname[0] == '>') fname.erase(0, 1);
        File_Handler fh;
        fh.setContentToWrite(vector<string>{result});
        fh.WriteFile(fname, output_append_ ? wr_mode::Add : wr_mode::Clean);
    }
    return result;
}

void Time::addParameters(const vector<string>& tokens) {
    output_stream_.clear();
    output_append_ = false;
    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i] == ">" || tokens[i] == ">>") {
            output_append_ = (tokens[i] == ">>");
            if (i + 1 < tokens.size() && !tokens[i + 1].empty())
                output_stream_ = tokens[i + 1];
            i++;
        } else if (!tokens[i].empty() && tokens[i][0] == '>') {
            output_append_ = (tokens[i].size() >= 2 && tokens[i][1] == '>');
            output_stream_ = tokens[i];
        }
    }
}
