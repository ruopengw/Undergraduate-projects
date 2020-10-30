#define ssid		"SMART_SOCKET"
#define password	"12345678"

#define GROUP_NO  7	//请同学们改成自己的组号

#define pinLED13  8
#define pinLEDob  13

#define pinTemp A3
#define pinVol A2
#define pinCurrent0 A1
#define pinCurrent1 A0

#define MAX_FAN_CURRENT  473    	//unit = ma
#define MAX_LED_CURRENT  473		//unit = ma

#define __DEBUG

#ifdef __DEBUG
#define DBUG_PRINTLN(a) mySerial.println((a))
#define DBUG_PRINTLN2(a,b) mySerial.println((a),(b))
#define DBUG_PRINT(a) mySerial.print((a))
#define DBUG_PRINT2(a,b) mySerial.print((a),(b))
#define DBUG_WRITE(a) mySerial.write((a))
#else
#define DBUG_PRINTLN(a)  
#define DBUG_PRINTLN2(a,b) 
#define DBUG_PRINT(a) 
#define DBUG_PRINT2(a,b) 
#define DBUG_WRITE(a) 
#endif	


#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

// constants won't change. Used here to set a pin number :
const int relayPin9 = 9;      // the pin number of the relay pin
const int relayPin10 = 10;      // the pin number of the relay pin

const int pinLED8 = 8;

//loop中的循环计数值最大值，用于控制各种定时操作
//const int MAX_CIRCNT = 10000;
const int MAX_CIRCNT = 2000;
//采样间隔
//const int SAMPLE_RATE = 500;
const int SAMPLE_RATE = 1;
//时间检测间隔
//const int TDPROC_RATE = 5000;
const int TDPROC_RATE = 2;
//heartbeat 发送间隔
const int HBEAT_RATE = 5;
//最大允许的心跳计数差值
const int maxHBdiff = 5;

const char OK_str[] = "OK";
const char ERROR_str[] = "ERROR";
const char AT_CIPSTATUS_str[] = "AT+CIPSTATUS";
const char AT_CIPCLOSE_str[] = "AT+CIPCLOSE";
const char AT_CIPSTART_str[] = "AT+CIPSTART=";

//状态信息结构
typedef struct _STATUS_INFO {
  unsigned long flag;
  unsigned char st_sw1, st_sw2, alarm_sw1, alarm_sw2;
  int  cur_A1, cur_A2, cur_V, curT, curP, time;
} StatusInfo;


//定延时信息结构
typedef struct _TIMER_SETTING {
  unsigned long flag;
  unsigned int sw1_do, sw1_dc, sw1_to, sw1_tc;
  unsigned int sw2_do, sw2_dc, sw2_to, sw2_tc;
} TimerSetting;

//安全设置信息结构
typedef struct _SAFTY_SETTING {
  unsigned long flag;
  unsigned int maxA1, maxA2, minV, maxV, maxP1, maxP2, minT, maxT;
} SaftySetting;

//udp数据缓冲大小
const int udprcvBuffSize = 80;
//const int udpsndBuffSize = 64;
//udp数据缓冲区
char udprcvPakageBuffer[udprcvBuffSize];
//char udpsndPakageBuffer[udpsndBuffSize];
//缓冲区指针
int bufPointR = 0;
//int bufPointS=0;

//设备状态
int voltage, current1, current2, temperature, power, power1, power2;//mV,mA,mW,1/10摄氏度
bool state1, state2;
int last_cur1=0,last_cur2=0;

//设备名称
char name[16] = "smartSocket";

//loop中的循环计数值，用于控制各种定时操作
int circleCount = 0;

//安全设置
int max_current1 = 50;//单位10mA
int max_current2 = 50;//单位10mA
int max_power1 = 350;//单位10mW
int max_power2 = 350;
int max_voltage = 550;//单位10mV
int min_voltage = 400;
int max_temperature = 500;//单位0.1度
int min_temperature = -200;

//告警来源
int warningID1 = -1;
int warningID2 = -1;

//定时开关设置
typedef struct _TimeOpenClose {
  bool enabled;
  bool triged;
  int hour;
  int minute;
} TimeOpenClose;
//延时开关设置
typedef struct _DelayOpenClose {
  bool enabled;
  long lStartTick;
  int minutes;
} DelayOpenClose;

TimeOpenClose toInfo1, tcInfo1, toInfo2, tcInfo2;
DelayOpenClose doInfo1, dcInfo1, doInfo2, dcInfo2;

unsigned long CurrTime, PrevTime, LastTime;
int flipDiff;

char flipFast_cnt = 0;
char flipSlow_cnt = 0;

long lTicks = 0;

long delayForbitNotifyCnt = 0;

char fatal_err = 0;

unsigned long pwmc_lastTicks = 0;
unsigned long pwmc_csum1 = 0, pwmc_csum2 = 0;
int pwmc_scnt = 0;
int pwmcnt_num = 0;

bool pinLED13_st = false;
bool pinLEDob_st = false;

int heartbeatCnt = 0;
bool linked = false;

//time to open & time to close
void initTOC()
{
	toInfo1.enabled = false;
	toInfo1.hour = 4;
	toInfo1.minute = 5;
	tcInfo1.enabled = false;
	tcInfo1.hour = 6;
	tcInfo1.minute = 7;
	toInfo2.enabled = false;
	toInfo2.hour = 12;
	toInfo2.minute = 13;
	tcInfo2.enabled = false;
	tcInfo2.hour = 14;
	tcInfo2.minute = 15;
}

//delay to open & delay to close
void initDOC()
{
	doInfo1.enabled = false;
	doInfo1.lStartTick = 0;
	doInfo1.minutes = 1;

	dcInfo1.enabled = false;
	dcInfo1.lStartTick = 0;
	dcInfo1.minutes = 123;

	doInfo2.enabled = false;
	doInfo2.lStartTick = 0;
	doInfo2.minutes = 489;

	dcInfo2.enabled = false;
	dcInfo2.lStartTick = 0;
	dcInfo2.minutes = 611;
}

int getCurrent(int ch)
{
	if (ch == 0)
		return (analogRead(pinCurrent0) / 4);
	else if (ch == 1)
		return (analogRead(pinCurrent1) / 4);
	else
		return (0);
}


