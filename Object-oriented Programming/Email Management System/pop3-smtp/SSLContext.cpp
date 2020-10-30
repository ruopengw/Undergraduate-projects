#include "SSLContext.h"

#include <vector>

#ifdef UNDER_CE
#include <schnlsp.h>
#else
#include <schannel.h>
#endif

LSSLContext::LSSLContext()
	:m_ReqContextAttr(ISC_REQ_CONFIDENTIALITY|ISC_REQ_MANUAL_CRED_VALIDATION|ISC_REQ_STREAM|ISC_REQ_ALLOCATE_MEMORY)
{
	memset(&m_hUserCred, 0, sizeof(m_hUserCred));
	memset(&m_hContext, 0, sizeof(m_hContext));
}

LSSLContext::~LSSLContext()
{
	BYTE stub1[sizeof(m_hContext)]; memset(stub1, 0, sizeof(stub1));
	if(0!=memcmp(&m_hContext, stub1, sizeof(m_hContext))){DeleteSecurityContext(GetContextHandle());
		memset(&m_hContext, 0, sizeof(m_hContext));
	}

	BYTE stub2[sizeof(m_hUserCred)]; memset(stub2, 0, sizeof(stub2));
	if(0!=memcmp(&m_hUserCred, stub2, sizeof(m_hUserCred))){FreeCredentialsHandle(&m_hUserCred);
		memset(&m_hUserCred, 0, sizeof(m_hUserCred));
	}
}

void LSSLContext::AcquireCredentials()
{

	SCHANNEL_CRED credData;
	ZeroMemory(&credData, sizeof(credData));
	credData.dwVersion = SCHANNEL_CRED_VERSION;

	SECURITY_STATUS sec_s = AcquireCredentialsHandle(0, UNISP_NAME, SECPKG_CRED_OUTBOUND, 0, &credData, 0, 0, &m_hUserCred, &m_UserCredExpiry);
}

