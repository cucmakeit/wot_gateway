#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <cstdio>
#include "common.h"

void init_winsock()
{
	int ret;
	WSADATA wsaData;

	ret = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ret != 0) {
		printf("WSAStartup failed: %d\n", ret);
		exit(-1);
	}
}

#ifdef _WIN32
IN_ADDR get_addr_info(const char* hostname)
{
	int ret;
	char ipstr[16];
	struct addrinfo hint;
	struct addrinfo *ans,*cur;

	//ZeroMemory(&hint, sizeof(hint));
	memset(&hint,0,sizeof(hint));//���ڿ�ƽ̨���ұ�����0
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;

	ret = getaddrinfo(hostname,NULL,&hint,&ans);
	if (ret != 0) {
		fprintf(stderr,"getaddrinfo failed:%s\n",gai_strerror(ret));
		exit(-1);
	}
	
	
	for (cur = ans; cur != NULL; cur = cur->ai_next) {
		inet_ntop(AF_INET,&(((struct sockaddr_in*)(cur->ai_addr))->sin_addr),
			ipstr,16);
		printf("%s\n",ipstr);
	}
	
	//�ٶ����ص��ǵ�һ����ַ�ṹ
	return ((struct sockaddr_in*)ans->ai_addr)->sin_addr;
}

#endif
/*
	fd_cli:�ͻ��׽���������ָ��
	hostip:�����ӵ�Զ�̵�����ip
*/
void create_client_sock(int* fd_cli,const char* hostip,int port)
{
	struct sockaddr_in cliaddr;//���岻�ܷź���
	int ret;

	/* �˺����Ѿ���ʱ
	host = gethostbyname("api.yeelink.com");
	if( host == NULL ) {
		printf("DNS failed\r\n");
		return;
	}*/

	*fd_cli = socket(AF_INET, SOCK_STREAM, 0);

	cliaddr.sin_family = AF_INET;
	cliaddr.sin_port = htons(port);
	//cliaddr.sin_addr = get_addr_info(LCZ_HOST);

	inet_pton(AF_INET,hostip,(void*)&cliaddr.sin_addr);
	
	memset(&(cliaddr.sin_zero), 0, sizeof(cliaddr.sin_zero));

	ret = connect(*fd_cli, (struct sockaddr *)&cliaddr, sizeof(struct sockaddr));
	if( ret == 0 ){
		printf("connect ok\n");
	}
	else {
		printf("connect to python server socket failed\n");
	}
}
