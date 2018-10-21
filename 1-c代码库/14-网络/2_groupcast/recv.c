//组播的接收者

#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket 
#include <netinet/in.h>  //sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strcat
#include <strings.h> 

#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s,%s,%d\n", __FILE__, __func__, __LINE__);\
                          exit(1);\
						 }while(0)
#define N 128

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in groupcast_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char buf[N] = {};

	//bzero  memset
	bzero(&groupcast_addr, sizeof(groupcast_addr));

	if(argc < 3)
	{
		printf("argument is too less\n");
		exit(1);
	}

	//创建套接字  --->  socket( )
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		//errlog("fail to socket");
		perror("fail to socket");
		exit(1);
	}
	
	//加入多播组
	struct ip_mreq mreq;

	mreq.imr_multiaddr.s_addr = inet_addr(argv[1]);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	//填充服务器组播信息结构体 sockaddr_in
	//inet_addr : 将点分十进制IP地址转化为网络能够识别的整型数据
	//htons ： 将主机字节序转化为网络字节序
	//atoi ：将字符串转化为整型数据
	groupcast_addr.sin_family = AF_INET;
	groupcast_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	groupcast_addr.sin_port = htons(atoi(argv[2]));

	//将套接字与网络信息结构体绑定 ---> bind( )
	if(bind(sockfd, (struct sockaddr *)&groupcast_addr, sizeof(groupcast_addr)) < 0)
	{
		errlog("fail to bind");
	}


	while(1)
	{
		if(recvfrom(sockfd, buf, N, 0, (struct sockaddr *)&groupcast_addr, &addrlen) < 0)
		{
			errlog("fail to recvfrom");
		}

		printf("groupcast >>> %s\n", buf);
	}

	close(sockfd);

	return 0;
}
