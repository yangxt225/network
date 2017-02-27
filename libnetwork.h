/**
*	网络处理库(socket连接)
*/
#ifndef __NETWORK_H__
#define __NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif

// 网络连接错误码表
#define LIBNETWORK_ERR_NONE 		0 	 // 成功
#define LIBNETWORK_ERR_LINK 		1 	 // 网卡(eth)连接状态
#define LIBNETWORK_ERR_OPENSOCK		2	 // 创建socket连接 错误
#define LIBNETWORK_ERR_CONNECT		3 	 // 客户端建立tcp连接 错误
#define LIBNETWORK_ERR_SEND 		4
#define LIBNETWORK_ERR_RECV 		5
#define LIBNETWORK_ERR_SELECT 		6 	// 设置非阻塞 		
#define LIBNETWORK_ERR_TIMEOUT 		7 	// 超时

#define NETWK_COMM_TIMEOUT 			10	// 网络连接超时时间

/**
* FunctionName： libnetwork_getNetlinkStat
* Desc:			 创建socket连接
* @param1:		 是否需要root权限
* @param2:		 if_name 是网卡名称，如 eth0。
* Return: 		
				 -1 -- no root
*                -2 -- error , details can check errno
*                1 -- interface link up
*				 0 -- interface link down.	 
* 
*/
int libnetwork_getNetlinkStat(int chkroot, const char *if_name);

/**
* FunctionName： libnetwork_createSockConn
* Desc:			 创建socket连接
* @param1:		 IP
* @param2：		 Port	
* return：	 	 返回错误码表
*/
int libnetwork_createSockConn(char *ip, int port);

/**
* FunctionName： libnetwork_closeSockConn
* Desc:			 关闭socket连接
*/
void libnetwork_closeSockConn(void);

/**
* FunctionName： libnetwork_send_package
* Desc:			 发送数据
* @param1:		 input,待发送数据缓存区
* @param2：		 input,待发送数据 长度	
* return：	 	 返回错误码表
*/
int libnetwork_send_package(const char *sendBuf, const int sendLen);

/**
* FunctionName： libnetwork_send_package
* Desc:			 接收数据
* @param1:		 output,接收到的数据缓存区
* @param2：		 output,接收到的数据 长度	
* return：	 	 返回错误码表
*/
int libnetwork_recv_package(char *recvBuf, int *recvLen);

#ifdef __cplusplus
}
#endif

#endif






