#include "WordQuery.h"
#include <algorithm>
#include <cmath>
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
struct SimilarityCompare {
    SimilarityCompare(vector<double>& base) : _base(base) {}

    bool operator()(
        const pair<int, vector<double>>& lhs,
        const pair<int, vector<double>>& rhs) {  // 都与基准向量进行计算
        double lhsCrossProduct = 0;
        double rhsCrossProduct = 0;
        double lhsVectorLength = 0;
        double rhsVectorLength = 0;

        for (int index = 0; index != _base.size(); ++index) {
            lhsCrossProduct += (lhs.second)[index] * _base[index];
            rhsCrossProduct += (rhs.second)[index] * _base[index];
            lhsVectorLength += pow((lhs.second)[index], 2);
            rhsVectorLength += pow((rhs.second)[index], 2);
        }

        if (lhsCrossProduct / sqrt(lhsVectorLength) <
            rhsCrossProduct / sqrt(rhsVectorLength)) {
            return false;
        } else {
            return true;
        }
    }
    vector<double> _base;
};

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
    if (str.size() == 0) {
        return returnNoAnswer();
    }
    vector<string> queryWords;
    queryWords = _jieba(str);
    for (auto& item : queryWords) {
        auto it = _invertIndex.find(item);
        if (it == _invertIndex.end()) {
            LogInfo("%s, not found", item);
            return returnNoAnswer();
        }
    }
    //计算每个词的权重
    vector<double> weightList = getQueryWordsWeight(queryWords);
    SimilarityCompare similarityCom(weightList);
    vector<int> pages = getPages(queryWords);
    if (pages.size() == 0) {
        LogInfo("Not found");
        return returnNoAnswer();
    }

    vector<pair<int, vector<double>>> resultList;
    for (auto& docid : pages) {
        vector<double> wordWeight;
        for (auto& word : queryWords) {
            auto tmpSet = _invertIndex.find(word)->second;
            for (auto& item : tmpSet) {
                if (item.first == docid) {
                    wordWeight.push_back(item.second);
                }
            }
        }
        resultList.emplace_back(make_pair(docid, weightList));
    }
    std::stable_sort(resultList.begin(), resultList.end());
    pages.clear();
    for (auto& item : resultList) {
        pages.push_back(item.first);
    }
    return createJson(pages, queryWords);
}

vector<int> WordQuery::getPages(vector<string> queryWords) {
    vector<int> pages;
    for (auto& item : _invertIndex[queryWords[0]]) {
        pages.push_back(item.first);
    }
    std::sort(pages.begin(), pages.end());
    for (auto it = queryWords.begin() + 1; it != queryWords.end(); ++it) {
        vector<int> temp, rhs;
        for (auto& item : _invertIndex[*it]) {
            rhs.push_back(item.first);
        }
        std::sort(rhs.begin(), rhs.end());
        std::set_intersection(pages.begin(), pages.end(), rhs.begin(),
                              rhs.end(), std::back_inserter(temp));
        pages.swap(temp);
    }
    return pages;
}

vector<double> WordQuery::getQueryWordsWeight(vector<string>& queryWords) {
    unordered_map<string, int> wordFreq;
    for (auto& item : queryWords) {
        ++wordFreq[item];
    }
    //计算权重
    vector<double> weightVec;
    double weightSum = 0;
    int totalPageNum = _pageLib.size();

    for (auto& word : queryWords) {
        int tf = wordFreq[word];
        int df = _invertIndex[word].size();
        double idf = log2(static_cast<double>(totalPageNum / (df + 1)));
        double w = idf * tf;
        weightSum += pow(w, 2);
        weightVec.push_back(w);
    }
    //归一化
    for (auto& item : weightVec) {
        item /= sqrt(weightSum);
    }
    return weightVec;
}

string WordQuery::createJson(vector<int>& docIdVec,
                             const vector<string>& queryWords) {
    Json::Value root;
    Json::Value arr;

    int cnt = 0;
    for (auto id : docIdVec) {
        string summary = _pageLib[id].summary(queryWords);
        string title = _pageLib[id].getTitle();
        string url = _pageLib[id].getUrl();

        Json::Value elem;
        elem["title"] = title;
        elem["summary"] = summary;
        elem["url"] = url;
        arr.append(elem);
        if (++cnt == 100)  // 最多记录100条
            break;
    }

    root["files"] = arr;
    Json::StyledWriter writer;
    return writer.write(root);
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
