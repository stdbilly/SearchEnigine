#pragma ocne
#include <stdlib.h>
#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;

namespace wd {

#define XML_PATH "xmlDir"
#define RIPEPAGE_PATH "ripepageLib"
#define OFFSET_PATH "offsetLib"

class Configuration {
   public:
    static Configuration* getInstance();
    static Configuration* getInstance(const string& filepath);
    static void destroy();

    unordered_map<string,string>& getConfigMap() {
        return _configMap;
    }

   private:
    Configuration(const string& filepath);
    ~Configuration() { cout << "~Configuration()" << endl; }
    void readFile(const string& filename);

   private:
    static Configuration* _pInstance;
    string _filepath;
    unordered_map<string, string> _configMap;
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
