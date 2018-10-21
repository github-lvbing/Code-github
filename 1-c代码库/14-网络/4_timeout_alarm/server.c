//TCP服务器的实现

#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket bind listen accept
#include <netinet/in.h>  //sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strcat
#include <errno.h>
#include <signal.h>

#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s,%s,%d\n", __FILE__, __func__, __LINE__);\
                          exit(1);\
						 }while(0)
#define N 128

void handler(int sig)
{

}

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

	//使用alarm闹钟实现网络超时检测
	//当程序直接调用alarm，当闹钟响时，程序会直接退出，
	//如果当闹钟响时，捕捉到此信号，调用signal执行信号处理函数，当信号处理函数执行完毕之后，还会回到
	//原来的位置继续执行，此性质称之为自重启属性，所以如果想实现超时检测，需要将其关闭，
	//关闭之后，当信号处理函数执行完毕之后，不会接着之前的代码执行，而会立即返回（跳过之前运行的代码）
	
	//调用alarm设置超时时间，设置一次，只有有效一次，所以需要每次都设置
	
	//注意：对位进行操作，需要进行读、改、写
	
	//第一步：获取旧的行为
	struct sigaction act;
	
	if(sigaction(SIGALRM, NULL, &act) < 0)
	{
		errlog("fail to sigaction");
	}

	//第二步：修改行为
	act.sa_handler = handler;
	act.sa_flags = act.sa_flags & (~SA_RESTART); //01340443  &  (~01000000) 

	//将新的行为写进去
	if(sigaction(SIGALRM, &act, NULL) < 0)
	{
		errlog("fail to sigaction");
	}

	while(1)
	{
		alarm(5);

		//阻塞等待客户端的连接请求 --->accept( )
		if((acceptfd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen)) < 0)
		{ 
			//printf("errno = %d\n", errno);
			if(errno == 4)
			{
				printf("accept timeout ...\n");
			}
			else
			{
				errlog("fail to accept");
			}
		}
		else
		{
			printf("%s ---> %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

			while(1)
			{
				alarm(5);

				if(recv(acceptfd, buf, N, 0) < 0)
				{
					//printf("errno = %d\n", errno);
					if(errno == 4)
					{
						printf("recv timeout ...\n");
					}
					else
					{
						errlog("fail to recv");
					}
				}
				else
				{
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
			}
		}
	}

	close(acceptfd);
	close(sockfd);

	return 0;
}
