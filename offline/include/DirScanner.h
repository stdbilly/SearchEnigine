#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

namespace wd {
class DirScanner {
   public:
    DirScanner();

    void operator()();
    vector<string>& getFiles();
    void traverse(const string& dirPath);
   private:
    vector<string> _files;//存放每个xml文件的绝对路径
};

}  // namespace wd