void getPwmCurrent_1()
{
  if ( millis() > pwmc_lastTicks+50 )
  {
    pwmc_lastTicks = millis();
    int c0 = getCurrent(0);
	delay(5);
    int c1 = getCurrent(1);
	if( (c0-last_cur1>20) || (last_cur1-c0>20) ||
		(c1-last_cur2>20) || (last_cur2-c1>20) )
	{
		DBUG_PRINT2(c0,DEC);
		DBUG_PRINT('~');
		DBUG_PRINT2(c1,DEC);
		DBUG_PRINTLN(',');
	}
	last_cur1 = c0;
	last_cur2 = c1;
      // DBUG_PRINT('.');
/*    if ( pwmc_csum1 + c0 < pwmc_csum1 || pwmc_csum1 + c0 >= 0xffffffff / MAX_LED_CURRENT )
    {
      DBUG_PRINT('x');
      DBUG_PRINT('1');
      DBUG_PRINTLN('x');
    }
    if ( pwmc_csum2 + c1 < pwmc_csum2 || pwmc_csum2 + c1 >= 0xffffffff / MAX_FAN_CURRENT )
    {
      DBUG_PRINT('x');
      DBUG_PRINT('2');
      DBUG_PRINTLN('x');
    }*/
    pwmc_csum1 += c0;
    pwmc_csum2 += c1;
    pwmc_scnt++;
    if ( pwmc_scnt > 8 )
    {
      // DBUG_PRINT(pwmc_csum1,DEC);
      // DBUG_PRINT('.');
      // DBUG_PRINTLN(pwmc_csum1/pwmc_scnt,DEC);
      // DBUG_PRINT(pwmc_csum2,DEC);
      // DBUG_PRINT('_');
      // DBUG_PRINTLN(pwmc_csum2/pwmc_scnt,DEC);
      current1 = pwmc_csum1 * MAX_LED_CURRENT / 256 / pwmc_scnt;
      current2 = pwmc_csum2 * MAX_FAN_CURRENT / 256 / pwmc_scnt;
      pwmc_scnt = 0;
      pwmc_csum1 = 0;
      pwmc_csum2 = 0;
	  pwmcnt_num++;
      //DBUG_PRINTLN2(current1,DEC);
    }
  }
}



unsigned char ChkSum(unsigned char* buf,int size){
	int i;
	long sum = 0;
	for(int i=0;i<size;i++)
		sum += buf[i];
	return (unsigned char)(sum%256);
}

bool mySerialFind(char* dest,int to_ms){
	unsigned long start = millis();
	char c;
	int dest_len = strlen(dest);
	int dest_ptr=0;
	while(millis()-start<to_ms){
		if(mySerial.available()>0 ){
			c = mySerial.read();
			if( c==dest[dest_ptr] ){
				dest_ptr++;
				if( dest_ptr>=dest_len )	return true;
			}
			else{
				dest_ptr = 0;
			}
		}
		getPwmCurrent_1();
	}
	return false;
}


bool SerialFind(char* dest,int to_ms){
	unsigned long start = millis();
	String data;
	char c;
	int i;
	int dest_len = strlen(dest);
	int dest_ptr=0;
	while(millis()-start<to_ms){
		if(Serial.available()>0 ){
			c = Serial.read();
			data = data+c;
			if( c==dest[dest_ptr] ){
				dest_ptr++;
				if( dest_ptr>=dest_len )	return true;
			}
			else{
				dest_ptr = 0;
			}
		}
		getPwmCurrent_1();
	}
	if( data.length()>0 ){
		DBUG_PRINT('[');
		DBUG_PRINT2(data.length(),DEC);
		DBUG_PRINT(':');
		for(i=0;i<data.length();i++){
			DBUG_WRITE((char)data[i]);
		}
		DBUG_PRINTLN(']');
	}
	return false;
}

int mySerialFind2(char* destok1,char* destok2,char* desterr1,char* desterr2,char* Info,int to_ms){
	unsigned long start = millis();
	char c;
	int destok1_len = strlen(destok1);
	int destok1_ptr=0;
	int destok2_len = strlen(destok2);
	int destok2_ptr=0;
	int desterr1_len = strlen(desterr1);
	int desterr1_ptr=0;
	int desterr2_len = strlen(desterr2);
	int desterr2_ptr=0;
	int ret = 0;
	bool ok1 = false;
	bool ok2 = false;
	bool err1 = false;
	bool err2 = false;
	while(millis()-start<to_ms){
		if(mySerial.available()>0 ){
			c = mySerial.read();
			if( c==destok1[destok1_ptr] ){
				destok1_ptr++;
				if( destok1_ptr>=destok1_len )	ok1 = true;
			}
			else{
				destok1_ptr = 0;
			}
			if( c==destok2[destok2_ptr] ){
				destok2_ptr++;
				if( destok2_ptr>=destok2_len )	ok2 = true;
			}
			else{
				destok2_ptr = 0;
			}
			if( c==desterr1[desterr1_ptr] ){
				desterr1_ptr++;
				if( desterr1_ptr>=desterr1_len )	err1 = true;
			}
			else{
				desterr1_ptr = 0;
			}
			if( c==desterr2[desterr2_ptr] ){
				desterr2_ptr++;
				if( desterr2_ptr>=desterr2_len )	err2 = true;
			}
			else{
				desterr2_ptr = 0;
			}
		}
		getPwmCurrent_1();
		if( ok1 || ok2 ){
			if( Info!=NULL ){
				DBUG_PRINT(OK_str);
				DBUG_PRINT(" for ");
				DBUG_PRINTLN(Info);
			}
			return 1;
		}
		if( err1 || err2 )	{
			if( Info!=NULL ){
				DBUG_PRINT("Fail");
				DBUG_PRINT(" for ");
				DBUG_PRINTLN(Info);
			}
			ret = -1;
			break;
		}
	}
	return ret;
}

