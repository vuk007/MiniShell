#include "Batch.h"
#include "../core/Lexer.h"
#include "../utils/Constants.h"
#include "../utils/Exceptions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

static string batchTrim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static vector<string> splitPipeline(const string& line) {
    vector<string> segments;
    string seg;
    bool in_quote = false;
    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (c == '\"') in_quote = !in_quote;
        if (c == '|' && !in_quote) {
            segments.push_back(seg);
            seg.clear();
            continue;
        }
        seg += c;
    }
    segments.push_back(seg);
    return segments;
}
static void runBatchLine(Lexer& lexer, const string& line) {
    vector<string> segments = splitPipeline(line);
    for (auto& s : segments) s = batchTrim(s);

    if (segments.size() == 1) {
        if (!lexer.runCommand(segments[0])) {
            auto [toks, pos] = lexer.lex(segments[0]);
            (void)pos;
            if (!toks.empty()) throw UnknownCommandError(toks[0]);
            else               throw NoCommandError();
        }
        return;
    }
    string pipe_input;
    for (size_t i = 0; i < segments.size(); i++) {
        if (segments[i].empty()) throw EmptyPipelineCommandError();

        streambuf* saved_cin  = cin.rdbuf();
        streambuf* saved_cout = cout.rdbuf();

        istringstream in_stream;
        if (i > 0) {
            in_stream.str(pipe_input);
            in_stream.clear();
            cin.rdbuf(in_stream.rdbuf());
        }
        ostringstream out_capture;
        if (i < segments.size() - 1)
            cout.rdbuf(out_capture.rdbuf());

        bool ok = false;
        try {
            ok = lexer.runCommand(segments[i], i > 0, i < segments.size() - 1);
        } catch (const CLIException& e) {
            cin.rdbuf(saved_cin);
            cout.rdbuf(saved_cout);
            cerr << e.what() << endl;
            return;
        } catch (...) {
            cin.rdbuf(saved_cin);
            cout.rdbuf(saved_cout);
            throw;
        }

        cin.rdbuf(saved_cin);
        if (i < segments.size() - 1) {
            cout.rdbuf(saved_cout);
            pipe_input = out_capture.str();
        } else {
            cout.rdbuf(saved_cout);
        }

        if (!ok) {
            auto [toks, pos] = lexer.lex(segments[i]);
            (void)pos;
            if (!toks.empty()) throw UnknownCommandError(toks[0]);
            else               throw NoCommandError();
        }
    }
}

string Batch::execute() {
    if (filename_.empty()) {
        cerr << "Error: batch requires a filename argument." << endl;
        return "";
    }
    ifstream f(filename_);
    if (!f.is_open()) {
        cerr << "Error: could not open file '" << filename_ << "'." << endl;
        return "";
    }

    streambuf* saved_cout = nullptr;
    ofstream out_file;
    if (!output_stream_.empty()) {
        string fname = output_stream_;
        while (!fname.empty() && fname[0] == '>') fname.erase(0, 1);
        out_file.open(fname, output_append_ ? ios::app : ios::trunc);
        if (out_file.is_open()) {
            saved_cout = cout.rdbuf();
            cout.rdbuf(out_file.rdbuf());
        }
    }

    Lexer lexer;
    string line;
    while (getline(f, line)) {
        if (line.size() > MaxLineSize) line.resize(MaxLineSize);
        if (line.empty()) continue;
        cin.clear();

        try {
            runBatchLine(lexer, line);
        } catch (const CLIException& e) {
            cerr << e.what() << endl;
        } catch (const std::exception& e) {
            cerr << e.what() << endl;
        } catch (...) {
            cerr << "Error: nepoznata greska" << endl;
        }
    }

    if (saved_cout != nullptr) {
        cout.rdbuf(saved_cout);
        out_file.close();
    }
    return "";
}

void Batch::addParameters(const vector<string>& tokens) {
    filename_.clear();
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
        } else if (filename_.empty()) {
            filename_ = tokens[i];
        }
    }
}
