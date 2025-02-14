#pragma once
#include "MyThread.h"
#include "Socket.h"
#include <string>
#include <map>
#include "MyQueue.h"

class RTSPRequest {
public:
	RTSPRequest();
	RTSPRequest(const RTSPRequest& protocol);
	RTSPRequest& operator=(const RTSPRequest& protocol);
	~RTSPRequest();

private:
	int m_method; // 0: OPTIONS 1: DESCRIBE 2:SETUP 3:PLAY 4:TEARDOWN
	std::string m_url;
	std::string m_session;
	int m_seq;
	short m_client_port[2];
};

class RTSPReply {
public:
	RTSPReply();
	RTSPReply(const RTSPReply& protocol);
	RTSPReply& operator=(const RTSPReply& protocol);
	~RTSPReply();
	EBuffer toBuffer();

private:
	int m_method; // 0: OPTIONS 1: DESCRIBE 2:SETUP 3:PLAY 4:TEARDOWN
	short m_client_port[2];
	short m_server_port[2];
	std::string m_sdp;
};

// 会话
class RTSPSession {
public:
	RTSPSession() {}
	RTSPSession(const ESocket& client);
	RTSPSession(const RTSPSession& session);
	RTSPSession& operator=(const RTSPSession& session);
	~RTSPSession() {}
	int PickRequestAndReply();

private:
	EBuffer Pick();
	RTSPRequest AnalyseRequest(const EBuffer& buffer);
	RTSPReply Reply(const RTSPRequest& request);

private:
	std::string m_id;
	ESocket m_client;
};

class RTSPServer : public ThreadFuncBase
{
public:
	RTSPServer() : m_socket(true), m_status(0) ,m_pool(10)
	{
		m_threadMain.UpdateWorker(ThreadWorker(this, (FUNCTYPE)&RTSPServer::threadWorker));
	}

	int Init(const std::string& strIP = "0.0.0.0", short port = 554);

	int Invoke();

	void Stop();

	~RTSPServer();

protected:
	// 返回0继续，返回负数终止，返回其他警告
	int threadWorker();
	int ThreadSession();

private:
	static SocketIniter m_initer;
	ESocket m_socket;
	EAddress m_addr;
	int m_status; // 0: 未初始化 1: 初始化完成 2: 正在运行 3：关闭
	CMyThread m_threadMain;
	MyThreadPool m_pool;
	CMyQueue<RTSPSession> m_lstSession;
};