int SerialFind2(char* destok1,char* destok2,char* desterr1,char* desterr2,char* Info,int to_ms){
	unsigned long start = millis();
	String data;
	char c;
	int i;
	int destok1_len = strlen(destok1);
	int destok1_ptr=0;
	int destok2_len = strlen(destok2);
	int destok2_ptr=0;
	int desterr1_len = strlen(desterr1);
	int desterr1_ptr=0;
	int desterr2_len = strlen(desterr2);
	int desterr2_ptr=0;
	int ret = 0;
	bool ok1 = false;
	bool ok2 = false;
	bool err1 = false;
	bool err2 = false;
	while(millis()-start<to_ms){
		if(Serial.available()>0 ){
			c = Serial.read();
			data = data+c;
			if( c==destok1[destok1_ptr] ){
				destok1_ptr++;
				if( destok1_ptr>=destok1_len )	ok1 = true;
			}
			else{
				destok1_ptr = 0;
			}
			if( c==destok2[destok2_ptr] ){
				destok2_ptr++;
				if( destok2_ptr>=destok2_len )	ok2 = true;
			}
			else{
				destok2_ptr = 0;
			}
			if( c==desterr1[desterr1_ptr] ){
				desterr1_ptr++;
				if( desterr1_ptr>=desterr1_len )	err1 = true;
			}
			else{
				desterr1_ptr = 0;
			}
			if( c==desterr2[desterr2_ptr] ){
				desterr2_ptr++;
				if( desterr2_ptr>=desterr2_len )	err2 = true;
			}
			else{
				desterr2_ptr = 0;
			}
		}
		getPwmCurrent_1();
		if( ok1 || ok2 ){
			if( Info!=NULL ){
				DBUG_PRINT(OK_str);
				DBUG_PRINT(" for ");
				DBUG_PRINTLN(Info);
			}
			return 1;
		}
		if( err1 || err2 )	{
			if( Info!=NULL ){
				DBUG_PRINT("Fail");
				DBUG_PRINT(" for ");
				DBUG_PRINTLN(Info);
			}
			ret = -1;
			break;
		}
	}
	if( data.length()>0 ){
		DBUG_PRINT('[');
		DBUG_PRINT2(data.length(),DEC);
		DBUG_PRINT(':');
		for(i=0;i<data.length();i++){
			DBUG_WRITE((char)data[i]);
		}
		DBUG_PRINTLN(']');
	}
	return ret;
}


//获取udp数据包
int getUdpDatagram_content(char* buf, int size)
{
	char strTmp[16];
	int nLen = 0;
	char cTmp;
	int i;
	Serial.setTimeout(300);
	nLen = Serial.parseInt();
	if (nLen > size)
	{
		digitalWrite(pinLED13, LOW);
		DBUG_PRINTLN("!get UDP1 x!");
		fatal_err = 1;
		return -1;//error!!!
	}
	cTmp = Serial.readBytesUntil(':', strTmp, 15);
	Serial.setTimeout(300);

	Serial.readBytes(buf, nLen);      //get data
	Serial.readBytesUntil('\x1', strTmp, 15);
	return nLen;
}

//**************************************************************
//通过udp发送缓冲区数据,返回发送成功与否
bool putUdpDatagram(char* buf, int size)
{
	int nTmp;
	char strTmp[16];
	int cnt = 0;
	int i;

	DBUG_PRINT("S");
	DBUG_PRINT2(size,DEC);

	Serial.print("AT+CIPSEND=");
	Serial.println(size+1, DEC);
	cnt++;
	if ( SerialFind2((char*)OK_str,(char*)OK_str,(char*)ERROR_str,"usy",NULL,200)==1 )
	{
		if( SerialFind2(">",">","ERROR","ERR",NULL,200)==1 )
		{
			Serial.write(buf, size);
			Serial.write(ChkSum((unsigned char*)buf,size));
			if( SerialFind2("SEND OK","SEND OK","ERROR","ERR",NULL,500)==1 )
			{
				DBUG_PRINT("S");
				return true;
			}
		}
	}
	
	DBUG_PRINT("!");
	return false;
}
//****************************************************************

//以下函数需要实现
int getVoltage()
{
	return (analogRead(pinVol) / 4);
}


int getTemperature()
{
	return (analogRead(pinTemp) / 4);
}


//获取udp数据到缓冲区中，udp数据包来源填写到hi中，返回得到的数据长度，没有返回0，失败返回-1
int getUdpDatagram(char* buf, int size)
{
	int nLen = 0,i;
	char cTmp;
	do {
		if( lTicks<4 ){
			DBUG_PRINT("Init");
			if( SerialFind((char*)ERROR_str,2000) ){
				DBUG_PRINTLN("!get UDP x!");	
				fatal_err = 1;
			}
			break;
		}
		if ( Serial.available() < 13 )	//wait +IPD,x,x[x]:xxxx\r\nOK
			break;
		if( !SerialFind("+IPD,",80) )
			break;
		DBUG_PRINT("R");
		//获取udp数据包并将链路号设置到hi->id
		nLen = getUdpDatagram_content(buf, size);
		DBUG_PRINT2(nLen,DEC);
		if( nLen>0 ){
			SerialFind("OK\r\n",50); //read to skip "OK\r\n"

			DBUG_PRINT("R");
			break;
		}
		DBUG_PRINT("!");
	} while (0);
	return nLen;
}

int strToInt(char *str)
{
	int ret = 0;
	int sig = 1;
	if ( str[0] == '-' )
	{
		sig = -1;
		str++;
	}
	while (*str != 0)
	{
		ret *= 10;
		if ( *str < '0' || *str > '9' )  return 0; //invalid
		ret += *str - '0';
		str++;
	}
	return ret * sig;
}

int intToStr(char* str, int size, int value)
{
	char *str1 = str;
	int size1 = size;
	char cTemp;
	if ( value < 0 )
	{
		*str = '-';
		str++;
		str1 = str;
		size--;
		size1--;
		value = -value;
	}
	if (value == 0 && size1 > 0)
	{
		*str1 = '0';
		*(str1 + 1) = 0;
		size1--;
		return size - size1;
	}
	while (value > 0 && size1 > 0)
	{
		*str1 = (value % 10) + '0';
		str1++;
		size1--;
		value /= 10;
	}
	if ( size1 == 0 )  str1--;
	*str1 = 0;
	str1--;
	//翻转
	while (str1 > str)
	{
		cTemp = *str;
		*str = *str1;
		*str1 = cTemp;
		str++;
		str1--;
	}
	return size - size1;
}

