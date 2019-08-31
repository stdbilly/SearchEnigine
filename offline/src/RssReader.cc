#include "Configuration.h"
#include "RssReader.h"
#include <string.h>
#include <boost/regex.hpp>
#include <iostream>
#include <sstream>
using std::cout;
using std::endl;
using std::istringstream;
using std::ostringstream;
using std::to_string;

namespace wd {

RssReader::RssReader(vector<string>& files) : _files(files) {
    cout << "RssReader()" << endl;
}

RssReader::~RssReader() { cout << " ~RssReader()" << endl; }

void RssReader::loadFiles() {
    for (auto& path : _files) {
        loadXML(path);
    }
    cout << ">> parse xml files success" << endl;
}

void RssReader::loadXML(const string& xmlPath) {
    XMLDocument doc;
    doc.LoadFile(xmlPath.c_str());
    if (doc.ErrorID()) {
        cout << ">> tinyxml load file error!" << endl;
        return;
    }
    parseRss(doc);
}

void RssReader::parseRss(XMLDocument& doc) {
    XMLElement* root = doc.RootElement();
    XMLElement* channel = root->FirstChildElement("channel");
    XMLElement* item = channel->FirstChildElement("item");

    do {
        RssItem tmp;
        XMLElement* pNode = item->FirstChildElement("title");
        tmp.title = pNode->GetText();
        pNode = item->FirstChildElement("link");
        tmp.link = pNode->GetText();
        pNode = item->FirstChildElement("description");
        string descrip = pNode->GetText();
        pNode = item->FirstChildElement("content:encoded");

        if (pNode) {
            tmp.content = pNode->GetText();
        } else {
            tmp.content = descrip;
        }

        // boost::regex reg("<[^>]*>");
        boost::regex reg("<.*?>");  //效果一样
        tmp.content = boost::regex_replace(tmp.content, reg, "");
        //去掉多余的空格和换行
        // boost::regex reg2("[\\s]+");
        boost::regex reg2(R"(( )+|(\n)+)");
        tmp.content = boost::regex_replace(tmp.content, reg2, "$1$2");

        _rssItems.push_back(tmp);
    } while (item = item->NextSiblingElement());
}

void RssReader::createXML() {
    string declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
    XMLDocument pageLib;
    pageLib.Parse(declaration.c_str());

    int i = 0;
    for (auto& item : _rssItems) {
        XMLElement* doc = pageLib.NewElement("doc");
        pageLib.InsertEndChild(doc);
        XMLElement* docid = pageLib.NewElement("docid");
        XMLText* docidText = pageLib.NewText(to_string(++i).c_str());
        docid->InsertEndChild(docidText);
        doc->InsertEndChild(docid);
        XMLElement* title = pageLib.NewElement("title");
        title->InsertEndChild(pageLib.NewText(item.title.c_str()));
        doc->InsertEndChild(title);
        XMLElement* link = pageLib.NewElement("link");
        link->InsertEndChild(pageLib.NewText(item.link.c_str()));
        doc->InsertEndChild(link);
        XMLElement* content = pageLib.NewElement("content");
        content->InsertEndChild(pageLib.NewText(item.content.c_str()));
        doc->InsertEndChild(content);
    }

    pageLib.SaveFile(CONFIG[RIPEPAGE_PATH].c_str());
}

void RssReader::makePages(vector<string>& pages) {
    int i = 0;
    for (auto& item : _rssItems) {
        ostringstream oss;
        oss << "<doc>" << '\n'
            << '\t' << "<docid>" << ++i << "</docid>" << '\n'
            << '\t' << "<title>" << item.title << "</title>" << '\n'
            << '\t' << "<link>" << item.link << "</link>" << '\n'
            << '\t' << "<content>" << item.content << "</content>" << '\n'
            << "</doc>" << '\n';
        pages.push_back(oss.str());
    }
}

}  // namespace wd