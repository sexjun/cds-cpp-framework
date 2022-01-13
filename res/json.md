# json

## 引入头文件
```c++
#include "nlohmann/json.hpp"
using json = nlohmann::json;
```

## 简单使用
- 方法一
```c++
json j = R"(
{
    "name": "Judd Trump",
    "credits": 1754500,
    "ranking": 1
}
)"_json;
```
- 方法二
```c++
json j{
    { "name", "Judd Trump"},
    { "credits", 1754500 },
    { "ranking", 1}
};
```

## 解析与序列化
```c++
std::string s = R"(
{
    "name": "Judd Trump",
    "credits": 1754500,
    "ranking": 1
}
)";
auto j = json::parse(s);
```