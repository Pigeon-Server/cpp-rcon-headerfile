//
// Created by Half_nothing on 2022/5/25.
//
#pragma once
#include <string>
#include <utility>
#include <iostream>
using namespace std;

class rconclient
{
public:
    rconclient(string hostname,int hostport,string passwd):hostname(std::move(hostname)),hostport(hostport),passwd(std::move(passwd)){}
    void printinfo()
    {
        cout << hostname << hostport << passwd << endl;
    }
private:
    string hostname;
    int hostport;
    string passwd;
};
