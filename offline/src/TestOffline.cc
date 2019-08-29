#include "DirScanner.h"
#include "Configuration.h"
#include "PageLib.h"
using namespace wd;

int main() {
    Configuration::getInstance("/home/whb/project/RssSearchEnigine/conf/offline.conf");
    DirScanner scanner;
    scanner();
    //displayS(scanner.getFiles());
    PageLib pagelib(scanner);
    pagelib.create();
    pagelib.store();
    return 0;
}