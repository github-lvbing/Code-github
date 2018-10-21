//UDP服务器的实现

#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket 
#include <netinet/in.h>  //sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strcat
#include <strings.h> 
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

	//bzero  memset
	bzero(&client_addr, sizeof(client_addr));
	bzero(&server_addr, sizeof(client_addr));

	//创建套接字  --->  socket( )
	if((sockfd = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0)
	{
		//errlog("fail to socket");
		perror("fail to socket");
		exit(1);
	}

	//填充服务器网络信息结构体 sockaddr_un
	server_addr.sun_family = AF_UNIX;
	strcpy(server_addr.sun_path, "server_socket");

	//将套接字与网络信息结构体绑定 ---> bind( )
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		errlog("fail to bind");
	}
	
	while(1)
	{
		if(recvfrom(sockfd, buf, N, 0, (struct sockaddr *)&client_addr, &addrlen) < 0)
		{
			errlog("fail to recvfrom");
		}

		if(strncmp(buf, "break", 5) == 0)
		{
			break;
		}
		else
		{
			printf("from client >>> %s\n", buf);

			strcat(buf, " *_*");

			if(sendto(sockfd, buf, N, 0, (struct sockaddr *)&client_addr, addrlen) < 0)
			{
				errlog("fail to sendto");
			}
		}
	}

	close(sockfd);

	return 0;
}
