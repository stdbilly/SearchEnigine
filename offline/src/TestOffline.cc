#include "DirScanner.h"
#include "PageLib.h"
#include "PageLibPreprocessor.h"
#include "tinyxml2.h"
using namespace wd;
using namespace tinyxml2;

int main() {
    Configuration::getInstance(
        "/home/whb/project/RssSearchEngine/offline/conf/offline.conf");
    DirScanner scanner;
    scanner();
    PageLib pagelib(scanner);
    pagelib.create();
    pagelib.store();

    PageLibPreprocessor processer;
    processer.doProcess();

    return 0;
}