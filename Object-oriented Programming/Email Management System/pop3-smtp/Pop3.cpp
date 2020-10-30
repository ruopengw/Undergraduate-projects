#include "Pop3.h"

template <class T> const T& max(const T& a, const T& b) {
	return (a < b) ? b : a;     // or: return comp(a,b)?b:a; for version (2)
}

LPop3::LPop3(LByteStream* BS)
	:m_BS(BS)
{

}

unsigned int StrToInt(const char* buf, int data_len)
{
	unsigned int num=0;

	for(;data_len>0;++buf,--data_len){
		num=num*10;
		switch(buf[0]){
			case '9':num+=9;break;
			case '8':num+=8;break;
			case '7':num+=7;break;
			case '6':num+=6;break;
			case '5':num+=5;break;
			case '4':num+=4;break;
			case '3':num+=3;break;
			case '2':num+=2;break;
			case '1':num+=1;break;
			case '0':break;
			default:
				throw LPop3::BAD_POP3_PROTOCOL();//only numbers expecting
		}
	}

	return num;
}

/*
inline bool CheckEndCRLF(const char* buf, int data_len)
{
	return data_len>2&&'\r'==buf[data_len-1]&&'\n'==buf[data_len];
}
*/

inline bool IsOKResponse(const char* buf, int data_len)
{
	//ASSERT(data_len>0);
	return ('+'==buf[0]);
}

inline int FindFirstCRLF(const char* buf, int start_offset, int data_len)
{
	for(int i = max(start_offset, 1); i<data_len; ++i){
		if('\r'==buf[i-1]&&'\n'==buf[i]){
			return i+1;
		}
	}
	return 0;
}

bool LPop3::ReadStatusLine(unsigned int* StatusLineLen, unsigned int* ReadedDataLen)
{
	unsigned int BuffOffset=0;
	unsigned int SecondLineOffset=0;
	do{
		unsigned int ret = m_BS->RawRecv(m_Response+BuffOffset, MAX_RESPONSE_SIZE-BuffOffset);
		BuffOffset+=ret;
	
		SecondLineOffset = FindFirstCRLF(m_Response, BuffOffset-ret, BuffOffset);
	}while(BuffOffset<MAX_RESPONSE_SIZE&&0==SecondLineOffset);

	if(0==SecondLineOffset){
		throw BAD_POP3_PROTOCOL();
	}

	m_Response[BuffOffset]='\0';

	if(StatusLineLen) *StatusLineLen = SecondLineOffset;
	if(ReadedDataLen) *ReadedDataLen = BuffOffset;
	return IsOKResponse(m_Response, SecondLineOffset);
}

bool LPop3::ReadLISTResponse(std::vector<unsigned int>* l)
{
	//ASSERT(l);
	unsigned int BuffOffset=0;
	unsigned int SecondLineOffset=0;
	if(!ReadStatusLine(&SecondLineOffset, &BuffOffset)){
		
		return false;
	}

	char NetBuf[NET_BUFFER_LENGTH];
	memcpy(&NetBuf[0], &m_Response[SecondLineOffset], BuffOffset-SecondLineOffset);
	int net_buf_data_len=BuffOffset-SecondLineOffset;
	m_Response[SecondLineOffset]='\0';

	int start=0;
	while(true){
		//S: +OK 2 messages (320 octets)
		//S: 1 120
		//S: 2 200
		//S: .
		int NextLineOffset = FindFirstCRLF(NetBuf, start, net_buf_data_len);
		if(0==NextLineOffset){
			memmove(&NetBuf[0], &NetBuf[start], net_buf_data_len-start);
			net_buf_data_len-=start;
			start=0;

			unsigned int ret = m_BS->RawRecv(&NetBuf[net_buf_data_len], NET_BUFFER_LENGTH-net_buf_data_len);
			net_buf_data_len+=ret;
		}
		else{

			if('.'==NetBuf[start]){

				return true;
			}
			else{
				for(;NetBuf[start]!=' ';++start);
				++start;
				l->push_back(StrToInt(&NetBuf[start], (NextLineOffset-2)-start));
				start=NextLineOffset;
			}
		}
	}
	return false;
}

bool LPop3::ReadUIDLResponse(std::vector<std::string>* l)
{
	//ASSERT(l);
	unsigned int BuffOffset=0;
	unsigned int SecondLineOffset=0;
	if(!ReadStatusLine(&SecondLineOffset, &BuffOffset)){
		return false;
	}

	char NetBuf[NET_BUFFER_LENGTH];
	memcpy(&NetBuf[0], &m_Response[SecondLineOffset], BuffOffset-SecondLineOffset);
	int net_buf_data_len=BuffOffset-SecondLineOffset;
	m_Response[SecondLineOffset]='\0';

	int start=0;

	while(true){
          //S: +OK
          //S: 1 whqtswO00WBw418f9t5JxYwZ
          //S: 2 QhdPYR:00WBw1Ph7x7
          //S: .
		int NextLineOffset = FindFirstCRLF(NetBuf, start, net_buf_data_len);
		if(0==NextLineOffset){
			memmove(&NetBuf[0], &NetBuf[start], net_buf_data_len-start);
			net_buf_data_len-=start;
			start=0;
    
			unsigned int ret = m_BS->RawRecv(&NetBuf[net_buf_data_len], NET_BUFFER_LENGTH-net_buf_data_len);
			net_buf_data_len+=ret;
		}
		else{

			if('.'==NetBuf[start]){

				return true;
			}
			else{
				for(;NetBuf[start]!=' ';++start);
				++start;
				l->push_back(std::string(&NetBuf[start], (NextLineOffset-2)-start));
				start=NextLineOffset;
			}
		}
	}
	return false;
}

