#if !defined(L_SSLBYTESTREAM_H)
#define L_SSLBYTESTREAM_H

#include <vector>

#include "SockByteStream.h"

#ifndef SECURITY_WIN32
	#define SECURITY_WIN32 1
	#include <security.h>
#endif
class LSSLContext;//#include "SSLContext.h"

class LSSLByteStream : public LByteStream
{
public:
	//исключения касающиеся SSPI.
	struct SEC_E
	{
		SEC_E(SECURITY_STATUS e):error(e){}
		SECURITY_STATUS error;
	};

	LSSLByteStream(LSockByteStream* SockByteStream);
	LSSLByteStream(LSockByteStream* SockByteStream, LSSLContext* SSLCtxt);
	~LSSLByteStream();

	void SetSSLContext(LSSLContext* SSLCtxt);

	virtual unsigned int RawRecv(void* buf, unsigned int buf_len);

	virtual void RawSend(const void* buf, unsigned int data_len);

private:
	LSockByteStream* m_SockByteStream;
	CtxtHandle* m_hSSLContext;
	SecPkgContext_StreamSizes m_StreamSizes;

	enum{STREAM_HEADER=0, STREAM_DATA=1, STREAM_TRAILER=2,	STREAM_EXTRA=3,
	     EMPTY0=0, EMPTY1=1, EMPTY2=2, EMPTY3=3,
	     decSTREAM_DATA=0};
	SecBufferDesc     m_sendBufferDesc;
	SecBuffer         m_sendBuffers[4];
	std::vector<BYTE> m_send;

	SecBufferDesc     m_recvBufferDesc;
	SecBuffer         m_recvBuffers[4];
	std::vector<BYTE> m_recv;
};

typedef LSSLByteStream::SEC_E LSSL_SEC_E;

#endif //L_SSLBYTESTREAM_H
