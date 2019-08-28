#include "DirScanner.h"
#include "Configuration.h"
using namespace wd;

int main() {
    Configuration::getInstance("/home/whb/project/RssSearchEnigine/conf/offline.conf");
    DirScanner scanner;
    scanner();
    //displayS(scanner.getFiles());
    
    return 0;
}