#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>


#define  L  1
#define  B  2
#define  Q  3

#define  N  128

typedef struct {
	int type;
	char name[N];
	char text[N];
}MSG;

int main(int argc, const char *argv[])
{
	int sockfd;
	pid_t pid;
	MSG msg;
	struct sockaddr_in serveraddr;

	char buf[N] = {0};

	if(argc != 3)
	{
		fprintf(stderr,"Usage:%s serverip port.\n", argv[0]);
		return -1;
	}

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("fail to socket");
		return -1;
	}

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(argv[1]);
	serveraddr.sin_port = htons(atoi(argv[2]));

	msg.type = L;

	fgets(msg.name, N, stdin);
	msg.name[strlen(msg.name)-1] = '\0';
	
	if((pid = fork()) < 0)
	{
		perror("fail to fork");
		return -1;
	}
	else if(pid == 0)  // child sendto
	{
		while(1)
		{
			fgets(msg.text, N, stdin);
			if(strncmp(msg.text, "quit", 4) == 0)
			{
				msg.type = Q;
				sendto();
			}

			msg.type = B;
			sendto();
		}
	}
	else  //parent recvfrom
	{
		while(1)
		{
			recvfrom();
		}
	}

	close(sockfd);


	return 0;
}
