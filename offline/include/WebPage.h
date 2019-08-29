#pragma ocne
#include <string>
#include <unordered_map>
using std::unordered_map;
using std::string;

namespace wd {
class WebPage {
   public:
    WebPage(int id, const string& title, const string& link, const string& content);

    int getDocId() { return _docid; }
    string getDoc();
    void generateSimhash();
    void buildWordsMap();
    bool operator<(const WebPage& rhs);
    bool operator==(const WebPage& rhs);

   private:
    int _docid;
    string _title;
    string _link;
    string _content;
    uint64_t _simhashVal;
    unordered_map<string, int> _wordsMap;
};

}  // namespace wd
