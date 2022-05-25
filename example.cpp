//
// Created by Half_nothing on 2022/5/25.
//
#include <cstdio>
#include "rcon.h"
using namespace std;

int main()
{
    auto rcon = new rconclient("127.0.0.1",1337,"test");
    rcon -> printinfo();
}