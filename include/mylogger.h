#pragma once
#include <pthread.h>
#include <log4cpp/Category.hh>
#include <iostream>
#include <string>
using std::string;
using namespace log4cpp;

#define suffix(msg)                  \
    std::string(msg)                 \
        .append(" [")                \
        .append(__FILE__)            \
        .append(":")                 \
        .append(__func__)            \
        .append(":")                 \
        .append(to_string(__LINE__)) \
        .append("]")                 \
        .c_str()

class Mylogger {
   public:
    static Mylogger* getInstance();
    static void destroy();
    static void init();

    static void setFilename(const string& filename) { 
        _filename = filename;
    }

    template <typename... Args>
    void warn(Args... args) {
        _mycategory.warn(args...);
    }

    template <typename... Args>
    void error(Args... args) {
        _mycategory.error(args...);
    }

    template <typename... Args>
    void debug(Args... args) {
        _mycategory.debug(args...);
    }

    template <typename... Args>
    void info(Args... args) {
        _mycategory.info(args...);
    }

   private:
    Mylogger();
    ~Mylogger();

   private:
    static string _filename;
    Category& _mycategory;
    static Mylogger* _pInstance;
    static pthread_once_t _once;
};

#define LogError(msg, ...) \
    Mylogger::getInstance()->error(suffix(msg), ##__VA_ARGS__)
#define LogWarn(msg, ...) \
    Mylogger::getInstance()->warn(suffix(msg), ##__VA_ARGS__)
#define LogInfo(msg, ...) \
    Mylogger::getInstance()->info(suffix(msg), ##__VA_ARGS__)
#define LogDebug(msg, ...) \
    Mylogger::getInstance()->debug(suffix(msg), ##__VA_ARGS__)
