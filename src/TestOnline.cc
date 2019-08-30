#include "Configuration.h"
#include "mylogger.h"
using namespace wd;

int main() {
    Configuration::getInstance("/home/whb/project/RssSearchEngine/conf/online.conf");

    Mylogger::destroy();
    return 0;
}