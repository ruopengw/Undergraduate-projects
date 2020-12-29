#if !defined(W_SOCK_HELPERS_H)
#define W_SOCK_HELPERS_H

#include <winsock2.h>

//////////////////////////////////////////////////////////////////////
// GetBinAddr
//////////////////////////////////////////////////////////////////////
u_long GetBinAddr(const char* Host);

//////////////////////////////////////////////////////////////////////
// class UseWinSock
//////////////////////////////////////////////////////////////////////
class UseWinSock
{
	void Init(WORD wVersionRequested)
	{
		ZeroMemory(&wsaData, sizeof(wsaData));
		WSAStartupError = WSAStartup( wVersionRequested, &wsaData );
	}

public:
	UseWinSock(BYTE bVersionMajorRequested, BYTE bVersionMinorRequested)
	{
		Init( MAKEWORD(bVersionMajorRequested, bVersionMinorRequested) );
	}
	UseWinSock(WORD wVersionRequested)
	{
		Init( wVersionRequested );
	}

	operator bool(){return WSAStartupError==0;}
	const ::WSADATA& WSADATA(){return wsaData;}

	~UseWinSock(){if(0==WSAStartupError) /*VERIFY(0==*/WSACleanup()/*)*/;}

private:
	int WSAStartupError;
	::WSADATA wsaData;
};

//////////////////////////////////////////////////////////////////////
// class AutoCloseSocket
//////////////////////////////////////////////////////////////////////
class AutoCloseSocket
{
public:
	AutoCloseSocket(SOCKET hSock)
		:m_hSock(hSock){}
	~AutoCloseSocket() {closesocket(m_hSock);}
private:
	const SOCKET m_hSock;
};

//////////////////////////////////////////////////////////////////////
// class AutoShutdownConnection
//////////////////////////////////////////////////////////////////////
class AutoShutdownConnection
{
public:
	AutoShutdownConnection(SOCKET hSock, int how = SD_BOTH)
		:m_hSock(hSock), m_HowToShutdown(how){}
	~AutoShutdownConnection() {shutdown(m_hSock, m_HowToShutdown);}
private:
	const SOCKET m_hSock;
	const int    m_HowToShutdown;
};

#endif //W_SOCK_HELPERS_H
