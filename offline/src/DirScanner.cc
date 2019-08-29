#include "DirScanner.h"
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Configuration.h"
#include "dirent.h"
#include "unistd.h"

namespace wd {
DirScanner::DirScanner() { _files.reserve(20); }

void DirScanner::operator()() {
    traverse(CONFIG[XML_PATH]);
    cout << ">> import xml files" << endl;
    for (auto& file : _files) {
        cout << file << endl;
    }
}

void DirScanner::traverse(const string& dirPath) {
    DIR* pdir = ::opendir(dirPath.c_str());
    if (!pdir) {
        perror("opendir");
        return;
    }
    ::chdir(dirPath.c_str());
    struct dirent* filedirent;
    struct stat filestat;
    while ((filedirent = ::readdir(pdir)) != nullptr) {
        ::stat(filedirent->d_name, &filestat);
        if (S_ISDIR(filestat.st_mode)) {  //是文件夹
            if (strcmp(filedirent->d_name, ".") == 0 ||
                strcmp(filedirent->d_name, "..") == 0) {
                continue;
            } else {
                traverse(filedirent->d_name);  //递归遍历
            }
        } else {
            string filePath;
            filePath.append(::getcwd(NULL, 0))
                .append("/")
                .append(filedirent->d_name);
            _files.push_back(filePath);
        }
    }

    ::chdir("..");
    ::closedir(pdir);
}

vector<string>& DirScanner::getFiles() { return _files; }

}  // namespace wd
