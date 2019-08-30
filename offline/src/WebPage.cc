#pragma once
#include "WebPage.h"
#include <iostream>
#include <sstream>
#include <unordered_set>
#include "Configuration.h"
using std::istringstream;
using std::ostringstream;
using std::unordered_set;

// this define can avoid some logs which you don't need to care about.
#define LOGGER_LEVEL LL_WARN

#include "cppjieba/Jieba.hpp"
#include "simhash/Simhasher.hpp"
using namespace simhash;
using namespace cppjieba;

namespace wd {
WebPage::WebPage(int id, const string& title, const string& link,
                 const string& content)
    : _docid(id),
      _title(title),
      _link(link),
      _content(content),
      _simhashVal(0) {}

void WebPage::generateSimhash() {
    Simhasher simhasher(CONFIG[DICT_PATH], CONFIG[HMM_PATH], CONFIG[IDF_PATH],
                        CONFIG[STOP_WORD_PATH]);
    size_t topN = 5;
    vector<pair<string, double>> res;
    simhasher.extract(_content, res, topN);
    simhasher.make(_content, topN, _simhashVal);

    cout << "docid: " << _docid << endl;
    cout << "关键词序列: " << res << endl;
    cout << "simhash值: " << _simhashVal << endl;
}

void WebPage::buildWordsMap() {
    unordered_set<string>& stopWords =
        Configuration::getInstance()->getStopWords();

    Jieba jieba(CONFIG[DICT_PATH], CONFIG[HMM_PATH], CONFIG[USER_DICT_PATH]);

    istringstream iss(_content);
    string s;
    while (iss >> s) {
        vector<string> words;
        jieba.Cut(s, words, true);
        for (auto& word : words) {
            if (stopWords.count(word) == 0) {
                ++_wordsMap[word];
            }
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
    return Simhasher::isEqual(_simhashVal, rhs._simhashVal);
}

}  // namespace wd
