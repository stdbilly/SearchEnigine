#include "Configuration.h"
#include "PageLib.h"
#include <fstream>
#include "RssReader.h"
using std::ofstream;

namespace wd {
PageLib::PageLib(DirScanner& scanner) : _scanner(scanner) {}

void PageLib::create() {
    vector<string>& files = _scanner.getFiles();
    RssReader reader(files);
    reader.loadFiles();
    //reader.makePages(_pages);
    reader.createXML();
}

#if 0
void PageLib::store() {
    ofstream ofsPage(CONFIG[RIPEPAGE_PATH]);
    ofstream ofsOffset(CONFIG[OFFSET_PATH]);
    int i = 0;
    for (auto& page : _pages) {
        ofstream::pos_type offset = ofsPage.tellp();
        size_t length = page.size();
        ofsPage << page;
        ofsOffset << ++i << '\t' << offset << '\t' << length << '\n';
    }
    cout << ">> store ripepage and offset success" << endl;
    ofsPage.close();
    ofsOffset.close();
}
#endif
}  // namespace wd
