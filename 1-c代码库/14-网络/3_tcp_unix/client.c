//TCP客户端的实现

#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket connect
#include <netinet/in.h>  //sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strlen
#include <sys/un.h>

#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s,%s,%d\n", __FILE__, __func__, __LINE__);\
                          exit(1);\
						 }while(0)
#define N 128


int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_un server_addr;
	socklen_t addrlen = sizeof(struct sockaddr_un);
	char buf[N] = {};

	//创建套接字  --->  socket( )
	if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		//errlog("fail to socket");
		perror("fail to socket");
		exit(1);
	}

	//填充服务器local信息结构体 sockaddr_un
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "server_socket");

	//发送客户端的连接请求 --->accept( )
	if(connect(sockfd, (struct sockaddr *)&server_addr, addrlen) < 0)
	{ 
		errlog("fail to accept");
	}

	while(1)
	{
		fgets(buf, N, stdin);
		buf[strlen(buf) - 1] = '\0';

		if(send(sockfd, buf, N, 0) < 0)
		{
			errlog("fail to send");
		}

		if(strncmp(buf, "break", 5) == 0)
		{
			break;
		}
		else
		{
			if(recv(sockfd, buf, N, 0) < 0)
			{
				errlog("fail to recv");
			}

			printf("from server >>> %s\n", buf);
		}
	}

	close(sockfd);

	return 0;
}
