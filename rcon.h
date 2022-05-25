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
    int rconauth() {
        rsock = net_connect();
        int tempdata;
        rconpacket *packet = packet_build(0xBADC0DE, 3, (char*)passwd);
        if (packet == nullptr) {
            return 0;
        }
        tempdata = netsendpacket(rsock, packet);
        if (!tempdata) {
            return 0;
        }
        packet = netrecvpacket(rsock);
        if (packet == nullptr) {
            return 0;
        }
        return packet->id == -1 ? 0 : 1;
    }
    char* rcon_command(char *command)
    {
        rconpacket *packet = packet_build(0xBADC0DE, 2, command);
        if (packet == nullptr)
        {
            connect_alive = 0;
            return (char*)"0";
        }
        int tempdata = netsendpacket(rsock, packet);
        if (!tempdata) {
            return (char*)"0";
        }
        packet = netrecvpacket(rsock);
        if (packet == nullptr)
        {
            return (char*)"0";
        }
        if (packet->id != 0xBADC0DE)
        {
            return (char*)"0";
        }
        if (packet->size > 10)
        {
            return packet->data;
        }
        return (char*)"1";
    }
private:
    rconpacket* netrecvpacket(int socket)
    {
        int packetsize;
        rconpacket packet = {0, 0, 0, {0x00}};
        int tempdata = recv(socket, (char *)&packetsize, sizeof(int), 0);
        if (tempdata == 0)
        {
            printf("连接已丢失\n");
            connect_alive = 0;
            return nullptr;
        }
        if (tempdata != sizeof(int))
        {
            printf("错误:接收失败。无效的数据包大小(%d)\n", tempdata);
            connect_alive = 0;
            return nullptr;
        }
        if (packetsize < 10 || packetsize > 4096)
        {
            printf("警告:无效数据包大小(%d)。必须大于10且小于%d.\n", packetsize, 4096);
            if (packetsize > 4096 || packetsize < 0)
                packetsize = 4096;
            net_clean_incoming(socket, packetsize);
            return nullptr;
        }
        packet.size = packetsize;
        int received = 0;
        while (received < packetsize)
        {
            tempdata = recv(socket, (char *)&packet + sizeof(int) + received, packetsize - received, 0);
            if (tempdata == 0)
            {
                printf("连接已丢失\n");
                connect_alive = 0;
                return nullptr;
            }
            received += tempdata;
        }
        rconpacket* ret = &packet;
        return ret;
    }
    int net_clean_incoming(int socket, int size)
    {
        char data[size];
        int tempdata = recv(socket, data, size, 0);
        if (tempdata == 0)
        {
            printf("连接已丢失\n");
            connect_alive = 0;
        }
        return tempdata;
    }
    static int netsendpacket(int socket, rconpacket *packet)
    {
        int len;
        int total = 0;
        int byte;
        int tempdata = -1;
        byte = len = packet->size + sizeof(int);
        while (total < len)
        {
            tempdata = send(socket, (char *)packet + total, byte, 0);
            if (tempdata == -1)
            {
                break;
            }
            total += tempdata;
            byte -= tempdata;
        }
        return tempdata == -1 ? -1 : 1;
    }
    static rconpacket *packet_build(int id, int cmd, char *s1)
    {
        static rconpacket packet = {0, 0, 0, {0x00}};
        int len = strlen(s1);
        if (len >= 4096)
        {
            printf("指令过长(%d)最大长度为: %d.\n", len, 4096 - 1);
            return nullptr;
        }
        packet.size = sizeof(int) * 2 + len + 2;
        packet.id = id;
        packet.cmdnum = cmd;
        strncpy(packet.data, s1, 4096 - 1);
        return &packet;
    }
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
    static void net_close(int input)
    {
        closesocket(input);
        WSACleanup();
    }
    const char* hostname;
    const char *hostport;
    const char *passwd;
    int rsock{};
    int connect_alive = 0;
};
