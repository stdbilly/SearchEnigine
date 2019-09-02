#pragma once
#include <string>
#include <vector>
#include "DirScanner.h"
using std::string;
using std::vector;

namespace wd {
class PageLib {
   public:
    PageLib(DirScanner& scanner);
    void create();
    //void store();

   private:
   DirScanner& _scanner;
   vector<string> _pages;
};

}  // namespace wd
