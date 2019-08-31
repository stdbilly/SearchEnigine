#include "WordQuery.h"
#include <fstream>
#include "Configuration.h"
#include "mylogger.h"
using std::ifstream;
using std::stoi;
using std::make_pair;

namespace wd {
    WordQuery::WordQuery() {
        _pageLib.reserve(300);
        loadLibrary();
    }

void WordQuery::loadLibrary() {
    ifstream ifsPage(CONFIG[NEW_RIPEPAGE_PATH]);
    ifstream ifsOffset(CONFIG[NEW_OFFSET_PATH]);
    if(!ifsOffset.good() && !ifsPage.good()) {
        LogError("ifstream open file error");
        return;
    } 
    
    LogDebug("load rigepage success, size = %d", _pageLib.size());
}

}  // namespace wd
