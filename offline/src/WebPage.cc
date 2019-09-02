#include "WebPage.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <vector>
using std::cout;
using std::endl;
using std::ostringstream;
using std::vector;
using std::to_string;

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

void WebPage::insertDoc(XMLDocument& pageLib) {
    XMLElement* doc = pageLib.NewElement("doc");
    pageLib.InsertEndChild(doc);
    XMLElement* docid = pageLib.NewElement("docid");
    XMLText* docidText = pageLib.NewText(to_string(_docid).c_str());
    docid->InsertEndChild(docidText);
    doc->InsertEndChild(docid);
    XMLElement* title = pageLib.NewElement("title");
    title->InsertEndChild(pageLib.NewText(_title.c_str()));
    doc->InsertEndChild(title);
    XMLElement* link = pageLib.NewElement("link");
    link->InsertEndChild(pageLib.NewText(_link.c_str()));
    doc->InsertEndChild(link);
    XMLElement* content = pageLib.NewElement("content");
    content->InsertEndChild(pageLib.NewText(_content.c_str()));
    doc->InsertEndChild(content);
}

#if 0
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
#endif

bool WebPage::operator<(const WebPage& rhs) {
    return _simhashVal < rhs._simhashVal;
}

bool WebPage::operator==(const WebPage& rhs) {
    return simhash::Simhasher::isEqual(_simhashVal, rhs._simhashVal);
}

}  // namespace wd
