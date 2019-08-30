#pragma ocne
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::unordered_set;

namespace wd {

#define XML_PATH "xmlDir"
#define RIPEPAGE_PATH "ripepageLib"
#define OFFSET_PATH "offsetLib"
#define NEW_RIPEPAGE_PATH  "newRipepageLib"
#define NEW_OFFSET_PATH "newOffsetLib"
#define INDEX_PATH "invertIndex"
#define DICT_PATH "dict"
#define HMM_PATH "hmm_model"
#define IDF_PATH "idf"
#define STOP_WORD_PATH "stop_words"  
#define USER_DICT_PATH "user_dict" 

class Configuration {
   public:
    static Configuration* getInstance();
    static Configuration* getInstance(const string& filepath);
    static void destroy();

    std::unordered_map<string,string>& getConfigMap() {
        return _configMap;
    }
    
    unordered_set<string>& getStopWords();

   private:
    Configuration(const string& filepath);
    ~Configuration() { cout << "~Configuration()" << endl; }
    void readFile(const string& filename);

   private:
    static Configuration* _pInstance;
    string _filepath;
    std::unordered_map<string, string> _configMap;
    unordered_set<string> _stopWords;
};

#define CONFIG Configuration::getInstance()->getConfigMap()

template <class Container>
void display(const Container& c) {
    typename Container::const_iterator cit = c.begin();
    for (; cit != c.end(); ++cit) {
        cout << cit->first << " ---> " << cit->second << endl;
    }
}

template <class Container>
void displayS(const Container& c) {
    typename Container::const_iterator cit = c.begin();
    while (cit != c.end()) {
        cout << *cit << endl;;
        ++cit;
    }
    //cout << endl;
}

}  // namespace wd
