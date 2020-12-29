#include "WSockHelpers.h"

u_long GetBinAddr(const char* Host)
{
	u_long na = inet_addr(Host);
	if(INADDR_NONE==na){
		const HOSTENT* shost = gethostbyname(Host);
		if(shost){
			na = ((unsigned long **)shost->h_addr_list)[0][0];
		}
		else{
			na = INADDR_NONE;
		}
	}
	return na;
}

