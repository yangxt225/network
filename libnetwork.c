#include "libnetwork.h"
#include <linux/sockios.h>
#include <linux/ethtool.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <string.h>

/*
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdlib.h>
*/

static int sock_fd = -1;

//��ȡ��������״̬
int libnetwork_getNetlinkStat(int chkroot, const char *if_name)
{
    int skfd;
    struct ifreq ifr;
    struct ethtool_value edata;
	
	if(chkroot && (getuid() != 0))
	{
		// ��ҪrootȨ�� Netlink Status Check Need Root Power.
		return -1;
	}
	
    edata.cmd = ETHTOOL_GLINK;
    edata.data = 0;
	
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, if_name, sizeof(ifr.ifr_name) - 1);
    ifr.ifr_data = (char *) &edata;
	
    if(( skfd = socket( AF_INET, SOCK_DGRAM, 0 )) == 0)
        return -2;
    if(ioctl( skfd, SIOCETHTOOL, &ifr ) == -1)
    {
        close(skfd);
        return -2;
    }
    close(skfd);
    return edata.data;
}

/**
* FunctionName�� libnetwork_createSockConn
*/
int libnetwork_createSockConn(char *ip, int port)
{
	int retval = LIBNETWORK_ERR_NONE, retry=3;
	struct sockaddr_in s_addr;
	
	do{
		retval = libnetwork_getNetlinkStat(1, "eth0");
		if (retval == 0)
		{
			// link down....
			sleep(5);
			retval = LIBNETWORK_ERR_LINK;
			break;
		}
		
		// ����socket
		if (sock_fd == -1)
		{
			if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			{
				// error
				retval = LIBNETWORK_ERR_OPENSOCK;
				break;
			}
		}
		
		// ip port
		memset(&s_addr, 0, sizeof(struct sockaddr_in));
		s_addr.sin_family = AF_INET;
		s_addr.sin_addr.s_addr = inet_addr(ip);
		s_addr.sin_port = htons(port); 
		
		// �ͻ��˽���tcp����
		do
		{
			if(connect(sock_fd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr)) == -1) 
			{
				// error 
				retval = LIBNETWORK_ERR_CONNECT;
				// ��������
				continue;
			}
			else{
				retval = LIBNETWORK_ERR_NONE;
				break;
			}
		}while(retry--);
		
	}while(0);
	
	return retval;
}

/**
* FunctionName�� libnetwork_closeSockConn
*/
void libnetwork_closeSockConn(void)
{
	// �ر�socket
	if(sock_fd != -1)
		close(sock_fd);	
	sock_fd = -1;
}

/**
* FunctionName��libnetwork_send_package
*/
int libnetwork_send_package(const char *sendBuf, const int sendLen)
{
	int retval = LIBNETWORK_ERR_NONE, ret;
	ret = send(sock_fd, sendBuf, sendLen, 0);
	if(ret == -1)
		retval = LIBNETWORK_ERR_SEND;
	
	return retval;
}

/**
* FunctionName��libnetwork_recv_package
*/
int libnetwork_recv_package(char *recvBuf, int *recvLen)
{
	int retval = LIBNETWORK_ERR_NONE, ret;
	struct timeval tv;
	fd_set readfds;
	// ��������
	while(1)
	{
		// ������ ��ȡ����
		FD_ZERO(&readfds);
		FD_SET(sock_fd, &readfds);
		tv.tv_sec = NETWK_COMM_TIMEOUT;
		tv.tv_usec = 0;
		ret = select(sock_fd+1, &readfds, NULL, NULL, &tv);
		if(ret == -1)
		{
			// socket select() Error
			retval = LIBNETWORK_ERR_SELECT;
			break;
		}
		if (ret == 0){
			// no Data within NETWK_COMM_TIMEOUT
			retval = LIBNETWORK_ERR_TIMEOUT;
			continue;
		}
		
		//���sock_fd�Ƿ��������������
		if(FD_ISSET(sock_fd, &readfds))
		{
			ret = recv(sock_fd, recvBuf, 1024, 0);
			if ((ret == -1) || (ret == 0))
				// ���ݽ��մ���
				retval = LIBNETWORK_ERR_RECV;
			else
				*recvLen = ret;
			
			// �˳�whileѭ��
			break;
		}		
	}
	
	return retval;
}




