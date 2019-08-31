#pragma once
#include "WebPage.h"
#include <vector>
using std::vector;
using std::pair;

namespace wd {
class PageLibPreprocessor {
   public:
    PageLibPreprocessor();

    void doProcess();
    void readPageFromFile();
    void cutRedundantPages();
    void buildInvertIndex();
    void store();

   private:
    WordSegmentation _jieba;
    vector<WebPage> _pageLib;
    unordered_map<string, vector<pair<int, double>>> _invertIndexTable;
};

}  // namespace wd
