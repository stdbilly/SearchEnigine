#include "WebPage.h"
#include <iostream>
#include <sstream>
#include "Configuration.h"
using std::cout;
using std::endl;
using std::istringstream;

namespace wd {
WebPage::WebPage(int id, const string& title, const string& link,
                 const string& content)
    : _docid(id), _title(title), _link(link), _content(content) {}

string WebPage::summary(const vector<string>& queryWords) {
    vector<string> summaryVec;

    istringstream iss(_content);
    string line;
    while (iss >> line) {
        for (auto word : queryWords) {
            size_t pos = line.find(word);
            if (pos != string::npos) {
                string temp = getNLenString(line, pos, 60);
                temp.append("...");
                summaryVec.push_back(line);
                break;
            }
        }

        if (summaryVec.size() >= 3) {
            break;
        }
    }

    string summary;
    for (auto s : summaryVec) {
        summary.append(s).append("\n");
    }
    return summary;
}

string WebPage::getNLenString(string& str, size_t pos, int len) {
    int ilen = 0;
    size_t beg = pos;

    while (ilen < len && pos != str.size()) {
        size_t bytes = getBytes(str[pos]);
        pos += bytes;
        ++ilen;
    }
    return str.substr(beg, pos);
}

size_t WebPage::getBytes(const char ch) {
    if (ch & (1 << 7)) {
        int nBytes = 1;
        for (int idx = 0; idx != 6; ++idx) {
            if (ch & (1 << (6 - idx))) {
                ++nBytes;
            } else
                break;
        }
        return nBytes;
    }
    return 1;
}

}