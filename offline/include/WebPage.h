#pragma ocne
#include <string>
#include <unordered_map>
#include "WordSegmentation.h"
#include "tinyxml2.h"
using namespace tinyxml2;
using std::unordered_map;
using std::string;

namespace wd {
class WebPage {
   public:
    WebPage(int id, const string& title, const string& link, const string& content);

    int getDocId() { return _docid; }
    uint64_t getSimhash() { return _simhashVal; }
    void insertDoc(XMLDocument& pageLib);
    //string getDoc();
    unordered_map<string, int>& getWordsMap() { return _wordsMap; }

    void generateSimhash(WordSegmentation& simhasher);
    void buildWordsMap(WordSegmentation& jieba);
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
