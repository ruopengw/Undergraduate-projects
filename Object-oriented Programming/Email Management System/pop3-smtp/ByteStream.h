#if !defined(L_BYTE_STREAM_H)
#define L_BYTE_STREAM_H

class LByteStream
{
public:
	struct BS_EOF{};//eof exception.
	//some other implementation specific exceptions are possible.

	virtual unsigned int RawRecv(void* buf, unsigned int buf_len)=0;
	virtual void RawSend(const void* buf, unsigned int data_len)=0;
};
typedef LByteStream::BS_EOF LBS_EOF;

#endif //L_BYTE_STREAM_H
