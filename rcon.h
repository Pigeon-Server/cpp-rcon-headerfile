//
// Created by Half_nothing on 2022/5/25.
//
#pragma once
#include <string>
#include <utility>
#include <winsock2.h>
#include <ws2tcpip.h>

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
    rconclient(const char* hostname,const char* hostport,const char* passwd):hostname(hostname),hostport(hostport),passwd(passwd){}
    int net_connect()
    {
        int tempdata;
        struct addrinfo info{};
        struct addrinfo *server_info, *sinfo;
        memset(&info, 0, sizeof info);
        info.ai_family = 0;
        info.ai_socktype = 1;
        info.ai_protocol = 6;
        net_init_WSA();
        int ret = getaddrinfo(hostname, hostport, &info, &server_info);
        if (ret != 0)
        {
            printf("名称解析失败。\n");
            printf("错误：%d: %s", ret, gai_strerror(ret));
            exit(1);
        }
        for (sinfo = server_info; sinfo != nullptr; sinfo = sinfo->ai_next)
        {
            tempdata = socket(sinfo->ai_family, sinfo->ai_socktype, sinfo->ai_protocol);
            if (tempdata == -1)
            {
                continue;
            }
            ret = connect(tempdata, sinfo->ai_addr, sinfo->ai_addrlen);
            if (ret == -1)
            {
                net_close(tempdata);
                continue;
            }
            break;
        }
        if (sinfo == nullptr)
        {
            printf("连接失败\n");
            freeaddrinfo(server_info);
            exit(1);
        }
        freeaddrinfo(server_info);
        return tempdata;
    }
private:
    static void net_init_WSA()
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
    const char* hostname;
    const char *hostport;
    string passwd;
    int rsock{};
    int connect_alive = 0;
};
