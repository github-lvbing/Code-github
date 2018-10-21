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

typedef struct node{
	struct sockaddr_in addr;
	struct node *next;
}linklist_t;

linklist_t *linklist_create()
{
	linklist_t *h = (linklist_t *)malloc(sizeof(linklist_t));

	h->next = NULL;
		
	return h;
}

int do_login(int sockfd, MSG msg, linklist_t *h,  struct sockaddr_in client_addr)
{
	sprintf(msg.text, "%s login!", msg.name);
	
	linklist_t *p = h->next;

	while(p != NULL)
	{
		sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&p->addr, sizeof(client_addr));

		p = p->next;
	}

	//将新登录的人加入到链表里面
	linklist_t *temp = (linklist_t *)malloc(sizeof(linklist_t));
	temp->addr = client_addr;

	temp->next = h->next;
	h->next = temp;
	
	return 0;
}

int do_broadcast(int sockfd, MSG msg, linklist_t *h,  struct sockaddr_in client_addr)
{
	char buf[N] = {};
	linklist_t *p = h;
	sprintf(buf, "%s said %s", msg.name, msg.text);

	strcpy(msg.text, buf);

	while(p->next != NULL)
	{
		if(memcmp(&client_addr, &p->next->addr, sizeof(client_addr)) == 0)
		{
			p = p->next;
		}
		else
		{
			sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&p->next->addr, sizeof(client_addr));
			p = p->next;
		}
	}

	return 0;
}

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	MSG msg;
	pid_t pid;

	//bzero  memset
	bzero(&client_addr, sizeof(client_addr));
	bzero(&server_addr, sizeof(client_addr));

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

	//将套接字与网络信息结构体绑定 ---> bind( )
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		errlog("fail to bind");
	}

	if((pid = fork()) < 0)
	{
		errlog("fail to fork");
	}
	else if(pid == 0) //子进程负责发送系统消息
	{
		while(1)
		{
			msg.type = B;

			strcpy(msg.name, "server");

			fgets(msg.text, N, stdin);
			msg.text[strlen(msg.text) - 1] = '\0';

			sendto(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&server_addr, addrlen);
		}

	}
	else //父进程负责接收消息并处理
	{
		linklist_t *h = linklist_create();
		while(1)
		{
			recvfrom(sockfd, &msg, sizeof(msg), 0, (struct sockaddr *)&client_addr, &addrlen);
			printf("type:%d ---> name:%s ---> text:%s\n", msg.type, msg.name, msg.text);

			switch(msg.type)
			{
			case L:
				do_login(sockfd, msg, h, client_addr);
				break;
			case B:
				do_broadcast(sockfd, msg, h, client_addr);
				break;
			case Q:
				//do_quit();
				break;
			default:
				printf("error !!!\n");
			}
		}
	}

	close(sockfd);

	return 0;
}
