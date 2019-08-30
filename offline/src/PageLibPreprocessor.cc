#include "PageLibPreprocessor.h"
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Configuration.h"
#include "tinyxml2.h"
using std::cout;
using std::endl;
using std::ofstream;
using namespace tinyxml2;

namespace wd {
PageLibPreprocessor::PageLibPreprocessor() { _pageLib.reserve(400); }

void PageLibPreprocessor::doProcess() {
    readPageFromFile();
    cutRedundantPages();
    store();
}

void PageLibPreprocessor::readPageFromFile() {
    XMLDocument doc;
    doc.LoadFile(CONFIG[RIPEPAGE_PATH].c_str());
    XMLElement* page = doc.FirstChildElement("doc");

    do {
        string docid = page->FirstChildElement("docid")->GetText();
        string title = page->FirstChildElement("title")->GetText();
        string link = page->FirstChildElement("link")->GetText();
        string content = page->FirstChildElement("content")->GetText();

        _pageLib.emplace_back(std::stoi(docid), title, link, content);
    } while (page = page->NextSiblingElement());
}

void PageLibPreprocessor::cutRedundantPages() {
    cout << ">> before cut: " << _pageLib.size() << endl;
    int i = 0;
    for (auto& page : _pageLib) {
        cout << ++i << endl;
        page.generateSimhash();
    }
    std::sort(_pageLib.begin(), _pageLib.end());

    /* ofstream ofsSort("/home/whb/project/RssSearchEngine/offline/data/sorted.dat");
    for (auto& page : _pageLib) {
        string temp = page.getDoc();
        ofsSort << page.getSimhash() << '\n' << temp;
    }
    cout << ">> store sorted ripepage success" << endl;
    ofsSort.close();  */

    auto it = std::unique(_pageLib.begin(), _pageLib.end());

    _pageLib.erase(it, _pageLib.end());
    cout << ">> after cut: " << _pageLib.size() << endl;
    cout << ">> cut redundant pages success" << endl;
}

void PageLibPreprocessor::store() {
    ofstream ofsPage(CONFIG[NEW_RIPEPAGE_PATH]);
    ofstream ofsOffset(CONFIG[NEW_OFFSET_PATH]);

    for (auto& page : _pageLib) {
        int id = page.getDocId();
        string temp = page.getDoc();
        ofstream::pos_type offset = ofsPage.tellp();
        size_t length = temp.size();
        ofsPage << temp;
        ofsOffset << id << '\t' << offset << '\t' << length << '\n';
    }
    cout << ">> store new ripepage and offset success" << endl;
    ofsPage.close();
    ofsOffset.close();
}

}  // namespace wd
