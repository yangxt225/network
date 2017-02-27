#include "libnetwork.h"  
#include <string.h>
#include <stdio.h>
  
int main(int argc, char *argv[])  
{   
    char msg[] = "admin";
	char recvBuf[128] = {0};
	memset(recvBuf, 0, sizeof(recvBuf));
	char ip[] = "127.0.0.1";
	int port = 10210;
	int retval, len=0;
	retval = libnetwork_createSockConn(ip, port);
	printf("[libnetwork_createSockConn]: ip=%s, port=%d \n", ip, port);
	if(retval == 0)
	{
		libnetwork_send_package(msg, strlen((char*)msg));
		printf("[libnetwork_send_package]: sendBuf: %s \n", msg);
		libnetwork_recv_package(recvBuf, &len);
		printf("[libnetwork_recv_package]: recvBuf: %s , len: %d \n", recvBuf, len);
	}
	
	libnetwork_closeSockConn();
	printf("[libnetwork_closeSockConn]: close connect. \n");
    return 0;  
}  
