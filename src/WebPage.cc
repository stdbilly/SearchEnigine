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
    string period = "。";
    istringstream iss(_content);
    string line;
    while (iss >> line) {
        for (auto word : queryWords) {
            string result;
            size_t pos = line.find(word);
            if (pos != string::npos) {
                if (length(line) >= 100) {
                    //找到word之前的距离word最近的第一个。
                    size_t p1 = line.rfind(period, pos);
                    size_t p2 = line.find(period, pos);

                    if (p1 != string::npos &&
                        p2 != string::npos) {  // word前后都有句号
                        p1 += getBytes(line[p1]);
                        p2 -= getBytes(line[p2]);

                        result = line.substr(p1, p2 - p1);
                    } else if (p1 != string::npos &&
                               p2 == string::npos) {  // word之后没有句号
                        p1 += getBytes(line[p1]);
                        //取100个字长
                        string temp = line.substr(p1);
                        size_t len = length(temp);
                        if (len > 100) {
                            len = 100;
                        }
                        p2 = p1;
                        for (size_t ilen = 0; ilen < len; ++ilen, ++p2) {
                            size_t bytes = getBytes(temp[p1]);
                            p2 += (bytes - 1);
                        }
                        result = line.substr(p1, p2 - p1);
                    } else if (p1 == string::npos &&
                               p2 != string::npos) {  // word之前没有句号
                        p2 -= getBytes(line[p2]);

                        result = line.substr(0, p2);
                    }
                } else {
                    result = line;
                }
                result.append("...");
                summaryVec.push_back(result);
                break;
            }
        }

        if (summaryVec.size() >= 2) {
            break;
        }
    }
    string summary;
    for (auto s : summaryVec) {
        summary.append(s).append("\n");
    }
    return summary;
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

size_t WebPage::length(const std::string& str) {
    std::size_t ilen = 0;
    for (std::size_t idx = 0; idx != str.size(); ++idx) {
        int nBytes = getBytes(str[idx]);
        idx += (nBytes - 1);
        ++ilen;
    }
    return ilen;
}

}  // namespace wd