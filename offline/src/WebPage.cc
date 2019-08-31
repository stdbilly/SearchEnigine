#pragma once
#include "WebPage.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>
#include "Configuration.h"
using std::cout;
using std::endl;
using std::istringstream;
using std::ostringstream;
using std::unordered_set;
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

void WebPage::generateSimhash(simhash::Simhasher& simhasher) {
    using namespace simhash;

    size_t topN = 6;
    vector<pair<string, double>> res;
    simhasher.extract(_content, res, topN);
    simhasher.make(_content, topN, _simhashVal);

    // cout << "docid: " << _docid << endl;
    // cout << "key words: " << res << endl;
    // cout << "simhash: " << _simhashVal << endl;
}

void WebPage::buildWordsMap(cppjieba::Jieba& jieba) {
    using namespace cppjieba;

    unordered_set<string>& stopWords =
        Configuration::getInstance()->getStopWords();

    vector<string> words;
    jieba.CutAll(_content, words);
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
