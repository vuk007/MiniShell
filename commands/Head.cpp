#include "Head.h"
#include "../core/File_Handler.h"
#include "../utils/Constants.h"
#include "../utils/Exceptions.h"
#include <iostream>
#include <cctype>

using namespace std;

static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string Head::execute() {
    if (line_count_ == 0) {
        throw HeadInvalidOptionError();
    }
    vector<string> lines;
    if (input_from_stdin_) {
        string line;
        while (getline(cin, line)) {
            if (line == ";" || trim(line) == "EOF") break;
            if (line.size() > MaxLineSize)
                line.resize(MaxLineSize);
            lines.push_back(line);
        }
    } else if (input_from_quoted_ && !input_source_.empty()) {
        size_t pos = 0;
        while(true) {
            size_t n = input_source_.find('\n', pos);
            if (n == string::npos) {
                if (pos < input_source_.size())
                    lines.push_back(input_source_.substr(pos));
                break;
            }
            lines.push_back(input_source_.substr(pos, n - pos));
            pos = n + 1;
        }
    } else if (!input_source_.empty()) {
        File_Handler fh;
        lines = fh.ReadFile(input_source_);
    } else {
        throw HeadNoInputError();
    }

    if (line_count_ < lines.size())
        lines.resize(line_count_);

    string out;
    for (size_t i = 0; i < lines.size(); i++) {
        if (i > 0) out += '\n';
        out += lines[i];
    }

    if (output_stream_.empty()) {
        cout << out << endl;
    } else {
        string fname = output_stream_;
        while (!fname.empty() && fname[0] == '>') fname.erase(0, 1);
        File_Handler fh;
        fh.setContentToWrite(lines);
        fh.WriteFile(fname, output_append_ ? wr_mode::Add : wr_mode::Clean);
    }
    return out;
}

void Head::addParameters(const vector<string>& tokens) {
    line_count_ = 0;
    input_source_.clear();
    input_from_quoted_ = false;
    input_from_stdin_ = false;
    output_stream_.clear();
    output_append_ = false;
    bool n_given = false;
    for (size_t i = 1; i < tokens.size(); i++) {
        const string& t = tokens[i];
        if (t == ">" || t == ">>") {
            output_append_ = (t == ">>");
            if (i + 1 < tokens.size() && !tokens[i + 1].empty())
                output_stream_ = tokens[i + 1];
            i++;
            continue;
        }
        if (!t.empty() && t[0] == '>') {
            output_append_ = (t.size() >= 2 && t[1] == '>');
            output_stream_ = t;
            continue;
        }
        if (t.size() >= 3 && t[0] == '-' && t[1] == 'n') {
            size_t j = 2;
            while (j < t.size() && j < 2 + 5 && isdigit((unsigned char)t[j])) j++;
            if (j > 2 && j <= 2 + 5) {
                int num = 0;
                for (size_t k = 2; k < j; k++)
                    num = num * 10 + (t[k] - '0');
                line_count_ = (size_t)num;
                n_given = true;
            }
            continue;
        }
        if (!n_given) continue;
        if (t.size() >= 2 && t[0] == '\"' && t.back() == '\"') {
            input_from_quoted_ = true;
            input_source_ = t.substr(1, t.size() - 2);
        } else if (!t.empty()) {
            input_source_ = t;
        }
    }
    if (n_given && input_source_.empty())
        input_from_stdin_ = true;
}
