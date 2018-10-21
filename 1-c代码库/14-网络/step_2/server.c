#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <signal.h>
#include <time.h>
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
	char data[256];   // password or word
} MSG;

void do_register(int connectfd, MSG *msg, sqlite3 *db);
void do_login(int connectfd, MSG *msg, sqlite3 *db);
void do_query(int connectfd, MSG *msg, sqlite3 *db);
void do_history(int connectfd, MSG *msg, sqlite3 *db);
void do_client(int connectfd, sqlite3 *db);
int main(int argc, char *argv[])
{
	int listenfd, connectfd;
	struct sockaddr_in server_addr;
	pid_t pid;
	sqlite3 *db;

	if (argc < 3)
	{
		printf("Usage : %s <ip> <port>\n", argv[0]);
		exit(-1);
	}

	if (sqlite3_open(DATABASE, &db) != SQLITE_OK)
	{
		printf("error : %s\n", sqlite3_errmsg(db));
		exit(-1);
	}

	if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("fail to socket");
		exit(-1);
	}

	bzero(&server_addr, sizeof(server_addr));

	server_addr.sin_family = PF_INET;
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	server_addr.sin_port = htons(atoi(argv[2]));

	if (bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("fail to bind");
		exit(-1);
	}

	if (listen(listenfd, 5) < 0)
	{
		perror("fail to listen");
		exit(-1);
	}

	signal(SIGCHLD, SIG_IGN);// 避免僵尸进程

	while ( 1 )
	{
		if ((connectfd = accept(listenfd, NULL, NULL)) < 0)
		{
			perror("fail to accept");
			exit(-1);
		}
	
		if ((pid = fork()) < 0)
		{
			perror("fail to fork");
			exit(-1);
		}
	
		if (pid == 0)
		{
			do_client(connectfd, db);
		}
		close(connectfd);
	}
	return 0;
}

//注册函数
void do_register(int connectfd, MSG *msg, sqlite3 *db)
{
	char sqlstr[128] = {0};
	char *errmsg;

	//向usr表中插入数据
	sprintf(sqlstr, "insert into usr values('%s', '%s')", msg->name, msg->data);
	
	if(sqlite3_exec(db, sqlstr, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		sprintf(msg->data, "user %s already exist!!!", msg->name);
	}
	else
	{
		strcpy(msg->data, "OK");
	}

	send(connectfd, msg, sizeof(MSG), 0);

	return;
}

//登录
void do_login(int connectfd, MSG *msg, sqlite3 *db)
{
	char sqlstr[128] = {0};
	char *errmsg, **result;
	int nrow, ncolumn;
	
	//查询是否有姓名和密码下面匹配的一行
	sprintf(sqlstr, "select * from usr where name = '%s' and pass = '%s'", msg->name, msg->data);

	if(sqlite3_get_table(db, sqlstr, &result, &nrow, &ncolumn, &errmsg) != SQLITE_OK)
	{
		printf("error : %s\n", errmsg);
	}
	//不存在
	if(nrow == 0)
	{
		strcpy(msg->data, "name or password is wrong!!!");
	}
	//存在
	else
	{
		strcpy(msg->data, "OK");
	}

	send(connectfd, msg, sizeof(MSG), 0);
	return;
}

void do_query(int connectfd, MSG *msg, sqlite3 *db)
{
	return;
}

void do_history(int connectfd, MSG *msg, sqlite3 *db)
{
	return;
}
void do_client(int connectfd, sqlite3 *db)
{
	MSG msg;
	while (recv(connectfd, &msg, sizeof(msg), 0) > 0)  // receive request
	{
		switch ( msg.type )
		{
		case R :
			do_register(connectfd, &msg, db);
			break;
		case L :
			do_login(connectfd, &msg, db);
			break;
		case Q :
			do_query(connectfd, &msg, db);
			break;
		case H :
			do_history(connectfd, &msg, db);
			break;
		}
	}
	printf("client quit\n");
	exit(0);
	return;
}