//处理udp数据缓冲
void udpDatagramProcess()
{
	char pkgBuf[48];
	_STATUS_INFO* si;
	_SAFTY_SETTING* ss;
	_TIMER_SETTING* ts;
	unsigned long flag = 0, ltime,ldate;
	int dst = 0;
	int value = 0;
	int hb_tmp = 0;
	if ( bufPointR < 4 )  return;
	DBUG_PRINT("P");
	DBUG_PRINT2(bufPointR,DEC);
	//DBUG_PRINTLN(udprcvPakageBuffer);
	flag = ((unsigned long*)udprcvPakageBuffer)[0];
	flag = (flag % 256) * 256 * 65536 + (flag % 65536) / 256 * 65536 + ((flag / 65536) % 256) * 256 + flag / (256 * 65536);
	//DBUG_PRINT2(flag,HEX);
	switch (flag)
	{
		case 0xFF0000FF://link
		case 0xFF0000EE://force link
			break;
		default:
			if ( !linked )	{
				bufPointR = 0;
				DBUG_PRINTLN("!");
				if( heartbeatCnt>0 )
					heartbeatCnt--;
				return;
			}
	}
  
	hb_tmp = heartbeatCnt;
	heartbeatCnt = 0;
	switch (flag) {
		case 0xFF0000EE://force link
		case 0xFF0000FF://link
			if ( !linked )
			{
				DBUG_PRINT('+');
				DBUG_PRINT('1');
				flag = 0xff0000ff;
				putUdpDatagram((char*)&flag, 4);
				linked = true;
			}
			else
			{
				flag = 0xff0000aa;
				putUdpDatagram((char*)&flag, 4);
				heartbeatCnt = hb_tmp;
			}
			break;
		case 0xFF000001://sync-1
			si = (_STATUS_INFO*)pkgBuf;
			si->flag = 0xff000001;
			si->st_sw1 = state1 ? 0xff : 0;
			si->st_sw2 = state2 ? 0xff : 0;
			si->alarm_sw1 = warningID1;
			si->alarm_sw2 = warningID2;
			si->cur_A1 = current1;
			si->cur_A2 = current2;
			si->cur_V = voltage;
			si->curT = temperature+1000;
			si->curP = power;
			strncpy(pkgBuf + sizeof(struct _STATUS_INFO), name, 16);
			putUdpDatagram(pkgBuf, strlen(name) + sizeof(struct _STATUS_INFO));
			if (  bufPointR >= 12 ) {
				ldate = ((unsigned long*)udprcvPakageBuffer)[1];
				ltime = ((unsigned long*)udprcvPakageBuffer)[2];
				lTicks = 2*((ltime & 0xff) + (((ltime >> 8) & 0xff) * 60) + (((ltime >> 16) & 0xff) * 3600));
			}	  
			break;
		case 0xFF000002://sync-2
			ss = (_SAFTY_SETTING*)pkgBuf;
			ss->flag = 0xff000002;
			ss->maxA1 = max_current1;
			ss->maxA2 = max_current2;
			ss->maxV = max_voltage;
			ss->minV = min_voltage;
			ss->maxP1 = max_power1;
			ss->maxP2 = max_power2;
			ss->maxT = max_temperature+1000;
			ss->minT = min_temperature+1000;
			putUdpDatagram(pkgBuf, sizeof(struct _SAFTY_SETTING));
			break;
		case 0xFF000003://sync-3
			ts = (_TIMER_SETTING*)pkgBuf;
			ts->flag = 0xff000003;
			ts->sw1_do = (doInfo1.minutes % 60) + doInfo1.minutes / 60 * 256 + (doInfo1.enabled ? 0x8000 : 0);
			ts->sw1_dc = (dcInfo1.minutes % 60) + dcInfo1.minutes / 60 * 256 + (dcInfo1.enabled ? 0x8000 : 0);
			ts->sw2_do = (doInfo2.minutes % 60) + doInfo2.minutes / 60 * 256 + (doInfo2.enabled ? 0x8000 : 0);
			ts->sw2_dc = (dcInfo2.minutes % 60) + dcInfo2.minutes / 60 * 256 + (dcInfo2.enabled ? 0x8000 : 0);
			ts->sw1_to = toInfo1.minute + toInfo1.hour * 256 + (toInfo1.enabled ? 0x8000 : 0);
			ts->sw1_tc = tcInfo1.minute + tcInfo1.hour * 256 + (tcInfo1.enabled ? 0x8000 : 0);
			ts->sw2_to = toInfo2.minute + toInfo2.hour * 256 + (toInfo2.enabled ? 0x8000 : 0);
			ts->sw2_tc = tcInfo2.minute + tcInfo2.hour * 256 + (tcInfo2.enabled ? 0x8000 : 0);
			putUdpDatagram(pkgBuf, sizeof(struct _TIMER_SETTING));
			break;
		case 0xFF000004://query
			if( bufPointR>4 )
			{
				state1 = (udprcvPakageBuffer[4]&0x0f)!=0;
				state2 = (udprcvPakageBuffer[4]&0xf0)!=0;
			}
			si = (_STATUS_INFO*)pkgBuf;
			si->flag = 0xff000004;
			si->st_sw1 = state1 ? 0xff : 0;
			si->st_sw2 = state2 ? 0xff : 0;
			si->alarm_sw1 = warningID1;
			si->alarm_sw2 = warningID2;
			si->cur_A1 = current1;
			si->cur_A2 = current2;
			si->cur_V = voltage;
			si->curT = temperature+1000;
			si->curP = power;
			si->time = lTicks/6;
			putUdpDatagram(pkgBuf, sizeof(struct _STATUS_INFO));
			break;
		case 0xFF000005://set name
			if ( bufPointR > 4 ) {
				memset(name, 0, 16);
				strncpy(name, udprcvPakageBuffer + 4, bufPointR - 4);
				flag = 0xff000005;
				putUdpDatagram((char*)&flag, 4);
				DBUG_PRINT('=');
				DBUG_PRINT(name);
				DBUG_PRINTLN('=');
			}
			break;
		case 0xFF000006://set safty setting
			if ( bufPointR >= sizeof(struct _SAFTY_SETTING) )
			{
				ss = (struct _SAFTY_SETTING*)udprcvPakageBuffer;
				max_current1 = ss->maxA1;
				max_current2 = ss->maxA2;
				max_voltage = ss->maxV;
				min_voltage = ss->minV;
				max_temperature = ss->maxT-1000;
				min_temperature = ss->minT-1000;
				max_power1 = ss->maxP1;
				max_power2 = ss->maxP2;
				flag = 0xff000006;
				putUdpDatagram((char*)&flag, 4);
			}
			break;
		case 0xFF000007://set timer setting
			if ( bufPointR >= sizeof(struct _TIMER_SETTING) )
			{
				ts = (struct _TIMER_SETTING*)udprcvPakageBuffer;
				doInfo1.enabled = ts->sw1_do > 0x7fff;
				doInfo1.lStartTick = lTicks;
				doInfo1.minutes = (ts->sw1_do & 0x7f00) / 256 * 60 + (ts->sw1_do % 256) % 60;
				doInfo2.enabled = ts->sw2_do > 0x7fff;
				doInfo2.lStartTick = lTicks;
				doInfo2.minutes = (ts->sw2_do & 0x7f00) / 256 * 60 + (ts->sw2_do % 256) % 60;
				dcInfo1.enabled = ts->sw1_dc > 0x7fff;
				dcInfo1.lStartTick = lTicks;
				dcInfo1.minutes = (ts->sw1_dc & 0x7f00) / 256 * 60 + (ts->sw1_dc % 256) % 60;
				dcInfo2.enabled = ts->sw2_dc > 0x7fff;
				dcInfo2.lStartTick = lTicks;
				dcInfo2.minutes = (ts->sw2_dc & 0x7f00) / 256 * 60 + (ts->sw2_dc % 256) % 60;

				toInfo1.enabled = ts->sw1_to > 0x7fff;
				toInfo1.minute = (ts->sw1_to % 256) % 60;
				toInfo1.hour = (ts->sw1_to & 0x7f00) / 256;
				toInfo2.enabled = ts->sw2_to > 0x7fff;
				toInfo2.minute = (ts->sw2_to % 256) % 60;
				toInfo2.hour = (ts->sw2_to & 0x7f00) / 256;
				tcInfo1.enabled = ts->sw1_tc > 0x7fff;
				tcInfo1.minute = (ts->sw1_tc % 256) % 60;
				tcInfo1.hour = (ts->sw1_tc & 0x7f00) / 256;
				tcInfo2.enabled = ts->sw2_tc > 0x7fff;
				tcInfo2.minute = (ts->sw2_tc % 256) % 60;
				tcInfo2.hour = (ts->sw2_tc & 0x7f00) / 256;
				flag = 0xff000007;
				putUdpDatagram((char*)&flag, 4);
			}
			break;
		case 0xFF000010://open sw1
			state1 = true;
			flag = 0xff000010;
			putUdpDatagram((char*)&flag, 4);
			break;
		case 0xFF000011://close sw1
			state1 = false;
			flag = 0xff000011;
			putUdpDatagram((char*)&flag, 4);
			break;
		case 0xFF000012://open sw2
			state2 = true;
			flag = 0xff000012;
			putUdpDatagram((char*)&flag, 4);
			break;
		case 0xFF000013://close sw2
			state2 = false;
			flag = 0xff000013;
			putUdpDatagram((char*)&flag, 4);
			break;
		case 0xFF000018://clear alarm1
			warningID1 = -1;
			flag = 0xff000018;
			putUdpDatagram((char*)&flag, 4);
			break;
		case 0xFF000019://clear alarm2
			warningID2 = -1;
			flag = 0xff000019;
			putUdpDatagram((char*)&flag, 4);
			break;
		default:
			heartbeatCnt = hb_tmp;
			break;
	}
	bufPointR = 0;
	DBUG_PRINT("P");
}

