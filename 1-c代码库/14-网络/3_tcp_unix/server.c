//TCP服务器的实现

#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket bind listen accept
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strcat
#include <sys/un.h>

#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s,%s,%d\n", __FILE__, __func__, __LINE__);\
                          exit(1);\
						 }while(0)
#define N 128

int main(int argc, const char *argv[])
{
	int sockfd, acceptfd;
	struct sockaddr_un server_addr, client_addr;
	socklen_t addrlen = sizeof(struct sockaddr_un);
	char buf[N] = {};

	//创建套接字  --->  socket( )
	if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
	{
		//errlog("fail to socket");
		perror("fail to socket");
		exit(1);
	}

	//填充服务器本地信息结构体 sockaddr_un
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "server_socket");

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
	
	//阻塞等待客户端的连接请求 --->accept( )
	if((acceptfd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen)) < 0)
	{ 
		errlog("fail to accept");
	}

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
