#include "Tr.h"
#include "../core/File_Handler.h"
#include "../utils/Constants.h"
#include "../utils/Exceptions.h"
#include <iostream>

using namespace std;

static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}
static bool parseWhatToken(const string& t, string& outWhat) {
    if (t.size() < 3 || t[0] != '-' || t[1] != '\"') return false;
    size_t last = t.rfind('\"');
    if (last == string::npos || last <= 1) return false;
    outWhat = t.substr(2, last - 2);
    return true;
}
static string replaceAll(const string& text, const string& from, const string& to) {
    if (from.empty()) return text;
    string result;
    result.reserve(text.size());
    size_t pos = 0;
    while (pos < text.size()) {
        size_t found = text.find(from, pos);
        if (found == string::npos) {
            result.append(text, pos, string::npos);
            break;
        }
        result.append(text, pos, found - pos);
        result.append(to);
        pos = found + from.size();
    }
    return result;
}
string Tr::execute() {
    if (what_.empty()) {
        throw TrMissingWhatError();
    }
    if (getInputPipeline() && !input_source_.empty()) {
        throw PipelineStreamConflictError();
    }
    if (getInputPipeline())
        input_source_.clear();
    string content;
    if (input_source_.empty()) {
        string line;
        while (getline(cin, line)) {
            if (line == ";" || trim(line) == "EOF") break;
            if (line.size() > MaxLineSize)
                line.resize(MaxLineSize);
            if (!content.empty()) content += '\n';
            content += line;
        }
    } else if (input_source_.size() >= 2 && input_source_[0] == '\"' && input_source_.back() == '\"') {
        content = input_source_.substr(1, input_source_.size() - 2);
    } else {
        File_Handler fh;
        auto lines = fh.ReadFile(input_source_);
        for (size_t i = 0; i < lines.size(); i++) {
            if (i > 0) content += '\n';
            content += lines[i];
        }
    }
    string out = replaceAll(content, what_, with_);
    if (output_stream_.empty()) {
        cout << out << endl;
    } else {
        string fname = output_stream_;
        while (!fname.empty() && fname[0] == '>') fname.erase(0, 1);
        vector<string> lines;
        size_t pos = 0;
        while (true) {
            size_t n = out.find('\n', pos);
            if (n == string::npos) {
                lines.push_back(out.substr(pos));
                break;
            }
            lines.push_back(out.substr(pos, n - pos));
            pos = n + 1;
        }
        File_Handler fh;
        fh.setContentToWrite(lines);
        fh.WriteFile(fname, output_append_ ? wr_mode::Add : wr_mode::Clean);
    }
    return out;
}

void Tr::addParameters(const vector<string>& tokens) {
    input_source_.clear();
    what_.clear();
    with_.clear();
    output_stream_.clear();
    output_append_ = false;
    int idx_what = -1;
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
        if (!parseWhatToken(t, what_))
            continue;
        idx_what = (int)i;
        break;
    }
    if (idx_what >= 0 && (size_t)(idx_what + 1) < tokens.size()) {
        const string& t = tokens[(size_t)(idx_what + 1)];
        if (t.size() >= 2 && t[0] == '\"' && t.back() == '\"')
            with_ = t.substr(1, t.size() - 2);
    }
    if (idx_what > 1) {
        for (size_t k = 1; k < (size_t)idx_what; k++) {
            if (tokens[k] == ">" || tokens[k] == ">>") { k++; continue; }
            if (!tokens[k].empty() && tokens[k][0] == '>') continue;
            input_source_ = tokens[k];
            break;
        }
    }
}
