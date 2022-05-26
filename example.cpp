//
// Created by Half_nothing on 2022/5/25.
//
#include "rcon.h"
#include <iostream>
using namespace std;
int main()
{
    char* output; //定义输出
    int authed;
    auto rcon = new rconclient("127.0.0.1","1337","test");//创建类，需要提供地址、端口和密码
    authed = rcon->rconauth();//验证身份
    if (authed != 1)
    {
      cout << "认证失败" << endl;
      return -1;
    }
    output = rcon->rcon_command((char*)"help");//发送命令并接受返回包
    cout << output << endl;
    return 0;
}
