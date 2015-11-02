
#pragma once
#if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED)
#define NTAPI __stdcall
#else
#define _cdecl
#define __cdecl
#define NTAPI
#endif

#include "GameGlobalSetting.h"
#include "cocos2d.h"

#include "Packet.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID||CC_TARGET_PLATFORM==CC_PLATFORM_IOS)
#define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
#define CopyMemory RtlCopyMemory
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;
//typedef char TCHAR, *PTCHAR;
#endif

#ifdef WIN32
#include <windows.h>
#include <WinSock.h>
#else
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#endif

#ifndef CHECKF
#define CHECKF(x) \
	do \
{ \
	if (!(x)) { \
	printf("CHECKF:%s %s", __FILE__, __LINE__); \
	return 0; \
	} \
} while (0)
#endif

#define _MAX_MSGSIZE 16 * 1024		// 暂定一个消息最大为16k
#define BLOCKSECONDS	30			// INIT函数阻塞时间
#define INBUFSIZE	(64*1024)		//?	具体尺寸根据剖面报告调整  接收数据的缓存
#define OUTBUFSIZE	(SOCKET_TCP_PACKET)		//? 具体尺寸根据剖面报告调整。 发送数据的缓存，当不超过8K时，FLUSH只需要SEND一次

enum eSocketStatus
{
	eSoccketUnknown = 0,
	eSocketConnecting = 1,
	eSocketConnected = 2,
	eSocketConnectFailed = 3,
};

//网络接口
struct IGameSocketSink
{
	//网络事件
public:
	// 连接事件
	virtual bool OnEventTCPSocketLink(bool bConnectSucc) = 0;
	//关闭事件
	virtual bool OnEventTCPSocketShut(unsigned char cbShutReason) = 0;
};

class CGameSocket {
public:
	CGameSocket(void);
	~CGameSocket();
	bool	Connect(const char* ip, unsigned short por, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);
	bool	SendMsg(unsigned short wMainCmdID, unsigned short wSubCmdID, void* pBuf, int nSize);
	bool	ReceiveMsg(void* pBuf, int& nSize);
	bool	Flush(void);
	bool	Check(void);
	void	Destroy(void);
	SOCKET	GetSocket(void) const { return m_sockClient; }
	bool	IsConnected();
	void	SetGameSocketSink(IGameSocketSink* pSocketSink){ m_pGameSocketSink = pSocketSink; }
	void	SetMaxKeepAliveTime(unsigned int nMaxTime){ m_nMaxKeepAliveTime = nMaxTime; }
	//加密函数
protected:
	//解密数据
	unsigned short CrevasseBuffer(unsigned char cbDataBuffer[], unsigned short wDataSize);
	//加密数据
	unsigned short EncryptBuffer(unsigned char cbDataBuffer[], unsigned short wDataSize, unsigned short wBufferSize);

	//字节映射
	inline unsigned short SeedRandMap(unsigned short wSeed);
	//发送映射
	inline unsigned char MapSendByte(unsigned char cbData);
	//接收映射
	inline unsigned char MapRecvByte(unsigned char cbData);
	
private:
	bool	recvFromSock(void);		// 从网络中读取尽可能多的数据
	bool    hasError();			// 是否发生错误，注意，异步模式未完成非错误
	void    closeSocket();
	void	ConnectThread();
	bool	Create(const char* pszServerIP, int nServerPort, int nBlockSec = BLOCKSECONDS, bool bKeepAlive = false);

	//加密数据
protected:
	unsigned char							m_cbSendRound;						//字节映射
	unsigned char							m_cbRecvRound;						//字节映射
	unsigned int							m_dwSendXorKey;						//发送密钥
	unsigned int							m_dwRecvXorKey;						//接收密钥

	unsigned int							m_dwSendTickCount;					//发送时间
	unsigned int							m_dwRecvTickCount;					//接收时间
	unsigned int							m_dwSendPacketCount;				//发送计数
	unsigned int							m_dwRecvPacketCount;				//接受计数
	// 连接信息
	sockaddr_in								m_sockaddr;
	int										m_nBlockSec;						// 连接超时时间
	bool									m_bKeepAlive;						// socket KeepAlive
	bool									m_bRunConnectThread;				// 
	eSocketStatus							m_eSocketStatus;					// socket 状态

	SOCKET	m_sockClient;
	IGameSocketSink* m_pGameSocketSink;	// 连接监听回调

	// 发送数据缓冲
	char	m_bufOutput[SOCKET_TCP_BUFFER];	//? 可优化为指针数组
	int		m_nOutbufLen;

	// 环形缓冲区
	char	m_bufInput[INBUFSIZE];
	int		m_nInbufLen;
	int		m_nInbufStart;				// INBUF使用循环式队列，该变量为队列起点，0 - (SIZE-1)

	unsigned int							m_dwPreRecvTick;					// 接收包时间戳
	unsigned int							m_nMaxKeepAliveTime;				// 判断是否掉线标识
};
