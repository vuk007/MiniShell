

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H
#include "../utils/Constants.h"
#include <string>
#include <vector>

enum class wr_mode {
    Clean,
    Add
};

using namespace std;
class File_Handler {

public:
    const vector<string> ReadFile(const string& FileName);
    void WriteFile(const string& FileName, wr_mode mode);
    void setContentToWrite(const vector<string>& content) { contentToWrite_ = content; }

private:
    vector<string> contentToWrite_;
};



#endif //FILE_HANDLER_H
