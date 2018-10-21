#include <stdio.h>   //printf
#include <arpa/inet.h>   //inet_addr htons
#include <sys/types.h>
#include <sys/socket.h>  //socket bind listen accept
#include <netinet/in.h>  //sockaddr_in
#include <stdlib.h>  //exit
#include <unistd.h> //close
#include <string.h> //strcat
#include <dirent.h> //opendir
#include <errno.h>  //errno
#include <sys/stat.h>
#include <fcntl.h>

#define errlog(errmsg) do{perror(errmsg);\
						  printf("%s,%s,%d\n", __FILE__, __func__, __LINE__);\
                          exit(1);\
						 }while(0)
#define N 128

int do_list(int acceptfd)
{
	DIR *dirp;
	struct dirent *dir;
	char buf[N] = {};

	//打开目录
	if((dirp = opendir(".")) == NULL)
	{
		errlog("fail to opendir");
	}

	//读取文件名
	while((dir = readdir(dirp)) != NULL)	
	{
		if(dir->d_name[0] == '.')
		{
			continue;
		}

		strcpy(buf, dir->d_name);

		send(acceptfd, buf, N, 0);
		
		usleep(25);
	}

	printf("目录清单已发送完毕\n");
	close(acceptfd);

	return 0;
}

int do_download(int acceptfd, char *filename)
{
	int fd;
	ssize_t bytes;
	char buf[N] = {};

	//打开文件并判断文件是否存在
	if((fd = open(filename, O_RDONLY)) < 0)
	{
		//不存在
		if(errno == ENOENT)
		{
			strcpy(buf, "N");
			send(acceptfd, buf, N, 0);
			
			close(acceptfd);
		}
		else
		{
			errlog("fail to open");
		}
	}

	strcpy(buf, "Y");
	send(acceptfd, buf, N, 0);

	//如果存在，读取文件内容并发送
	while((bytes = read(fd, buf, N)) > 0)
	{
		send(acceptfd, buf, bytes, 0);

		usleep(25);
	}

	printf("文件传送完毕\n");

	close(acceptfd);

	return 0;
}

int do_upload(int acceptfd, char *filename)
{
	int fd;
	ssize_t bytes;
	char buf[N] = {};

	//创建文件
	if((fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0664)) < 0)
	{
		errlog("fail to open");
	}

	while((bytes = recv(acceptfd, buf, N, 0)) > 0)
	{
		write(fd, buf, bytes);

		usleep(15);
	}

	printf("文件接收成功\n");

	close(fd);
	close(acceptfd);

	return 0;
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
	
	while(1)
	{
		//阻塞等待客户端的连接请求 --->accept( )
		if((acceptfd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen)) < 0)
		{ 
			errlog("fail to accept");
		}

		printf("%s ---> %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		recv(acceptfd, buf, N, 0);
		printf("buf >>> %s\n", buf);

		switch(buf[0])
		{
		case 'L':
			do_list(acceptfd);
			break;
		case 'G':
			do_download(acceptfd, buf + 2);  //G filename
			break;
		case 'P':
			do_upload(acceptfd, buf + 2);  //P filename
			break;
		default:
			printf("error command\n");
			break;
		}
	}

	close(sockfd);

	return 0;
}
