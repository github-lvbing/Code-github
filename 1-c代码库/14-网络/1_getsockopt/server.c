//TCP服务器的实现

#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket bind listen accept
#include <netinet/in.h>  //sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strcat

#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s,%s,%d\n", __FILE__, __func__, __LINE__);\
                          exit(1);\
						 }while(0)
#define N 128

int main(int argc, const char *argv[])
{
	int sockfd, acceptfd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char buf[N] = {};

	if(argc < 3)
	{
		printf("argument is too less\n");
		exit(1);
	}

	//创建套接字  --->  socket( )
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		//errlog("fail to socket");
		perror("fail to socket");
		exit(1);
	}

	//填充服务器网络信息结构体 sockaddr_in
	//inet_addr : 将点分十进制IP地址转化为网络能够识别的整型数据
	//htons ： 将主机字节序转化为网络字节序
	//atoi ：将字符串转化为整型数据
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	//将套接字与网络信息结构体绑定 ---> bind( )
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		errlog("fail to bind");
	}
	
	//将套接字设置为监听状态 ---> listen( )
	if(listen(sockfd, 5) < 0)
	{
		errlog("fail to listen");
	}

	//使用getsockopt获取网络属性

	int opt_value = 0;
	socklen_t opt_len = sizeof(opt_value);

	if(getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &opt_value, &opt_len) < 0)
	{
		errlog("fail to getsockopt");
	}

	printf("recv_buf = %dk\n", opt_value/1024);

	if(getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &opt_value, &opt_len) < 0)
	{
		errlog("fail to getsockopt");
	}

	printf("send_buf = %dk\n", opt_value/1024);

	
	//阻塞等待客户端的连接请求 --->accept( )
	if((acceptfd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen)) < 0)
	{ 
		errlog("fail to accept");
	}

	printf("%s ---> %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	while(1)
	{
		if(recv(acceptfd, buf, N, 0) < 0)
		{
			errlog("fail to recv");
		}

		if(strncmp(buf, "break", 5) == 0)
		{
			break;
		}
		else
		{
			printf("from client >>> %s\n", buf);

			strcat(buf, " *_*");

			if(send(acceptfd, buf, N, 0) < 0)
			{
				errlog("fail to send");
			}
		}
	}

	close(acceptfd);
	close(sockfd);

	return 0;
}
