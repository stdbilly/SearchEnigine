#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Configuration.h"
#include "cppjieba/Jieba.hpp"
using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace wd {
class WordSegmentation {
   public:
    WordSegmentation()
        : _jieba(CONFIG[DICT_PATH], CONFIG[HMM_PATH], CONFIG[USER_DICT_PATH],
                 CONFIG[IDF_PATH], CONFIG[STOP_WORD_PATH]) {
        cout << ">> jieba init" << endl;
    }

    vector<string> operator()(const string& str) {
        vector<string> words;
        _jieba.Cut(str, words, true);
        auto stopWords = Configuration::getInstance()->getStopWords();
        for (auto it = words.begin(); it != words.end(); ++it) {
            auto uit = stopWords.find(*it);
            if (uit != stopWords.end()) {
                words.erase(it);
            }
        }
        return words;
    }

   private:
    cppjieba::Jieba _jieba;
};

}  // namespace wd
