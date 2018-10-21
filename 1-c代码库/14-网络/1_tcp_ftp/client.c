#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket connect
#include <netinet/in.h>  //sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strlen
#include <sys/stat.h>
#include <fcntl.h>

#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s,%s,%d\n", __FILE__, __func__, __LINE__);\
                          exit(1);\
						 }while(0)
#define N 128

int do_help()
{
	printf("***************************************************************\n");
	printf("************ 输入 / 功能 **************************************\n");
	printf("************ list / 查看服务器所在目录的文件名 ****************\n");
	printf("**** get filename / 下载服务器的文件 **************************\n");
	printf("**** put filename / 上传文件到服务器 **************************\n");
	printf("************ quit / 退出 **************************************\n");

	return 0;
}

int do_list(struct sockaddr_in server_addr)
{
	int sockfd;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char buf[N] = {};

	//创建套接字  --->  socket( )
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		//errlog("fail to socket");
		perror("fail to socket");
		exit(1);
	}

	//发送客户端的连接请求 --->accept( )
	if(connect(sockfd, (struct sockaddr *)&server_addr, addrlen) < 0)
	{ 
		errlog("fail to accept");
	}

	strcpy(buf, "L");
	send(sockfd, buf, N, 0);

	while(recv(sockfd, buf, N, 0) > 0)
	{
		printf("***%s\n", buf);
		usleep(15);
	}
	
	printf("目录接收完毕\n");
	close(sockfd);

	return 0;
}

int do_download(struct sockaddr_in server_addr, char *filename)
{
	int sockfd;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char buf[N] = {};
	int fd;
	ssize_t bytes;

	//创建套接字  --->  socket( )
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		//errlog("fail to socket");
		perror("fail to socket");
		exit(1);
	}

	//发送客户端的连接请求 --->accept( )
	if(connect(sockfd, (struct sockaddr *)&server_addr, addrlen) < 0)
	{ 
		errlog("fail to accept");
	}

	//发送 G+filename
	sprintf(buf, "G %s", filename);
	send(sockfd, buf, N, 0);

	recv(sockfd, buf, N, 0);

	//如果文件不存在
	if(buf[0] == 'N')
	{
		printf("%s is not exist\n", filename);

		return -1;
	}

	//文件存在
	if((fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0664)) < 0)
	{
		errlog("fail to open");
	}

	//接收数据并写入文件
	while((bytes = recv(sockfd, buf, N, 0)) > 0)
	{
		write(fd, buf, bytes);

		usleep(15);
	}

	printf("文件下载完毕\n");
	
	close(fd);
	close(sockfd);
	
	return 0;
}

int do_upload(struct sockaddr_in server_addr, char *filename)
{
	int sockfd;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	char buf[N] = {};
	int fd;
	ssize_t bytes;

	//创建套接字  --->  socket( )
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		//errlog("fail to socket");
		perror("fail to socket");
		exit(1);
	}

	//发送客户端的连接请求 --->accept( )
	if(connect(sockfd, (struct sockaddr *)&server_addr, addrlen) < 0)
	{ 
		errlog("fail to accept");
	}

	//发送P filename
	sprintf(buf, "P %s", filename);
	send(sockfd, buf, N, 0);

	//打开文件
	if((fd = open(filename, O_RDONLY)) < 0)
	{
		errlog("fail to open");
	}

	//读取内容并发送
	while((bytes = read(fd, buf, N)) > 0)
	{
		send(sockfd, buf, bytes, 0);

		usleep(25);
	}

	printf("文件上传成功\n");
	
	close(fd);
	close(sockfd);
	
	return 0;
}

int main(int argc, const char *argv[])
{
	struct sockaddr_in server_addr;
	char buf[N] = {};
	
	//填充服务器网络信息结构体 sockaddr_in
	//inet_addr : 将点分十进制IP地址转化为网络能够识别的整型数据
	//htons ： 将主机字节序转化为网络字节序
	//atoi ：将字符串转化为整型数据
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if(argc < 3)
	{
		printf("argument is too less\n");
		exit(1);
	}

	printf("******************\n");
	printf("请输入help查看选项\n");
	printf("******************\n");

	while(1)
	{
		//输入命令
		printf("请输入：");
		fgets(buf, N, stdin);  //help list get+filename put+filename quit
		buf[strlen(buf) - 1] = '\0';
	
		if(strncmp(buf, "help", 4) == 0)
		{
			do_help();
		}
		else if(strncmp(buf, "list", 4) == 0)
		{
			do_list(server_addr);
		}
		else if(strncmp(buf, "get", 3) == 0)
		{
			do_download(server_addr, buf + 4);   //get filename
		}
		else if(strncmp(buf, "put", 3) == 0)
		{
			do_upload(server_addr, buf + 4);  //put filename
		}
		else if(strncmp(buf, "quit", 4) == 0)
		{
			break;
		}
		else
		{
			printf("error command...\n");
		}
	}

	return 0;
}
