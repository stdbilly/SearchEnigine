#include "WebPage.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
using std::cout;
using std::endl;
using std::ostringstream;
using std::vector;

// this define can avoid some logs which you don't need to care about.
#define LOGGER_LEVEL LL_WARN

namespace wd {
WebPage::WebPage(int id, const string& title, const string& link,
                 const string& content)
    : _docid(id),
      _title(title),
      _link(link),
      _content(content),
      _simhashVal(0) {}

void WebPage::generateSimhash(WordSegmentation& simhasher) {
    size_t topN = 6;
    _simhashVal = simhasher.makeSimhash(_content, topN);
}

void WebPage::buildWordsMap(WordSegmentation& jieba) {
    unordered_set<string>& stopWords =
        Configuration::getInstance()->getStopWords();

    vector<string> words = jieba.cutWords(_content);

    for (auto& word : words) {
        if (stopWords.count(word) == 0) {
            ++_wordsMap[word];
        }
    }
}

string WebPage::getDoc() {
    ostringstream oss;
    oss << "<doc>" << '\n'
        << '\t' << "<docid>" << _docid << "</docid>" << '\n'
        << '\t' << "<title>" << _title << "</title>" << '\n'
        << '\t' << "<link>" << _link << "</link>" << '\n'
        << '\t' << "<content>" << _content << "</content>" << '\n'
        << "</doc>" << '\n';
    return oss.str();
}

bool WebPage::operator<(const WebPage& rhs) {
    return _simhashVal < rhs._simhashVal;
}

bool WebPage::operator==(const WebPage& rhs) {
    return simhash::Simhasher::isEqual(_simhashVal, rhs._simhashVal);
}

}  // namespace wd
