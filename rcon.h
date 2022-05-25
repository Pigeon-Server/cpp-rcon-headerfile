//
// Created by Half_nothing on 2022/5/25.
//
#pragma once
#include <string>
#include <utility>
#include <winsock2.h>
using namespace std;

struct rconpacket
{
    int size;
    int id;
    int cmdnum;
    char data[4096];
};

class rconclient
{
public:
    rconclient(string hostname,int hostport,string passwd):hostname(std::move(hostname)),hostport(hostport),passwd(std::move(passwd)){}
private:
    void net_init_WSA()
    {
        WSADATA wsadata;
        WORD version = MAKEWORD(2, 2);
        int error = WSAStartup(version, &wsadata);
        if (error != 0)
        {
            printf("WSAStartup启动失败\n", error);
            exit(1);
        }
    }
    void sighandler(int signal)
    {
        if (signal == 2)
        {
            putchar('\n');
        }
        connect_alive = 0;
    }
    void exit_proc() const
    {
        if (rsock != -1)
        {
            net_close(rsock);
        }
    }
    static void net_close(int input)
    {
        closesocket(input);
        WSACleanup();
    }
    string hostname;
    int hostport;
    string passwd;
    int rsock;
    int connect_alive = 0;
};
