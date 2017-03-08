#include "libnetwork.h"

//////////////////////////////////////
/*
	//#include	<sys/types.h>
	//#include	<linux/types.h>
	//#include	<stdint.h>
	//#include	<sys/socket.h>
	//#include	<netinet/in.h>
	//#include	<arpa/inet.h>
#include	<netdb.h>
	//#include	<sys/io.h>
	//#include	<sys/stat.h>
	//#include	<sys/time.h>
	//#include	<sys/ioctl.h>
#include	<net/if.h>
	//#include	<pthread.h>
#include	<string.h>
	//#include	<stdlib.h>
	//#include	<stdio.h>
	//#include	<unistd.h>
	//#include	<syslog.h>
	//#include	<fcntl.h>
	//#include	<time.h>
	//#include	<errno.h>
	//#include 	<math.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>
*/

#include	<netdb.h>
#include	<net/if.h>
#include	<string.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>


static int sock_fd = -1;

//获取网卡链接状态
int libnetwork_getNetlinkStat(int chkroot, const char *if_name)
{
    int skfd;
    struct ifreq ifr;
    struct ethtool_value edata;
	
	if(chkroot && (getuid() != 0))
	{
		// 需要root权限 Netlink Status Check Need Root Power.
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
* FunctionName： libnetwork_createSockConn
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
		
		// 创建socket
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
		
		// 客户端建立tcp连接
		do
		{
			if(connect(sock_fd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr)) == -1) 
			{
				// error 
				retval = LIBNETWORK_ERR_CONNECT;
				// 尝试重连
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
* FunctionName： libnetwork_closeSockConn
*/
void libnetwork_closeSockConn(void)
{
	// 关闭socket
	if(sock_fd != -1)
		close(sock_fd);	
	sock_fd = -1;
}

/**
* FunctionName：libnetwork_send_package
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
* FunctionName：libnetwork_recv_package
*/
int libnetwork_recv_package(char *recvBuf, int *recvLen)
{
	int retval = LIBNETWORK_ERR_NONE, ret;
	struct timeval tv;
	fd_set readfds;
	// 接收数据
	while(1)
	{
		// 非阻塞 读取数据
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
		
		//检查sock_fd是否在这个集合里面
		if(FD_ISSET(sock_fd, &readfds))
		{
			ret = recv(sock_fd, recvBuf, 1024, 0);
			if ((ret == -1) || (ret == 0))
				// 数据接收错误
				retval = LIBNETWORK_ERR_RECV;
			else
				*recvLen = ret;
			
			// 退出while循环
			break;
		}		
	}
	
	return retval;
}