void flipLED13()
{
  if ( pinLED13_st )
    digitalWrite(pinLED13, LOW);
  else
    digitalWrite(pinLED13, HIGH);
  pinLED13_st = !pinLED13_st;
}

void flipLED13_1()
{
    digitalWrite(pinLED13, LOW);
	pinLED13_st = 0;
	delay(200);
	flipLED13();
	delay(200);
	flipLED13();
	delay(200);
	flipLED13();
	delay(200);
	flipLED13();
	delay(200);
}

void flipLED13_2()
{
    digitalWrite(pinLED13, LOW);
	pinLED13_st = 0;
	delay(800);
	flipLED13();
	delay(800);
	flipLED13();
	delay(800);
	flipLED13();
	delay(800);
	flipLED13();
	delay(800);	
}

void flipLED13_byDiff()
{
	if( flipFast_cnt>0 ){
		if( CurrTime-LastTime>flipDiff/2 ){
			flipLED13();
			flipFast_cnt--;
			LastTime = CurrTime;
		}		
	}
	else{
		if( flipSlow_cnt>0 ){
			if( CurrTime-LastTime>flipDiff*2 ){
				flipLED13();
				flipSlow_cnt--;
				LastTime = CurrTime;
			}		
		}
		else{
			if( CurrTime-LastTime>flipDiff ){
				flipLED13();
				LastTime = CurrTime;
			}
		}
	}
}

void flipLEDob()
{
  if ( pinLEDob_st )
    digitalWrite(pinLEDob, LOW);
  else
    digitalWrite(pinLEDob, HIGH);
  pinLEDob_st = !pinLEDob_st;
}

void LEDNotify(unsigned char flag){
	//digitalWrite(pinLED13, (flag&0x01)>0?HIGH:LOW);
	//delay(20);
	digitalWrite(relayPin9, (flag&0x02)>0?HIGH:LOW);
	delay(20);
	digitalWrite(relayPin10, (flag&0x04)>0?HIGH:LOW);
}

bool removeHostChain()
{
	int i;
	int result;
	long start = millis();
	DBUG_PRINT("<rHC:");
	while(millis()-start<3000){
		Serial.println(AT_CIPCLOSE_str);
		result = SerialFind2((char*)OK_str,"UNLINK","ER@!","ER@!",NULL,900);
		if ( result==1 )
		{
			DBUG_PRINT(OK_str);
			DBUG_PRINTLN(" /rHC>");
			return true;
		}
	}
	DBUG_PRINT("!rHC x!");
	DBUG_PRINTLN(" /rHC>");
	//fatal_err = 1;  
	return false;
}

void createHostChain()
{
	do {
		delay(50);
		Serial.flush();
		Serial.print(AT_CIPSTART_str); //open udp #0
		Serial.print('"');
		Serial.print("UDP");
		Serial.print('"');
		Serial.print(",");
		Serial.print('"');
		Serial.print("255.255.255.255");
		Serial.print('"');
		Serial.println(",7761,7762,2");

		if( SerialFind2((char*)OK_str,(char*)OK_str,(char*)ERROR_str,"busy","UDP 0 listen!",3000)==1 )
			break;
		removeHostChain();
	} while (1);
}

