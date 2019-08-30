#pragma ocne
#include <string>
#include <vector>
#include <unordered_map>
using std::unordered_map;
using std::string;
using std::vector;

namespace wd {
class WebPage {
   public:
    WebPage(int id, const string& title, const string& link, const string& content);

    int getDocId() { return _docid; }
    //uint64_t getSimhash() { return _simhashVal; }
    string getDoc();
    unordered_map<string, int>& getWordsMap() { return _wordsMap; }

    //void generateSimhash();
    void buildWordsMap();
    string summary(const vector<string>& queryWords);

    bool operator<(const WebPage& rhs);
    bool operator==(const WebPage& rhs);

   private:
    int _docid;
    string _title;
    string _link;
    string _content;
    //uint64_t _simhashVal;
    unordered_map<string, int> _wordsMap;
};

}  // namespace wd
