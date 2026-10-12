#include "Date.h"
#include <iostream>
#include <ctime>
#include <cstring>

using namespace std;

string Date::execute() {
    time_t t = time(nullptr);
    tm* tm = localtime(&t);
    if (!tm)
        return "";
    char buf[32];
    if (strftime(buf, sizeof(buf), "%d.%m.%Y.", tm) == 0)
        return "";
    string result(buf);
    cout << result << endl;
    return result;
}

void Date::addParameters(const vector<string>& tokens) {}
