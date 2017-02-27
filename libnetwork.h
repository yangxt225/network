/**
*	���紦���(socket����)
*/
#ifndef __NETWORK_H__
#define __NETWORK_H__

#ifdef __cplusplus
extern "C" {
#endif

// �������Ӵ������
#define LIBNETWORK_ERR_NONE 		0 	 // �ɹ�
#define LIBNETWORK_ERR_LINK 		1 	 // ����(eth)����״̬
#define LIBNETWORK_ERR_OPENSOCK		2	 // ����socket���� ����
#define LIBNETWORK_ERR_CONNECT		3 	 // �ͻ��˽���tcp���� ����
#define LIBNETWORK_ERR_SEND 		4
#define LIBNETWORK_ERR_RECV 		5
#define LIBNETWORK_ERR_SELECT 		6 	// ���÷����� 		
#define LIBNETWORK_ERR_TIMEOUT 		7 	// ��ʱ

#define NETWK_COMM_TIMEOUT 			10	// �������ӳ�ʱʱ��

/**
* FunctionName�� libnetwork_getNetlinkStat
* Desc:			 ����socket����
* @param1:		 �Ƿ���ҪrootȨ��
* @param2:		 if_name ���������ƣ��� eth0��
* Return: 		
				 -1 -- no root
*                -2 -- error , details can check errno
*                1 -- interface link up
*				 0 -- interface link down.	 
* 
*/
int libnetwork_getNetlinkStat(int chkroot, const char *if_name);

/**
* FunctionName�� libnetwork_createSockConn
* Desc:			 ����socket����
* @param1:		 IP
* @param2��		 Port	
* return��	 	 ���ش������
*/
int libnetwork_createSockConn(char *ip, int port);

/**
* FunctionName�� libnetwork_closeSockConn
* Desc:			 �ر�socket����
*/
void libnetwork_closeSockConn(void);

/**
* FunctionName�� libnetwork_send_package
* Desc:			 ��������
* @param1:		 input,���������ݻ�����
* @param2��		 input,���������� ����	
* return��	 	 ���ش������
*/
int libnetwork_send_package(const char *sendBuf, const int sendLen);

/**
* FunctionName�� libnetwork_send_package
* Desc:			 ��������
* @param1:		 output,���յ������ݻ�����
* @param2��		 output,���յ������� ����	
* return��	 	 ���ش������
*/
int libnetwork_recv_package(char *recvBuf, int *recvLen);

#ifdef __cplusplus
}
#endif

#endif






