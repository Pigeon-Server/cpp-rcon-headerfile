# Cpp-rcon-headerfile
CPP head file about the RCON protocol  
封装rcon协议的cpp头文件

## How to use?  
一个例子：
```
#include "rcon.h"
#include <iostream>
using namespace std;
int main()
{
    char* output; //定义输出
    int authed;//是否认证成功
    auto rcon = new rconclient("127.0.0.1","1337","test");//创建类，需要提供地址、端口和密码
    authed = rcon->rconauth();//验证身份,认证成功返回1，否则认证失败
    if (authed != 1)
    {
      cout << "认证失败" << endl;
      return -1;
    }
    output = rcon->rcon_command((char*)"help");//发送命令并接受返回包
    cout << output << endl;
    return 0;
}
```
这个例子我放在了[example.cpp](example.cpp)里面，你可以下载编译一下  
记得在服务器的[```server.properties```](https://minecraft.gamepedia.com/Server.properties)文件里开启RCON  
```
enable-rcon=true
rcon.port=25575
rcon.password=your_rcon_pasword
```

## Compile and operation  
下载build.bat文件，正确配置g++路径即可一件编译  
或者你可以手动在编译时加上-lwsock32 -lws2_32这两个参数  

## Existing problems  
我没写多包响应，以及没有适配颜色代码  
也就是说在连接bukkit等启用颜色代码的服务端的时候会出一点问题  
所以此头文件只是我下一个项目的前置，并不存在实际生产价值  
