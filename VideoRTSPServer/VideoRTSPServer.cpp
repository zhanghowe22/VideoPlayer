// VideoRTSPServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "RTSPServer.h"
#include "RTPHelper.h"

int main()
{
    /*
    RTSPServer server;
    server.Init();
    server.Invoke();
    printf("press any key to stop\r\n");
    getchar();
    server.Stop();
    */
    RTPHeader header;
    RTPHeader* pheader = &header;
    memset(&header, 0, sizeof(header));
    printf("header size: %d\r\n", sizeof(header));
    header.version = 2;
    header.extension = 0;
    header.padding = 1;
    header.csrccount = 5;
    return 0;
}