void Init8266()
{
	long cnt = 0;
	boolean result;
	//bool rstReady = false;
	//bool skip9600 = false;
	LEDNotify(0);
	Serial.flush();
	Serial.print("+++");  
	delay(50);
	Serial.print("+++");  
	delay(50);
	Serial.print("+++");  
	delay(50);
	do {//set into station mode
		delay(50);
		//Serial.flush();
		Serial.println("AT+CWMODE=1");  
		if( SerialFind2((char*)OK_str,"no change",(char*)ERROR_str,"busy","station mode!",2000)==1 )
			break;
		flipLED13();
	}while(1);

	while(1){
		//Serial.flush();
		DBUG_PRINTLN("try RST..");
		Serial.println("AT+RST");
		if( SerialFind((char*)OK_str,2000) ){
			//rstReady = true;
			DBUG_PRINT("8266 ready on ");
			DBUG_PRINTLN("115200...");
			break;
		}
		flipLED13();
	}
	
	LEDNotify(2);
	//print 8266 startup info
	DBUG_PRINTLN('.');
	DBUG_PRINTLN("..8266_info..");

	cnt = 0;
	while (Serial.available() <= 128 && cnt < 3000)
	{
		delay(1);
		while (Serial.available() > 0)
		{
			char c = (char)Serial.read();
			DBUG_WRITE(c);
			flipLED13();
		}
		cnt++;
	}
  	DBUG_PRINTLN('.');

	LEDNotify(4);

	do {//set into station mode
		delay(50);
		//Serial.flush();
		Serial.println("AT+CWMODE=1");  
		if( SerialFind2((char*)OK_str,"no change",(char*)ERROR_str,"busy","station mode!",2000)==1 )
			break;
		flipLED13();
	}while(1);

	//Serial.flush();
	do//attach to the wifi
	{
		DBUG_PRINT("connect to wifi:");
		DBUG_PRINT(ssid);
		DBUG_PRINT(" - ");
		DBUG_PRINTLN(password);
		delay(50);
		Serial.print("AT+CWJAP=");  //attach to the wifi
		Serial.print("\"");     //"ssid"
		Serial.print(ssid);
		Serial.print("\"");

		Serial.print(",");

		Serial.print("\"");      //"pwd"
		Serial.print(password);
		Serial.println("\"");
		flipLED13();

		char strMyIP[16];
		int nLen;

		unsigned long start = millis();
		while(millis()-start<2000)
		{
			if( Serial.available()>0 )
				Serial.read();			
		}
		while(millis()-start<10000)
		{
			Serial.println("AT+CIFSR"); //get IP address
			result = SerialFind("TAIP,",500);
			if (result)
			{
				break;
			}		
			DBUG_PRINTLN("failure for IP Address!");
		}

		int i;
		unsigned int ucsum=0;
		for (i = 0; i < 16; i++)
			strMyIP[i] = '\0';

		Serial.setTimeout(200);
		Serial.readBytesUntil('"', strMyIP, 15); //read to skip the prev '"'
		Serial.setTimeout(200);
		Serial.readBytesUntil('"', strMyIP, 15); //read the IP addr
		i = 0;
		while (i < 16 && strMyIP[i] != 0)
		{
			DBUG_PRINT(strMyIP[i]);
			i++;
		}
		DBUG_PRINTLN('.');
		if( SerialFind("AMAC,",200) ){
			Serial.setTimeout(200);
			Serial.readBytesUntil('"', strMyIP, 15); //read to skip the prev '"'
			Serial.setTimeout(200);
			Serial.readBytesUntil('"', strMyIP, 15); //read the MAC addr
			for(i=0;i<17;i++)
				ucsum += strMyIP[i];
			String strName = String("SMTSKT-")+String(ucsum,HEX);
			strName.toCharArray(name,16);
			name[15] = 0;
			DBUG_PRINT("NAME:");
			DBUG_PRINTLN(name);
		}
		else{
			DBUG_PRINTLN("failure for MAC Address!");
		}
		break;
	}
	while(1);
  
	LEDNotify(6);
	Serial.flush();
  
	// 设置 单连接
	do{
		delay(50);
		Serial.println("AT+CIPMUX=0");
		if( SerialFind2((char*)OK_str,(char*)OK_str,(char*)ERROR_str,"busy","Mux mode!",2000)==1 )
			break;
		flipLED13();
	} while (1);
	LEDNotify(2);

	//建立UDP连接，准备广播至7762端口。
	createHostChain();
	LEDNotify(0);

	delay(100);
	Serial.flush();
	Serial.setTimeout(300);
}


void delayTimeControl()
{
  //延时和定时控制需要结合实时钟
  if (toInfo1.enabled && (lTicks/60 == (toInfo1.minute +toInfo1.hour * 60) * 2) )
  {
    //toInfo1.enabled = false;
    state1 = true;
    DBUG_PRINTLN("to1");
  }
  if (tcInfo1.enabled && (lTicks/60 == (tcInfo1.minute +tcInfo1.hour * 60) * 2) )
  {
    //tcInfo1.enabled = false;
    state1 = false;
    DBUG_PRINTLN("tc1");
  }

  if (toInfo2.enabled && (lTicks/60 == (toInfo2.minute +toInfo2.hour * 60) * 2) )
  {
    //toInfo2.enabled = false;
    state2 = true;
    DBUG_PRINTLN("to2");
  }
  if (tcInfo2.enabled && (lTicks/60 == (tcInfo2.minute +tcInfo2.hour * 60) * 2) )
  {
    //tcInfo2.enabled = false;
    state2 = false;
    DBUG_PRINTLN("tc2");
  }

  
  if (doInfo1.enabled && ((lTicks - doInfo1.lStartTick)/60 >= doInfo1.minutes * 2) )
  {
    doInfo1.enabled = false;
    state1 = true;
    DBUG_PRINTLN("do1");
  }
  if (dcInfo1.enabled && ((lTicks - dcInfo1.lStartTick)/60 >= dcInfo1.minutes * 2) )
  {
    dcInfo1.enabled = false;
    state1 = false;
    DBUG_PRINTLN("dc1");
  }

  if (doInfo2.enabled && ((lTicks - doInfo2.lStartTick)/60 >= doInfo2.minutes * 2) )
  {
    doInfo2.enabled = false;
    state2 = true;
    DBUG_PRINTLN("do2");
  }
  if (dcInfo2.enabled && ((lTicks - dcInfo2.lStartTick)/60 >= dcInfo2.minutes * 2) )
  {
    dcInfo2.enabled = false;
    state2 = false;
    DBUG_PRINTLN("dc2");
  }
}

void safetyCheck()
{
  //安全检测检测失败立刻断开电源!!!!!!!!!!!!!!
  do{
	  if ((voltage > max_voltage*10 || voltage < min_voltage*10) && (state1 || state2) )
	  {
		state1 = state2 = false;
		warningID1 = 0;
		warningID2 = 0;
		DBUG_PRINT('V');
		DBUG_PRINTLN2(voltage, DEC);
		break;;
		//notifyState();
	  }
	  if ( (state1 || state2 ) && (temperature > max_temperature || temperature < min_temperature ) )
	  {
		state1 = state2 = false;
		warningID1 = 3;
		warningID2 = 3;
		DBUG_PRINT('T');
		DBUG_PRINTLN2(temperature, DEC);
		break;;
		//notifyState();
	  }
	  if ( state1 && (current1 > max_current1*10 || power1 > max_power1*10) )
	  {
		state1 = false;
		warningID1 = ((current1 > max_current1*10 ? 1 : 0) + (power1 > max_power1*10 ? 10 : 0));
		DBUG_PRINT('C');
		DBUG_PRINT('a');
		DBUG_PRINTLN2(current1, DEC);
		//notifyState();
	  }
	  if ( state2 && (current2 > max_current2*10 || power2 > max_power2*10) )
	  {
		state2 = false;
		warningID2 = ((current2 > max_current2*10 ? 2 : 0) + (power2 > max_power2*10 ? 20 : 0));
		DBUG_PRINT('C');
		DBUG_PRINT('b');
		DBUG_PRINTLN2(current2, DEC);
		//notifyState();
	  }
  }while(0);
	if (state1)
	  digitalWrite(relayPin9, HIGH);
	else
	  digitalWrite(relayPin9, LOW);
	if (state2)
	  digitalWrite(relayPin10, HIGH);
	else
	  digitalWrite(relayPin10, LOW);
}

