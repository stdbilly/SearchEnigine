#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "tinyxml2.h"
using std::ofstream;
using std::string;
using std::vector;
using namespace tinyxml2;

namespace wd {

struct RssItem {
    string title;
    string link;
    string description;
    string content;
};

class RssReader {
   public:
    RssReader();
    ~RssReader();
    void parseRss(const char* xmlPath);  //解析
    void dump(const char* filename);     //输出
    friend ofstream& operator<<(ofstream& ofs, const RssReader& rhs);

   private:
    XMLElement* getItem(XMLDocument& doc, const char* xmlPath);
    void writeToRssItem(XMLElement* item, vector<RssItem>& rss);

    vector<RssItem> _rss;
};

}  // namespace wd