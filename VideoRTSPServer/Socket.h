#pragma once

#include <WinSock2.h>
#include <iostream>
#include <share.h>

#pragma warning(disable:6031)

class Socket
{
public:
	// bIsTcp 1:TCP 0:UDP
	Socket(bool bIsTcp = true) {
		m_sock = INVALID_SOCKET;
		if (bIsTcp) {
			m_sock = socket(PF_INET, SOCK_STREAM, 0);
		}
		else {
			m_sock = socket(PF_INET, SOCK_DGRAM, 0);
		}
	}

	Socket(SOCKET s) {
		m_sock = s;
	}


	void Close() {
		if (m_sock != INVALID_SOCKET) {
			SOCKET temp = m_sock;
			m_sock = INVALID_SOCKET;
			closesocket(temp);
		}
		closesocket(m_sock);
	}

	operator SOCKET() {
		return m_sock;
	}

	~Socket() {
		Close();
	}

private:
	SOCKET m_sock;
};

class EAddress
{
public:
	EAddress() {
		m_port = -1;
		memset(&m_addr, 0, sizeof(m_addr));
		m_addr.sin_family = AF_INET;
	}

	EAddress(const EAddress& addr) {
		m_ip = addr.m_ip;
		m_port = addr.m_port;
		memcpy(&m_addr, &addr.m_addr, sizeof(sockaddr_in));
	}

	EAddress& operator=(const EAddress& addr) {
		if (this != &addr) {
			m_ip = addr.m_ip;
			m_port = addr.m_port;
			memcpy(&m_addr, &addr.m_addr, sizeof(sockaddr_in));
		}
		return *this;
	}

	~EAddress() {}

	void Update(const std::string& ip, short port) {
		m_ip = ip;
		m_port = port;
		m_addr.sin_port = htons(port);
		m_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	}

	operator const sockaddr* () const {
		return (sockaddr*)&m_addr;
	}

	operator sockaddr_in* () {
		return &m_addr;
	}

	int size() const { return sizeof(sockaddr_in); }

private:
	std::string m_ip;
	short m_port;
	sockaddr_in m_addr;
};

class ESocket {
public:
	ESocket(bool isTcp = true) 
		:m_socket(new Socket(isTcp)) {}

	ESocket(const ESocket& sock) :
		m_socket(sock.m_socket) 
	{}

	ESocket& operator=(const ESocket& sock) {
		if (this != &sock) {
			m_socket = sock.m_socket;
		}
		return *this;
	}

	~ESocket() {
		m_socket.reset();
	}

	operator SOCKET() {
		return *m_socket;
	}

	int Bind(const EAddress& addr) 
	{
		return bind(*m_socket, addr, addr.size());
	}

	int Listen(int backlog = 5) {
		return listen(*m_socket, backlog);
	}

	int Connect(const EAddress& addr) {
		return connect(*m_socket, addr, addr.size());
	}

private:
	std::shared_ptr<Socket> m_socket;
};

class SocketIniter {
public:
	SocketIniter() {
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);
	}

	~SocketIniter() {
		WSACleanup();
	}
};

