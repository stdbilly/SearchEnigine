#pragma once
#include "WebPage.h"
#include <string>
#include <unordered_map>
#include <set>
#include <vector>;
using std::string;
using std::unordered_map;
using std::vector;
using std::set;
using std::pair;

namespace wd {
class WordQuery {
   public:
    WordQuery();

    void loadLibrary();

   private:
    unordered_map<int, WebPage> _pageLib;
    unordered_map<string, set<pair<int, double>>> _invertIndex;
};

}  // namespace wd
