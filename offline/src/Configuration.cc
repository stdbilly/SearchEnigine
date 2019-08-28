#include "../include/Configuration.h"
#include <fstream>
#include <iostream>
#include <sstream>
using std::cout;
using std::endl;
using std::ifstream;
using std::istringstream;
using std::make_pair;

namespace wd {
Configuration* Configuration::getInstance() {
    if (_pInstance)
        return _pInstance;
    else
        return nullptr;
}

Configuration* Configuration::getInstance(const string& filepath) {
    if (_pInstance == nullptr) {
        ::atexit(destroy);
        _pInstance = new Configuration(filepath);
    }
    return _pInstance;
}

void Configuration::destroy() {
    if (_pInstance) delete _pInstance;
}

Configuration::Configuration(const string& filepath) : _filepath(filepath) {
    readFile(filepath);
    cout << "Configuration(const string&)" << endl;
}

void Configuration::readFile(const string& filename) {
    ifstream ifs(filename);
    if (!ifs) {
        perror("fopen");
        return;
    }
    string line, key, value;
    while (getline(ifs, line)) {
        istringstream iss(line);
        iss >> key >> value;
        _configMap.insert(make_pair(key, value));
    }
    cout << ">> read config file success" << endl;
}

Configuration* Configuration::_pInstance = nullptr;
}  // namespace wd
