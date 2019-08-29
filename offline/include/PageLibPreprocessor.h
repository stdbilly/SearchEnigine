#pragma once
#include "WebPage.h"
#include <vector>
using std::vector;

namespace wd {
class PageLibPreprocessor {
   public:
    PageLibPreprocessor(/* args */);

    void doProcess();
    void readPageFromFile();
    void cutRedundantPages();
    void buildInvertIndex();
    void store();

   private:
    vector<WebPage> _pageLib;
};

}  // namespace wd
