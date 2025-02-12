#pragma once
#include "MyThread.h"
#include "Socket.h"
#include <string>
#include <map>

class RTSPRequest {
public:
	RTSPRequest();
	RTSPRequest(const RTSPRequest& protocol);
	RTSPRequest& operator=(const RTSPRequest& protocol);
	~RTSPRequest();

private:
	int m_method; // 0: OPTIONS 1: DESCRIBE 2:SETUP 3:PLAY 4:TEARDOWN
};

class RTSReply {
public:
	RTSReply();
	RTSReply(const RTSReply& protocol);
	RTSReply& operator=(const RTSReply& protocol);
	~RTSReply();

private:
	int m_method; // 0: OPTIONS 1: DESCRIBE 2:SETUP 3:PLAY 4:TEARDOWN
};

class RTSPSession {
public:
	RTSPSession();
	RTSPSession(const RTSPSession& session);
	RTSPSession& operator=(const RTSPSession& session);
	~RTSPSession();
};

class RTSPServer : public ThreadFuncBase
{
public:
	RTSPServer() : m_socket(true), m_status(0) 
	{
		m_threadMain.UpdateWorker(ThreadWorker(this, (FUNCTYPE)&RTSPServer::threadWorker));
	}

	int Init(const std::string& strIP = "0.0.0.0", short port = 554);

	int Invoke();

	int Stop();

	~RTSPServer() {}

protected:
	int threadWorker();
	RTSPRequest AnalyseRequest(const std::string& data);
	RTSReply MakeReplay(const RTSPRequest& request);
	int ThreadSession();

private:
	ESocket m_socket;

	EAddress m_addr;

	int m_status; // 0: 未初始化 1: 初始化完成 2: 正在运行 3：关闭

	CMyThread m_threadMain;

	MyThreadPool m_pool;

	std::map<std::string, RTSPSession>m_mapSession;

	static SocketIniter m_initer;;
};

