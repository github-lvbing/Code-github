#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


#define  N  128

#define L 1
#define B 2
#define Q 3

typedef struct {
	int type;
	char name[N];
	char text[N];
}MSG;

typedef struct node{
	struct sockaddr_in clientaddr;
	struct node *next;
}linknode, *linklist;

linklist create_linklist()
{

	return ;
}

//插入链表
int process_login()
{

	return 0;
}
//遍历链表
int process_broadcast()
{
	return 0;
}
//删除结点
int process_quit()
{
	return 0;
}

int main(int argc, const char *argv[])
{
	int sockfd;
	struct sockaddr_in serveraddr, clientaddr;
	socklen_t addrlen = sizeof(clientaddr);
	int nbyte;
	pid_t pid;
	MSG msg;

	char buf[N] = {0};

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("fail to socket");
		return -1;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	if(bind(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
	{
		perror("fail to bind");
		return -1;
	}

	if((pid = fork()) < 0)
	{
		perror("fail to fork");
		return -1;
	}
	else if(pid == 0) // system msg
	{

	}
	else
	{
		h = create_linklist();
		while(1)
		{
			recvfrom(sockfd, &msg, sizeof(msg), 0);

			switch(msg.type)
			{
				case L:
					process_login();
					break;
				case B:
					process_broadcast();
					break;
				case Q:
					process_quit();
					break;
				default:
					printf("msg error!\n");
			}
		}

	}

	close(sockfd);


	return 0;
}
