#include "RssParse.h"
#include <boost/regex.hpp>
using std::cout;
using std::endl;

namespace wd {

RssReader::RssReader() { cout << "RssReader()" << endl; }

RssReader::~RssReader() { cout << " ~RssReader()" << endl; }

void RssReader::parseRss(const char* xmlPath) {
    XMLDocument doc;
    XMLElement* item = getItem(doc, xmlPath);
    while (item) {
        writeToRssItem(item, _rss);
        item = item->NextSiblingElement("item");
    }
}

void RssReader::dump(const char* filename) {
    ofstream ofs;
    ofs.open(filename, std::ios_base::app);
    ofs << *this;
    ofs.close();
}

void RssReader::writeToRssItem(XMLElement* item, vector<RssItem>& rss) {
    boost::regex reg("<[^>]*>");
    RssItem tmp;
    XMLElement* pNode = item->FirstChildElement("title");
    tmp.title = pNode->GetText();
    pNode = item->FirstChildElement("link");
    tmp.link = pNode->GetText();
    pNode = item->FirstChildElement("description");
    tmp.description = pNode->GetText();
    tmp.description = boost::regex_replace(
        tmp.description, reg, "", boost::match_default | boost::format_all);
    pNode = item->FirstChildElement("content:encoded");
    tmp.content = pNode->GetText();
    tmp.content = boost::regex_replace(
        tmp.content, reg, "", boost::match_default | boost::format_all);
    rss.push_back(tmp);
}

XMLElement* RssReader::getItem(XMLDocument& doc, const char* xmlPath) {
    doc.LoadFile(xmlPath);
    XMLElement* root = doc.RootElement();
    XMLElement* channel = root->FirstChildElement("channel");
    XMLElement* item = channel->FirstChildElement("item");
    return item;
}

ofstream& operator<<(ofstream& ofs, const RssReader& rhs) {
    int i = 0;
    for (auto ri : rhs._rss) {
        ofs << "<doc>" << '\n'
            << '\t'
            << "<docid>" << ++i << "</docid>" << '\n'
            << '\t'
            << "<title>" << ri.title << "</title>" << '\n'
            << '\t'
            << "<link>" << ri.link << "</link>" << '\n'
            << '\t'
            << "<content>" << ri.content << "</content>" << '\n'
            << "</doc>" << '\n';
    }
    return ofs;
}

}  // namespace wd