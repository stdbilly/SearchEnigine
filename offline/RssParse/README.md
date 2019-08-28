## [使用tinyXml2解析RSS文件，并生成一个网页库pagelib.dat](RssParse.h)

tinyXml2 -- https://github.com/leethomason/tinyxml2
rss    --   http://www.runoob.com/rss/rss-tutorial.html

   	正则表达式 进行过滤 html的标签

   参考接口:

```C++
struct RssItem
{
    string title;
    string link;
    string description;
    string content;
};

class RssReader
{
public:
    RssReader();
    
    void parseRss();//解析
    void dump(const string & filename);//输出
private:
    vector<RssItem> _rss;
};   
```

要求：最后生成一个 pagelib.dat, 其格式:
```xml
<doc>
  <docid>1</docid>
  <title> ... </title>
  <link> ...  </link>
  <content> ... </content>
</doc>
<doc>
  ...
</doc>
<doc>
  ...
</doc>
```

 RSS文件解析思路：         
    xml -->  rss --> tinyxml2 --> boost::regex/std::regex