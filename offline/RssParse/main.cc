#include "RssParse.h"
#include "tinyxml2.h"
#include <iostream>
using std::cout;
using std::endl;
using namespace tinyxml2;

void test1() {
     RssReader rssR;
     rssR.parseRss("coolshell.xml");
     rssR.dump("pagelib.dat");
}
int main() {
    test1();
    return 0;
}

