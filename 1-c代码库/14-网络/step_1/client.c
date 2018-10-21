#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define  N  16
#define  R  1   //  user register
#define  L  2   //  user login
#define  Q  3   //  query word
#define  H  4   //  history record

#define DATABASE "my.db"

typedef struct 
{
	int type;
	char name[N];
	char data[256];   // password or word or remark
} MSG;

void do_register(int socketfd, MSG *msg);
int do_login(int socketfd, MSG *msg);
void do_query(int socketfd, MSG *msg);
void do_history(int socketfd, MSG *msg);

int main(int argc, char *argv[])
{
	int socketfd ;
	struct sockaddr_in server_addr;
	MSG msg;
	if (argc < 3)
	{
		printf("Usage : %s <serv_ip> <serv_port>\n", argv[0]);
		exit(-1);
	}
	if ((socketfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("fail to socket");
		exit(-1);
	}

	bzero(&server_addr, sizeof(server_addr));
	//memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = PF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));
	
	if (connect(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("fail to connect");
		exit(-1);
	}
	int n;
	while(1)
	{
		printf("************************************\n");
		printf("* 1: register   2: login   3: quit *\n");
		printf("************************************\n");
		printf("please choose : ");

		if(scanf("%d", &n) <= 0)
		{
			perror("scanf");
			exit(-1);
		}

		switch(n)
		{
			case 1:
				do_register(socketfd, &msg);
				break;
			case 2:
				if(do_login(socketfd, &msg) == 1)
					goto next;
				break;
			case 3:
				close(socketfd);
				exit(0);
		}
	}

next:
	while(1)
	{
		printf("************************************\n");
		printf("* 1: query   2: history   3: quit *\n");
		printf("************************************\n");
		printf("please choose : ");

		if(scanf("%d", &n) <= 0)
		{
			perror("scanf");
			exit(-1);
		}
		switch(n)
		{
			case 1:
				do_query(socketfd, &msg);
				break;
			case 2:
				do_history(socketfd, &msg);
				break;
			case 3:
				close(socketfd);
				exit(0);
		}
	}
	return 0;
}
void do_register(int socketfd, MSG *msg)
{
	puts("register");
}
int do_login(int socketfd, MSG *msg)
{
	puts("login");
	return 1;
}
void do_query(int socketfd, MSG *msg)
{
	puts("query");
}
void do_history(int socketfd, MSG *msg)
{
	puts("history");
}