bool tryDebugLink(char* sIP,int iPort)
{
	Serial.print("tryDebugLink..");
	Serial.print(sIP);
	Serial.print(",");
	Serial.println(iPort,DEC);
	mySerial.println();
    mySerial.print(AT_CIPSTART_str); //open tcp
    mySerial.print('"');
    mySerial.print("TCP");
    mySerial.print('"');
    mySerial.print(",");
    mySerial.print('"');
    mySerial.print(sIP);
    mySerial.print('"');
    mySerial.print(",");
	mySerial.println(iPort,DEC);
	
	//if( mySerialFind2("CONNECT",(char*)OK_str,(char*)ERROR_str,"ERR",NULL,5000)==1 )
	//if( mySerialFind("CONNECT",500)==1 )
	{
		delay(100);
		mySerial.println();
		//while(1){
			mySerial.println("AT+CIPMODE=1");
			if ( mySerialFind((char*)OK_str,2000)==1 )
				//break;
				flipLED13();
		//}
		//while(1){
			mySerial.println("AT+CIPSEND");
			if ( mySerialFind(">",2000)==1 )
				//break;
				flipLED13();
		//}
	}
}


void setup()
{
	bool debug_wifi_existed = false;
	// put your setup code here, to run once:
	//.....
	pinMode(pinLED13, OUTPUT);
	digitalWrite(pinLED13, LOW); //turn off led13

	// set the digital pin as output:
	pinMode( relayPin9, OUTPUT );
	pinMode( relayPin10, OUTPUT );
	digitalWrite(relayPin9, LOW);
	digitalWrite(relayPin10, LOW);

	pinMode(pinLED8, OUTPUT);
	digitalWrite(pinLED8, LOW); //turn off led8

	Serial.begin(115200);
	while (!Serial);
	mySerial.begin(9600);
	delay(100);
	DBUG_PRINTLN("Debug UART ready...");
	//delay(100);
	//退出透传模式
	//mySerial.print("+++");
	//delay(100);
	//测试是否存在debug用的WIFI模块
	mySerial.println();
	mySerial.println("AT+CWMODE=3");
	if ( mySerialFind((char*)OK_str,200)==1 )
	{
		flipLED13_1();
		debug_wifi_existed = true;
	}
	LEDNotify(2);	
	//复位
	while(debug_wifi_existed){
		mySerial.println("AT+RST");
		if ( mySerialFind((char*)OK_str,2000)==1 )
			break;
		flipLED13();
	}
	LEDNotify(4);
	//设置单路连接
	while(debug_wifi_existed){
		mySerial.println("AT+CIPMUX=0");
		if ( mySerialFind((char*)OK_str,300)==1 )
			break;
		flipLED13();
	}	
	LEDNotify(7);

	PrevTime = millis();
	while (millis() - PrevTime < 5000)
	{
		while (Serial.available() > 0)
		{
			char c = (char)Serial.read();
			DBUG_PRINT(c);
		}
	}

	if(debug_wifi_existed){
		LEDNotify(0);
		while(1)
		{
			flipLED13_1();
			//连接WIFI
			mySerial.println();
			mySerial.print("AT+CWJAP=");  //attach to the wifi
			mySerial.print("\"");     //"ssid"
			mySerial.print(ssid);
			mySerial.print("\"");

			mySerial.print(",");

			mySerial.print("\"");      //"pwd"
			mySerial.print(password);
			mySerial.println("\"");

			//delay(500);
			if( mySerialFind("OK",8000) )
			{
				LEDNotify(2);
				//flipLED13_2();
				mySerial.println("AT+CIFSR"); //get IP address
				if( mySerialFind("IP,",3000) )
				{
					LEDNotify(4);
					break;
				}			
			}			
		}
		//设置单路连接
		while(1){
			mySerial.println("AT+CIPMUX=0");
			if ( mySerialFind((char*)OK_str,300) )
				break;
			flipLED13();
		}	
		
//		delay(1000);
		while(1){
			flipLED13_1();
			//创建UDP广播连接
			mySerial.println();
			mySerial.print(AT_CIPSTART_str); //open udp #0
			mySerial.print('"');
			mySerial.print("UDP");
			mySerial.print('"');
			mySerial.print(",");
			mySerial.print('"');
			mySerial.print("255.255.255.255");
			mySerial.print('"');
			mySerial.println(",8871,8862,2");
			delay(500);
			//if( mySerialFind("OK",800) )
			{
				LEDNotify(7);
				break;
			}			
		}
		
		char strTmp[16];
		for(int i=0;i<12;i++){
			flipLED13_1();
			//发送广播包
			mySerial.print("AT+CIPSEND=");
			mySerial.println(4, DEC);
			mySerial.setTimeout(1000);
			mySerial.readBytesUntil('>', strTmp, 15);
			mySerial.read();
			mySerial.setTimeout(300);
			
			unsigned long flag12;
			flag12 = 0xFFAADDEE;
			mySerial.write((unsigned char*)&flag12, 4);
			
			//测试是否有回应
			delay(100);
			//while(1)
			{
				if( mySerialFind("+IPD",1000) )
				{
					delay(50);
					if( mySerialFind("DEBUG_SOCKET",1000) )
					{
						//获取回包发送者信息
						do{
							flipLED13();
							mySerial.println(AT_CIPSTATUS_str);
							delay(300);
							if( !mySerialFind("STATUS:",500) ) //got the reply
								continue;
							if( !mySerialFind("+CIPSTATUS:",500) )
								continue;
							flipLED13_2();
							mySerial.setTimeout(500);
							mySerial.parseInt();
							int pos;
							pos = intToStr(strTmp, 16, mySerial.parseInt());
							strTmp[pos] = '.';
							pos++;
							strTmp[pos] = 0;
							Serial.println(strTmp);
							pos += intToStr(strTmp + pos, 16 - pos, mySerial.parseInt());
							strTmp[pos] = '.';
							pos++;
							strTmp[pos] = 0;
							Serial.println(strTmp);
							pos += intToStr(strTmp + pos, 16 - pos, mySerial.parseInt());
							strTmp[pos] = '.';
							pos++;
							strTmp[pos] = 0;
							Serial.println(strTmp);
							pos += intToStr(strTmp + pos, 16 - pos, mySerial.parseInt());
							strTmp[pos] = 0;
							Serial.println(strTmp);
							break;
						}while(1);
						break;
					}
					else
						strTmp[0] = 0;
				}							
			}
		}
		LEDNotify(4);
		
		//移除广播连接
		while(1){
			mySerial.println(AT_CIPCLOSE_str);
			if ( mySerialFind((char*)OK_str,300) )
				break;
			flipLED13();
		}	
		if( strTmp[0]!=0 )
		{
			flipLED13();
			tryDebugLink(strTmp,8080);				
		}
		else{
			flipLED13();
			tryDebugLink("192.168.8.251",8080);	
			flipLED13();
			tryDebugLink("192.168.9.251",8080);
			flipLED13();
			tryDebugLink("192.168.9.88",8080);
			flipLED13();
			tryDebugLink("192.168.8.88",8080);
			flipLED13();
			tryDebugLink("192.168.199.102",8080);
		}
	}
	

	LEDNotify(0);
	Init8266();
	

	initTOC();
	initDOC();

	PrevTime = millis()/500;

	LEDNotify(0);
	mySerial.println("AT+CIPMODE=1");
	delay(200);
	LEDNotify(1);
	mySerial.println("AT+CIPSEND");
	delay(200);
	LEDNotify(2);
	delay(200);
	mySerial.println("AT+CIPMODE=1");
	LEDNotify(4);
	delay(200);
	mySerial.println("AT+CIPSEND");
	LEDNotify(0);
	delay(200);
	LEDNotify(4);
	mySerial.println("AT+CIPMODE=1");
	delay(200);
	LEDNotify(2);
	delay(200);
	mySerial.println("AT+CIPSEND");
	LEDNotify(1);
	delay(200);
	LEDNotify(0);
	mySerial.println("AT+CIPSEND");

	LEDNotify(6);
	pinLED13_st = 0;
	state1 = state2 = true;

	lTicks = 0;

	LastTime = millis();
	flipDiff = 100;

}