void LSSLContext::Handshake(LSockByteStream* SockByteStream, const TCHAR* TargetName)
{
	SecPkgInfo* PkgInfo;
	/*VERIFY(SEC_E_OK==*/QuerySecurityPackageInfo(UNISP_NAME, &PkgInfo)/*)*/;
	std::vector<BYTE> SockDataBuf(PkgInfo->cbMaxToken);
	FreeContextBuffer(PkgInfo);

	//SSPI:HandShake
	SecBufferDesc InBufferDesc;
	SecBuffer     InBuffers[2];
	InBufferDesc.ulVersion = SECBUFFER_VERSION;
	InBufferDesc.pBuffers = InBuffers;
	InBufferDesc.cBuffers = 2;

	SecBufferDesc OutBufferDesc;
	SecBuffer     OutBuffers[1];
	OutBufferDesc.ulVersion = SECBUFFER_VERSION;
	OutBufferDesc.pBuffers = OutBuffers;
	OutBufferDesc.cBuffers = 1;

	OutBuffers[0].BufferType = SECBUFFER_TOKEN;
	OutBuffers[0].pvBuffer   = NULL;
	OutBuffers[0].cbBuffer   = 0;
	SECURITY_STATUS sec_s = InitializeSecurityContext(&m_hUserCred, 0, const_cast<TCHAR*>(TargetName),
		m_ReqContextAttr, 0, 0, NULL, 0, &m_hContext, &OutBufferDesc, &m_ContextAttr, &m_ContextExpiry);
	//ASSERT(SEC_E_OK==sec_s||SEC_I_CONTINUE_NEEDED==sec_s);

	unsigned int SockDataSize=0;
	bool WasExtra=false;
	while(SEC_I_CONTINUE_NEEDED==sec_s){

		if(OutBuffers[0].pvBuffer&&OutBuffers[0].cbBuffer){
			//ASSERT(OutBuffers[0].pvBuffer);
			//ASSERT(OutBuffers[0].cbBuffer);

			SockByteStream->RawSend(OutBuffers[0].pvBuffer, OutBuffers[0].cbBuffer);
		
			FreeContextBuffer(OutBuffers[0].pvBuffer);
		}
	
		do{
			if(!WasExtra){
				SockDataSize+= SockByteStream->RawRecv(&SockDataBuf[SockDataSize], SockDataBuf.size()-SockDataSize);
			}
			else{
				WasExtra=false;
			}

			InBuffers[0].BufferType = SECBUFFER_TOKEN;
			InBuffers[0].pvBuffer   = &SockDataBuf[0];
			InBuffers[0].cbBuffer   = SockDataSize;
			InBuffers[1].BufferType = SECBUFFER_EMPTY;
			InBuffers[1].pvBuffer   = NULL;
			InBuffers[1].cbBuffer   = 0;

			sec_s = InitializeSecurityContext(&m_hUserCred, &m_hContext, const_cast<TCHAR*>(TargetName),
				m_ReqContextAttr, 0, 0, &InBufferDesc, 0, 0, &OutBufferDesc, &m_ContextAttr, &m_ContextExpiry);
		}while(SEC_E_INCOMPLETE_MESSAGE==sec_s);
	
		//ASSERT(SEC_E_OK==sec_s||SEC_I_CONTINUE_NEEDED==sec_s);

		if(SECBUFFER_EXTRA==InBuffers[1].BufferType){
			WasExtra=true;
			if(SEC_I_CONTINUE_NEEDED==sec_s){
				memmove(&SockDataBuf[0],
						((BYTE*)InBuffers[0].pvBuffer)+InBuffers[0].cbBuffer-InBuffers[1].cbBuffer,
				        InBuffers[1].cbBuffer);
				SockDataSize=InBuffers[1].cbBuffer;
			}
			else{
				m_HandshakeExtra.insert(m_HandshakeExtra.end(),
					((BYTE*)InBuffers[0].pvBuffer)+InBuffers[0].cbBuffer-InBuffers[1].cbBuffer,
					((BYTE*)InBuffers[0].pvBuffer)+InBuffers[0].cbBuffer);
				SockDataSize=0;
			}
		}
		else{
			SockDataSize=0;
		}
	}

}

void LSSLContext::Shutdown(LSockByteStream* SockByteStream, const TCHAR* TargetName)
{
	DWORD ShutdownToken = SCHANNEL_SHUTDOWN;
	SecBufferDesc ShutDownBufferDesc;
	SecBuffer     ShutDownBuffers[1];
	ShutDownBufferDesc.cBuffers = 1;
	ShutDownBufferDesc.pBuffers = ShutDownBuffers;
	ShutDownBufferDesc.ulVersion = SECBUFFER_VERSION;
	ShutDownBuffers[0].pvBuffer   = &ShutdownToken;
	ShutDownBuffers[0].BufferType = SECBUFFER_TOKEN;
	ShutDownBuffers[0].cbBuffer   = sizeof(ShutdownToken);

	ShutDownBuffers[0].BufferType = SECBUFFER_TOKEN;
	ShutDownBuffers[0].pvBuffer   = 0;
	ShutDownBuffers[0].cbBuffer   = 0;

	/*VERIFY(SEC_E_OK==*/InitializeSecurityContext(&m_hUserCred, GetContextHandle(), const_cast<TCHAR*>(TargetName),
		                                       m_ReqContextAttr, 0, 0, 0, 0, 0,
		                                       &ShutDownBufferDesc, &m_ContextAttr, &m_ContextExpiry)/*)*/;

	SockByteStream->RawSend(ShutDownBuffers[0].pvBuffer, ShutDownBuffers[0].cbBuffer);

	FreeContextBuffer(ShutDownBuffers[0].pvBuffer);
	ShutDownBuffers[0].pvBuffer=0;
	ShutDownBuffers[0].cbBuffer=0;
}
