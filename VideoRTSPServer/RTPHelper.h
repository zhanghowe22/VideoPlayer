#pragma once
#include "base.h"
#include "Socket.h"

/* 开发总结
 * RTSP 负责控制媒体流，而 RTP 负责传输媒体数据，RTCP 负责监控传输质量。
 * 1. 一般较为复杂的数据类型，声明为类而非结构体，这样可以在构造函数中进行初始化，方便调用者使用
 * 2. 结构体和类的区别：默认结构体都是public属性和方法，默认类都是private属性和方法
 * 3. 位域的高位和低位的情况要进行分别： DataSheet中，默认左边是高位，右边是低位；位域中先声明的是低位，后声明的是高位，以字节为单位
 * 把DataSheet数据定义转为代码的几个注意事项：1) 位域 2) 位域当中的顺序,以字节位单位，先声明的是低位，后声明的是低位 3) 如果跨字节了，先声明的在前面，后声明的在后面，按字节排序
 * 4. 一个RTP包的长度不要超过1400字节，主要是为了避免 IP 分片，容易造成数据丢失
*/

// 头部类
class RTPHeader {
public:
	unsigned short csrccount : 4;
	unsigned short extension : 1; // 扩展位
	unsigned short padding : 1; // 填充位
	unsigned short version : 2; // 位域
	unsigned short pytype : 7;
	unsigned short mark : 1;
	unsigned short serial;
	unsigned timestamp;
	unsigned ssrc;
	unsigned csrc[15];
	
public:
	RTPHeader();
	operator EBuffer();
	RTPHeader(const RTPHeader& header);
	RTPHeader& operator=(const RTPHeader& header);
};

class RTPFrame {
public:
	RTPHeader m_head; // 头部
	EBuffer m_pyload; // 数据负载
	operator EBuffer();
};

class RTPHelper
{
public:
	RTPHelper() :timestamp(0), m_udp(false) {
		m_udp.Bind(EAddress("0.0.0.0", (short)55000));
	}

	~RTPHelper() {}

	int SendMediaFrame(RTPFrame& rtpframe, EBuffer& frame, const EAddress& client);

private:
	int GetFrameSepSize(EBuffer& frame);
	int SendFrame(const EBuffer& frame, const EAddress& client);
	DWORD timestamp;
	ESocket m_udp;
};

