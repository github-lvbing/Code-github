//UDP客户端的实现

#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket 
#include <netinet/in.h>  //sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strlen
#include <signal.h>

#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s,%s,%d\n", __FILE__, __func__, __LINE__);\
                          exit(1);\
						 }while(0)
#define N 128

#define L 1
#define B 2
#define Q 3

typedef struct{
	int type;
	char name[N];
	char text[N];
}MSG;

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in server_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	MSG msg;
	pid_t pid;

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

	//填充服务器网络信息结构体 sockaddr_in
	//inet_addr : 将点分十进制IP地址转化为网络能够识别的整型数据
	//htons ： 将主机字节序转化为网络字节序
	//atoi ：将字符串转化为整型数据
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	//登录
	msg.type = L;

	printf("< 请登录 >\n");
	printf("name : ");

	fgets(msg.name, N, stdin);
	msg.name[strlen(msg.name) - 1] = '\0';

	sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&server_addr, addrlen);

	//创建父子进程
	if((pid = fork()) < 0)
	{
		errlog("fail to fork");
	}
	else if(pid == 0)  //子进程负责发送消息
	{
		while(1)
		{
			fgets(msg.text, N, stdin);
			msg.text[strlen(msg.text) - 1] = '\0';

			//退出
			if(strncmp(msg.text, "quit", 4) == 0)
			{
				msg.type = Q;
				
				sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&server_addr, addrlen);
			
				kill(getppid(), SIGKILL);

				exit(0);
			}

			//发送广播信息
			msg.type = B;

			sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&server_addr, addrlen);
		}
	}
	else  //父进程负责接收消息
	{
		while(1)
		{
			recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&server_addr, &addrlen);

			printf("%s\n", msg.text);
		}
		
	}

	close(sockfd);

	return 0;
}
