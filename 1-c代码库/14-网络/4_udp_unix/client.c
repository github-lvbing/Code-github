//UDP客户端的实现

#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket 
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
	struct sockaddr_un server_addr, client_addr;
	socklen_t addrlen = sizeof(struct sockaddr_un);
	char buf[N] = {};

	//创建套接字  --->  socket( )
	if((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
	{
		//errlog("fail to socket");
		perror("fail to socket");
		exit(1);
	}

	//填充服务器本地信息结构体 sockaddr_in
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "server_socket");

	//注意：如果不绑定客户端的本地信息结构体，则客户端接收不到服务器的消息
	//填充客户端本地信息结构体
	client_addr.sun_family = AF_UNIX;
	strcpy(client_addr.sun_path, "client_socket");

	if(bind(sockfd, (struct sockaddr *)&client_addr, addrlen) < 0)
	{
		errlog("fail to bind");
	}

	while(1)
	{
		fgets(buf, N, stdin);
		buf[strlen(buf) - 1] = '\0';

		if(sendto(sockfd, buf, N, 0, (struct sockaddr *)&server_addr, addrlen) < 0)
		{
			errlog("fail to sendto");
		}

		if(strncmp(buf, "break", 5) == 0)
		{
			break;
		}
		else
		{
			if(recvfrom(sockfd, buf, N, 0, (struct sockaddr *)&server_addr, &addrlen) < 0)
			//if(recvfrom(sockfd, buf, N, 0, NULL, NULL) < 0)
			{
				errlog("fail to recvfrom");
			}

			printf("from server >>> %s\n", buf);
		}
	}

	close(sockfd);

	return 0;
}
