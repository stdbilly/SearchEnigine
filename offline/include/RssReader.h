#pragma once
#include <string>
#include <vector>
#include "tinyxml2.h"
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
    RssReader(vector<string>& files);
    ~RssReader();
    void loadFiles();
    void makePages(vector<string>& pages);
    void createXML();

   private:
    void parseRss(XMLDocument& doc);
    void loadXML(const string& xmlPath); 
   
   private:
    vector<RssItem> _rssItems;
    vector<string> _files;//所有xml文件
};

}  // namespace wd