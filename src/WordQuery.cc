#include "WordQuery.h"
#include <fstream>
#include <sstream>
#include "json/json.h"
#include "mylogger.h"
#include "tinyxml2.h"
using namespace tinyxml2;
using std::ifstream;
using std::istringstream;
using std::make_pair;
using std::stoi;

namespace wd {
WordQuery::WordQuery() {
    _pageLib.reserve(300);
    loadLibrary();
}

void WordQuery::loadLibrary() {
    XMLDocument doc;
    doc.LoadFile(CONFIG[NEW_RIPEPAGE_PATH].c_str());
    XMLElement* page = doc.FirstChildElement("doc");
    XMLError errid = doc.ErrorID();
    if (errid) {
        LogError("tinyxml load ripepage error");
        return;
    }

    do {
        string docid = page->FirstChildElement("docid")->GetText();
        string title = page->FirstChildElement("title")->GetText();
        string link = page->FirstChildElement("link")->GetText();
        string content = page->FirstChildElement("content")->GetText();

        WebPage webpage(stoi(docid), title, link, content);
        _pageLib.insert(make_pair(stoi(docid), webpage));
    } while (page = page->NextSiblingElement());
    LogDebug("load rigepage success, size = %d", _pageLib.size());

    // load invert index table
    ifstream ifsIndex(CONFIG[INDEX_PATH]);
    if (!ifsIndex.good()) {
        LogError("ifstream load invert index error");
        return;
    }
    string word, line;
    int docid;
    double weight;
    while (getline(ifsIndex, line)) {
        istringstream iss(line);
        iss >> word;
        set<pair<int, double>> tempSet;
        while (iss >> docid >> weight) {
            tempSet.insert(make_pair(docid, weight));
        }
        _invertIndex.insert(make_pair(word, tempSet));
    }
    ifsIndex.close();
    LogDebug("load invert index success, size = %d", _invertIndex.size());
}

string WordQuery::doQuery(const string& str) {
    return returnNoAnswer();
}

string WordQuery::returnNoAnswer() {
    Json::Value root;
    Json::Value arr;

    Json::Value elem;
    elem["title"] = "404, not found";
    elem["summary"] = "未找到你搜索的内容";
    elem["url"] = "";
    arr.append(elem);
    root["files"] = arr;
    Json::StyledWriter writer;
    return writer.write(root);
}

}  // namespace wd
