#pragma once
#include "mylogger.h"
#include <hiredis/hiredis.h>
#include <string>
using std::string;

namespace wd {
class Redis {
   public:
    Redis() : _connect(NULL), _reply(NULL) {}

    ~Redis() {
        if (_connect) {
            redisFree(_connect);
        }
        _connect = NULL;
        _reply = NULL;
    }

    bool connect(const string& ip, int port) {
        _connect = redisConnect(ip.c_str(), port);
        if (_connect == NULL || _connect->err) {
            if (_connect) {
                LogError("Error: %s", _connect->errstr);
            } else {
                LogError("Can't allocate redis context");
            }
            return false;
        }
        LogDebug("Connect to Redis server success");
        return true;
    }

    string get(const string& key) {
        _reply = (redisReply*)redisCommand(_connect, "GET %s", key.c_str());
        LogDebug("Succeed to execute command: GET %s", key.c_str());

        if(_reply->type == REDIS_REPLY_NIL) {
            return string("-1");
        }
        string value = _reply->str;
        freeReplyObject(_reply);
        return value;
    }

    void set(const string& key, const string& value) {
        redisCommand(_connect, "SET %s %s", key.c_str(), value.c_str());
        LogDebug("Succeed to execute command: SET %s %s", key.c_str(), value.c_str());
    }

   private:
    redisContext* _connect;
    redisReply* _reply;
};

}  // namespace wd
