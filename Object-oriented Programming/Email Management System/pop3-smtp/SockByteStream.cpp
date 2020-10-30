#include "SockByteStream.h"

unsigned int LSockByteStream::RawRecv(void* buf, unsigned int buf_len)
{
	int ret = recv(m_ServerSock, static_cast<char*>(buf), buf_len, 0);
	switch(ret){
		case SOCKET_ERROR:{
			throw WSAGetLastError();
			break;
		}
		case 0:{
			throw LBS_EOF();
			break;
		}
	}
	//ASSERT(ret>0);
	return ret;
}

void LSockByteStream::RawSend(const void* buf, unsigned int data_len)
{
	unsigned int offset=0;
	do{
		int ret = send(m_ServerSock, static_cast<const char*>(buf)+offset, data_len-offset, 0);
		if(SOCKET_ERROR==ret){
			throw WSAGetLastError();
		}
		//ASSERT(ret>0);
		offset+=ret;
	}
	while(offset<data_len);
}