void loop()
{
	// put your main code here, to run repeatedly:
	int i;
	int udp_rcvSize = 0;
	unsigned long flag;
	if ( fatal_err )
	{
		DBUG_PRINTLN("re-setup...");
		setup();
		fatal_err = 0;
	}

	getPwmCurrent_1();

	CurrTime = millis();
	//flipLED13_byDiff();

	CurrTime /= 500;

	if (CurrTime > PrevTime )
	{
		DBUG_PRINTLN("}");
		DBUG_PRINTLN("...");
		DBUG_PRINT("{");
		//lTicks = millis()/500;
		lTicks+=(CurrTime - PrevTime);
		if (lTicks >= (long)24 * 60 * 60 * 2)
			lTicks = 0;

		circleCount++;
		PrevTime = CurrTime;

		if ( circleCount > MAX_CIRCNT )
			circleCount = 0;

		DBUG_PRINT("~");
		//获取udp数据包到接收缓冲区
		udp_rcvSize = getUdpDatagram(udprcvPakageBuffer + bufPointR, udprcvBuffSize - bufPointR);
		if (udp_rcvSize != 0)
		{
			flipLEDob();
			//DBUG_PRINT('R');
			//DBUG_PRINTLN2(udp_rcvSize, DEC);
			digitalWrite(pinLED13, HIGH);
		}
		if (udp_rcvSize < 0)
		{
			//出错处理
			digitalWrite(pinLED13, LOW);
			DBUG_PRINTLN("!get UDP3 x!");
			fatal_err = 1;
			//while(1); //deadlock here!!!
		}
		else
		{
			bufPointR += udp_rcvSize;
		}
		udprcvPakageBuffer[bufPointR] = 0;
		DBUG_PRINT("~");

		//处理接收数据
		if( bufPointR>0 )
		{
			while ( bufPointR > 0 )
			{
				udpDatagramProcess();
				bufPointR = 0;
			}
			digitalWrite(pinLED13, LOW);
		}
		DBUG_PRINT("~");

		if ( circleCount % SAMPLE_RATE == 0 )
		{
			voltage = (long)getVoltage() * 39.0625; //标定5000mv为128
			temperature = (getTemperature() * 114) / 64; //单位0.1度
			power1 = (long)(voltage/10) * (long)(current1/10) / 10;//单位mW
			power2 = (long)(voltage/10) * (long)(current2/10) / 10;//单位mW
			power = power1 + power2;

			safetyCheck();
		}
		if ( circleCount % TDPROC_RATE == 0 )
		{
			//延时和定时控制
			delayTimeControl();

			//delievery data to each app
			//notifyState();
		}
		DBUG_PRINT("~");

		//心跳变量处理
		if ( circleCount % HBEAT_RATE == 0 )
		{
			flipFast_cnt = 3;
			DBUG_PRINT('-');
			if ( linked )
			{
				flipDiff = 160;
				heartbeatCnt++;
				if ( heartbeatCnt > maxHBdiff )
				{
					//移除主机信息
					removeHostChain();
					DBUG_PRINT('x');
					createHostChain();
					linked = false;
					heartbeatCnt = 0;
				}
			}
			else{
				flipDiff = 80;  
				heartbeatCnt++;
				DBUG_PRINT('.');
				//if( heartbeatCnt%5==0 ){
					flag = 0xFFAA00FF|((GROUP_NO&0xff)<<8);
					putUdpDatagram((char*)&flag, 4);
				//}
				if ( heartbeatCnt > maxHBdiff*5 )
				{
					//移除主机信息
					removeHostChain();
					DBUG_PRINT('x');
					createHostChain();
					heartbeatCnt = 0;
				}
			}
			DBUG_PRINT2(heartbeatCnt,DEC);
			DBUG_PRINT('-');
		}

		if (state1)
			digitalWrite(relayPin9, HIGH);
		else
			digitalWrite(relayPin9, LOW);
		if (state2)
			digitalWrite(relayPin10, HIGH);
		else
			digitalWrite(relayPin10, LOW);

		DBUG_PRINT(";");
	}
}
















