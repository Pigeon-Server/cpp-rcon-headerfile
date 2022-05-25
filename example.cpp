//
// Created by Half_nothing on 2022/5/25.
//
#include "rcon.h"
#include <iostream>
using namespace std;


int main()
{
    char* output;
    auto rcon = new rconclient("127.0.0.1","1337","test");
    rcon->rconauth();
    output = rcon->rcon_command((char*)"help");
    cout << output << endl;
}