bool LPop3::ReadRETRResponse(std::vector<char>* out_msg)
{
	unsigned int BuffOffset=0;
	unsigned int SecondLineOffset=0;
	if(!ReadStatusLine(&SecondLineOffset, &BuffOffset)){
		return false;
	}

	out_msg->clear();

	char NetBuf[NET_BUFFER_LENGTH];
	memcpy(&NetBuf[0], &m_Response[SecondLineOffset], BuffOffset-SecondLineOffset);
	int net_buf_data_len=BuffOffset-SecondLineOffset;
	m_Response[SecondLineOffset]='\0';

	int start=0;
	while(true){
		int NextLineOffset = FindFirstCRLF(NetBuf, start, net_buf_data_len);
		if(0==NextLineOffset){
			out_msg->insert(out_msg->end(), NetBuf, NetBuf+start);
			memmove(&NetBuf[0], &NetBuf[start], net_buf_data_len-start);
			net_buf_data_len-=start;
			start=0;
		
			unsigned int ret = m_BS->RawRecv(&NetBuf[net_buf_data_len], NET_BUFFER_LENGTH-net_buf_data_len);
			net_buf_data_len+=ret;
		}
		else{
			if(NextLineOffset-start==3&&'.'==NetBuf[start]&&'\r'==NetBuf[start+1]&&'\n'==NetBuf[start+2]){
				out_msg->insert(out_msg->end(), NetBuf, NetBuf+start);
				//ASSERT(NextLineOffset==net_buf_data_len);
				return true;
			}
			start=NextLineOffset;
		}
	}
	return false;
}


void LPop3::Greeting()
{
	/*VERIFY(*/ReadStatusLine()/*)*/;//RFC 1939: Once the TCP connection has been opened by a POP3 client, the POP3 server issues a one line greeting. This can be any positive response.
}

void LPop3::NOOP()
{
	const char cmdNOOP[]="NOOP\r\n";

	m_BS->RawSend(cmdNOOP, sizeof(cmdNOOP)-1);

	/*VERIFY(*/ReadStatusLine()/*)*/;//RFC 1939: Possible Responses: +OK
}

bool LPop3::USER(const std::string& user)
{
	std::string cmdUSER = "USER "+user+"\r\n";

	m_BS->RawSend(&cmdUSER[0], cmdUSER.length());

	return ReadStatusLine();
}

bool LPop3::PASS(const std::string& pass)
{
	std::string cmdPASS = "PASS "+pass+"\r\n";

	m_BS->RawSend(&cmdPASS[0], cmdPASS.length());

	return ReadStatusLine();
}

void LPop3::STAT(unsigned int* Count)
{
	const char cmdSTAT[]="STAT\r\n";
	m_BS->RawSend(cmdSTAT, sizeof(cmdSTAT)-1);

	*Count=0;
	if(ReadStatusLine()){
		//Possible Responses:
		//+OK nn mm
		int start=0;
		int end=0;
		for(;m_Response[start]!=' ';++start);
		++start;
		end=start;
		for(;m_Response[end]!=' ';++end);
		*Count=StrToInt(&m_Response[start], end-start);
	}
	//else ASSERT(false);//RFC 1939: Possible Responses: +OK
}


bool LPop3::LIST(std::vector<unsigned int>* l)
{
	const char cmdLIST[]="LIST\r\n";
	m_BS->RawSend(cmdLIST, sizeof(cmdLIST)-1);

	return ReadLISTResponse(l);
}

bool LPop3::UIDL(std::vector<std::string>* l)
{
	const char cmdLIST[]="UIDL\r\n";
	m_BS->RawSend(cmdLIST, sizeof(cmdLIST)-1);

	return ReadUIDLResponse(l);
}


bool LPop3::RETR(unsigned int msg_numb, std::vector<char>* out_msg)
{
	//ASSERT(msg_numb>0);
	char cmdRETR[30];
	size_t size = sprintf(cmdRETR, "RETR %d\r\n", msg_numb);
	//ASSERT(size<sizeof(cmdRETR));

	m_BS->RawSend(cmdRETR, size);

	return ReadRETRResponse(out_msg);
}

bool LPop3::TOP(unsigned int msg_numb, unsigned int lines, std::vector<char>* out_msg)
{
	//ASSERT(msg_numb>0);
	char cmdTOP[30];
	size_t size = sprintf(cmdTOP, "TOP %d %d\r\n", msg_numb, lines);
	//ASSERT(size<sizeof(cmdTOP));

	m_BS->RawSend(cmdTOP, size);

	return ReadRETRResponse(out_msg);
}

bool LPop3::DELE(unsigned int msg_numb)
{
	//ASSERT(msg_numb>0);
	char cmdDELE[30];
	size_t size = sprintf(cmdDELE, "DELE %d\r\n", msg_numb);
	//ASSERT(size<sizeof(cmdDELE));

	m_BS->RawSend(cmdDELE, size);

	return ReadStatusLine();
}

void LPop3::RSET()
{
	const char cmdRSET[]="RSET\r\n";

	m_BS->RawSend(cmdRSET, sizeof(cmdRSET)-1);

	/*VERIFY(*/ReadStatusLine()/*)*/;//RFC 1939: Possible Responses: +OK
}

void LPop3::QUIT()
{
	const char cmdQUIT[]="QUIT\r\n";

	m_BS->RawSend(cmdQUIT, sizeof(cmdQUIT)-1);

	/*VERIFY(*/ReadStatusLine()/*)*/;//RFC 1939: Possible Responses: +OK
}


