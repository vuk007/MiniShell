#include "Wc.h"
#include "../core/File_Handler.h"
#include "../utils/Constants.h"
#include "../utils/Exceptions.h"
#include <iostream>
#include <cctype>
#include <sstream>

using namespace std;

static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

string Wc::execute() {
    if (!option_given_) {
        throw WcMissingOptionError();
    }
    if (getInputPipeline() && (!input_arg_.empty() || input_from_quoted_)) {
        throw PipelineStreamConflictError();
    }
    if (getInputPipeline())
        input_from_stdin_ = true;
    string content;
    if (input_from_stdin_) {
        string line;
        while (getline(cin, line)) {
            if (line == ";" || trim(line) == "EOF") break;
            if (line.size() > MaxLineSize)
                line.resize(MaxLineSize);
            if (!content.empty()) content += '\n';
            content += line;
        }
    } else if (input_from_quoted_ && !input_arg_.empty()) {
        content = input_arg_;
    } else if (!input_arg_.empty()) {
        File_Handler fh;
        auto lines = fh.ReadFile(input_arg_);
        for (size_t i = 0; i < lines.size(); i++) {
            if (i > 0) content += '\n';
            content += lines[i];
        }
    } else {
        throw WcNoInputError();
    }

    size_t count = 0;
    if (mode_ == Mode::Chars) {
        count = content.size();
    } else {
        bool in_word = false;
        for (unsigned char c : content) {
            if (isspace(static_cast<unsigned char>(c))) {
                in_word = false;
            } else {
                if (!in_word) { count++; in_word = true; }
            }
        }
    }
    cout << count << endl;
    return to_string(count);
}

void Wc::addParameters(const vector<string>& tokens) {
    mode_ = Mode::Words;
    input_arg_.clear();
    option_given_ = false;
    input_from_quoted_ = false;
    input_from_stdin_ = false;
    for (size_t i = 1; i < tokens.size(); i++) {
        if (tokens[i] == "-w") {
            mode_ = Mode::Words;
            option_given_ = true;
        } else if (tokens[i] == "-c") {
            mode_ = Mode::Chars;
            option_given_ = true;
        } else if (!tokens[i].empty() && tokens[i][0] == '\"' && tokens[i].back() == '\"') {
            input_from_quoted_ = true;
            input_arg_ = tokens[i].substr(1, tokens[i].size() - 2);
        } else if (!tokens[i].empty() && tokens[i] != "-w" && tokens[i] != "-c") {
            input_arg_ = tokens[i];
        }
    }
    if (option_given_ && input_arg_.empty())
        input_from_stdin_ = true;
}
