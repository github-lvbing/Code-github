//使用select实现TCP并发服务器
//TCP服务器的实现

#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket bind listen accept
#include <netinet/in.h>  //sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <sys/select.h>
#include <sys/time.h>
#include <string.h>

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
	fd_set readfds;
	int maxfd;
	int i = 0;
	fd_set tempfds;
	int num = 0;

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

	printf("sockfd = %d\n", sockfd);

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

	//使用select函数实现网络超时检测
	//注意：使用select设置的超时时间，只会有效一次，所以需要每次都设置

	//注意：当select函数返回，意味着有文件描述符准备就绪，
	//	    此时，函数会将除这个文件描述符以外的其他文件描述符清除，
	//	    所以需要每次都添加。
	 
	//第一步：清空一个集合
	FD_ZERO(&readfds);
	
	//第二步：将需要的文件描述符添加到集合里面
	FD_SET(sockfd, &readfds);

	maxfd = sockfd;


	while(1)
	{
		//设置超时时间
		struct timeval out_time;
		out_time.tv_sec = 5;
		out_time.tv_usec = 0;

		tempfds = readfds;

		//第三步：调用函数，阻塞等待文件描述符准备就绪
		if((num = select(maxfd + 1, &tempfds, NULL, NULL, &out_time)) < 0)
		{
			errlog("fail to select");
		}
		else if(num == 0)
		{
			printf("timeout ...\n");
		}
		else
		{
			//第四步：判断时哪个文件描述符准备就绪
			for(i = 0; i < maxfd + 1; i++)
			{
				if(FD_ISSET(i, &tempfds))
				{
					if(i == sockfd)
					{
						//阻塞等待客户端的连接请求 --->accept( )
						if((acceptfd = accept(i, (struct sockaddr *)&client_addr, &addrlen)) < 0)
						{ 
							errlog("fail to accept");
						}

						printf("%s ---> %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

						printf("acceptfd = %d\n", acceptfd);

						FD_SET(acceptfd, &readfds);

						maxfd = maxfd > acceptfd ? maxfd : acceptfd;
					}
					else
					{
						if(recv(i, buf, N, 0) < 0)
						{
							errlog("fail to recv");
						}

						if(strncmp(buf, "break", 5) == 0)
						{
							FD_CLR(i, &readfds);
							close(i);

							break;
						}
						else
						{
							printf("from client >>> %s\n", buf);

							strcat(buf, " *_*");

							if(send(i, buf, N, 0) < 0)
							{
								errlog("fail to send");
							}
						}
					}
				}
			}
		}
	}

	close(acceptfd);
	close(sockfd);

	return 0;
}
