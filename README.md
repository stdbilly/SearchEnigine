# Search Enigine

### 简介

基于文本检索的轻量级搜索引擎

### 特性

- 使用[针对中文文档的simhash算法库](https://github.com/yanyiwu/simhash)进行网页去重
- 使用[cppjieba](https://github.com/yanyiwu/cppjieba)进行中文分词
- 使用[tinyxml2](https://github.com/leethomason/tinyxml2)解析和保存网页库
- 通过TF-IDF算法建立网页库的倒排索引
- 服务端框架采用了Reactor + 线程池的网络库
- 对查找到的网页进行排序采用了余弦相似度算法
- 将查询结果封装成`json`字符串发送给前端
- 使用`Redis`作为缓存系统
- 使用`log4cpp`作为日志系统

### 依赖

- [针对中文文档的simhash算法库](https://github.com/yanyiwu/simhash)

- [CppJieba](https://github.com/yanyiwu/cppjieba)

- [TinyXML-2](https://github.com/leethomason/tinyxml2)

- [JsonCpp](https://github.com/open-source-parsers/jsoncpp) (version = 1.8.3 is recommended)

- [log4cpp](http://log4cpp.sourceforge.net/)

- Redis

  ```shell
  sudo apt install redis-server
  git clone git@github.com:redis/hiredis.git
  cd hiredis
  make
  ./test.sh
  sudo make install
  ```

### 用法

- ##### 离线部分

  ```shell
  cd offline
  mkdir bin
  make
  ./bin/offline.exe
  ```

- ##### 在线部分

  ```shell
  cd ..
  mkdir bin
  make
  ./bin/SearchEngine.exe
  ```

- ##### 前端搭建

  ```shell
  sudo apt-get update
  sudo apt-get upgrade
  sudo apt-get install apache2
  sudo apt-get install php7.2 libapache2-mod-php7.2
  sudo cp php_TCP /var/www/html/
  ```

  在浏览器中输入http://127.0.0.1/php_TCP/index.html

### 演示

离线部分

![](.assets/Screenshot%20from%202019-09-02%2021-08-50.png)

在线部分

![](.assets/Screenshot%20from%202019-09-02%2021-23-23.png)

![](.assets/Screenshot%20from%202019-09-02%2021-21-17.png)

