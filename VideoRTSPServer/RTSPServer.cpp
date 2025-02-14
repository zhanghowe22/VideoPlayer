#include "RTSPServer.h"
#include <rpc.h>

int RTSPServer::Init(const std::string& strIP, short port)
{
    m_addr.Update(strIP, port);
    m_socket.Bind(m_addr);
    m_socket.Listen();
    return 0;
}

int RTSPServer::Invoke()
{
    m_threadMain.Start();
    return 0;
}

void RTSPServer::Stop()
{
    m_socket.Close();
    m_threadMain.Stop();
    m_pool.Stop();
}

RTSPServer::~RTSPServer()
{
    Stop();
}

int RTSPServer::threadWorker()
{
    EAddress client_addr;
    ESocket client = m_socket.Accept(client_addr);
    if (client != INVALID_SOCKET) {
        RTSPSession session(client);
        m_lstSession.PushBack(session);
        m_pool.DispatchWorker(ThreadWorker(this, (FUNCTYPE)&RTSPServer::ThreadSession));
    }  
    return 0;
}

int RTSPServer::ThreadSession()
{
    // TODO: 接收数据请求，解析请求，应答请求
    RTSPSession session;
    if (m_lstSession.PopFront(session)) {
        return session.PickRequestAndReply();
    }
    return -1;
}

RTSPSession::RTSPSession(const ESocket& client)
    :m_client(client)
{
    // 生成唯一的session id
    UUID uuid;
    UuidCreate(&uuid);
    m_id.resize(8);
    snprintf((char*)m_id.c_str(), m_id.size(), "%08d", uuid.Data1);
}

RTSPSession::RTSPSession(const RTSPSession& session)
{
    m_id = session.m_id;
    m_client = session.m_client;
}

RTSPSession& RTSPSession::operator=(const RTSPSession& session)
{
    if (this != &session)
    {
		m_id = session.m_id;
		m_client = session.m_client;
    }
    return *this;
}

int RTSPSession::PickRequestAndReply()
{
    EBuffer buffer = Pick();
    if (buffer.size() <= 0) return -1;
    RTSPRequest req = AnalyseRequest(buffer);
    RTSPReply rep = Reply(req);
    m_client.Send(rep.toBuffer());
    return 0;
}

EBuffer RTSPSession::Pick()
{
	// 依照RTSP的格式，当连续出现两个/r/n时就是结尾了
	EBuffer result;
	int ret = 1;
	EBuffer buf(1);
	while (ret > 0)
	{
		buf.Zero(); // 内存值置零，不会改变大小
		ret = m_client.Recv(buf);
		if (ret > 0) {
			result += buf;
			if (result.size() >= 4) {
				UINT val = *(UINT*)(result.size() - 4 + (char*)result);
				if (val == *(UINT*)"\r\n\r\n") {
					break;
				}
			}
		}
	}
	return result;
}

RTSPRequest RTSPSession::AnalyseRequest(const EBuffer& buffer)
{
    RTSPRequest request;
    // TODO:
    return request;
}

RTSPReply RTSPSession::Reply(const RTSPRequest& request)
{
    RTSPReply reply;
    return reply;
}
