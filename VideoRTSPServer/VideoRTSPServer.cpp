// VideoRTSPServer.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "RTSPServer.h"

int main()
{
    RTSPServer server;
    server.Init();
    server.Invoke();
    printf("press any key to stop\r\n");
    getchar();
    server.Stop();
    return 0;
}


