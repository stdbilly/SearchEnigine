#pragma once
#include <string>
#include <vector>
#include "DirScanner.h"
using std::string;
using std::vector;

namespace wd {
class PageLib {
   public:
    PageLib(/* args */);
    ~PageLib();

   private:
   DirScanner& scnner;
   vector<string> _pages;
};

PageLib::PageLib(/* args */) {}

PageLib::~PageLib() {}

}  // namespace wd
