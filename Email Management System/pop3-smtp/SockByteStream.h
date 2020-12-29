#if !defined(L_SOCKBYTESTREAM_H)
#define L_SOCKBYTESTREAM_H

#include "ByteStream.h"
#include <winsock2.h>

class LSockByteStream: public LByteStream
{
public:
	LSockByteStream(SOCKET ServerSock)
		:m_ServerSock(ServerSock){};
	virtual unsigned int RawRecv(void* buf, unsigned int buf_len);
	virtual void RawSend(const void* buf, unsigned int data_len);

private:
	SOCKET m_ServerSock;
};
#endif //L_SOCKBYTESTREAM_H
