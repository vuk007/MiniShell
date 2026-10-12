#include "Machine.h"
#include "Lexer.h"
#include "../utils/Exceptions.h"
#include <iostream>
#include <sstream>
#include <vector>


static vector<string> splitPipeline(const string& line) {
    vector<string> segments;
    string seg;
    bool in_quote = false;
    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (c == '\"')
            in_quote = !in_quote;
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
static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

void Machine::start() {
    Lexer lexer;
    string line;

    while (true) {
        cout << prompt_;
        if (!getline(cin, line)) {
            if (cin.eof()) {
                cin.clear();
                cout << "\r" << string(prompt_.size() + 2, ' ') << "\r";
                continue;
            }
            break;
        }
        if (line.empty())
            continue;
        if (line == "exit" || line == "quit")
            break;

        vector<string> segments = splitPipeline(line);
        for (auto& s : segments)
            s = trim(s);

        try {
            if (segments.size() == 1) {
                if (!lexer.runCommand(segments[0])) {
                    pair<vector<string>, vector<int>> result = lexer.lex(segments[0]);
                    vector<string> Token_List = result.first;
                    (void)result.second;
                    if (!Token_List.empty())
                        throw UnknownCommandError(Token_List[0]);
                    else
                        throw NoCommandError();
                }
                continue;
            }

            string pipe_input;
            bool pipeline_ok = true;
            for (size_t i = 0; i < segments.size() && pipeline_ok; i++) {
                if (segments[i].empty()) {
                    throw EmptyPipelineCommandError();
                }
                streambuf* saved_cin = cin.rdbuf();
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
                bool inputPipe = (i > 0);
                bool outputPipe = (i < segments.size() - 1);
                try {
                    if (!lexer.runCommand(segments[i], inputPipe, outputPipe)) {
                        auto [Token_List, TokenPos] = lexer.lex(segments[i]);
                        (void)TokenPos;
                        if (!Token_List.empty())
                            throw UnknownCommandError(Token_List[0]);
                        else
                            throw NoCommandError();
                    }
                } catch (const CLIException& e) {
                    cin.rdbuf(saved_cin);
                    cout.rdbuf(saved_cout);
                    cout << e.what() << endl;
                    pipeline_ok = false;
                    break;
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
            }
        } catch (const CLIException& e) {
            cout << e.what() << endl;
        }
    }
}
