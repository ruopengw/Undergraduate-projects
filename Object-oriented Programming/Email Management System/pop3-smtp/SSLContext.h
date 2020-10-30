#if !defined(L_SSLCONTEXT_H)
#define L_SSLCONTEXT_H

#include "SockByteStream.h"

#include <vector>

#ifndef SECURITY_WIN32
#	define SECURITY_WIN32 1
#	include <security.h>
#endif

class LSSLContext
{
public:
	LSSLContext();
	~LSSLContext();

	void AcquireCredentials();
	void Handshake(LSockByteStream* SockByteStream, const TCHAR* TargetName);
	void Shutdown(LSockByteStream* SockByteStream, const TCHAR* TargetName);

	CtxtHandle* GetContextHandle() {return &m_hContext;}

	const std::vector<BYTE>& GetHandshakeExtra() const {return m_HandshakeExtra;};

private:
	ULONG m_ReqContextAttr;
	CredHandle m_hUserCred;
	TimeStamp  m_UserCredExpiry;

	CtxtHandle m_hContext;
	TimeStamp  m_ContextExpiry;
	ULONG m_ContextAttr;

	std::vector<BYTE> m_HandshakeExtra;
};

#endif //L_SSLCONTEXT_H
