#pragma ocne
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;

namespace wd {
class WebPage {
   public:
    WebPage(int id, const string& title, const string& link,
            const string& content);

    WebPage() {};

    int getDocId() { return _docid; }
    string getTitle() { return _title; }
    string getContent() { return _content; }
    string getUrl() { return _link; }
    string summary(const vector<string>& queryWords);

   private:
    size_t getBytes(const char ch);
    size_t length(const std::string &str);

   private:
    int _docid;
    string _title;
    string _link;
    string _content;
};

}  // namespace wd
