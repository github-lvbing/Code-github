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

//定义数据库
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
int  do_searchword(int connectfd, MSG *msg);
void getdata(char data[]);
int history_callback(void *arg, int f_num, char **f_value, char **f_name);

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
	
	//打开数据库（如果数据库已经创建好了，调用函数后，之后利用指针操作数据库）
	//数据库里面有两个表，一个负责存放用户名和密码，用户名唯一，另一个负责存放历史记录
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

//注册
void do_register(int connectfd, MSG *msg, sqlite3 *db)
{
	char sqlstr[128] = {0};
	char *errmsg;

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
	
	sprintf(sqlstr, "select * from usr where name = '%s' and pass = '%s'", msg->name, msg->data);
	
	if(sqlite3_get_table(db, sqlstr, &result, &nrow, &ncolumn, &errmsg) != SQLITE_OK)  
	{
		printf("error : %s\n", errmsg);
	}
	
	//nrow这个参数起始位置为1，所以等于0表示没有找到
	if(nrow == 0)
	{
		strcpy(msg->data, "name or password is wrony!!!");
	}
	else
	{
		strncpy(msg->data, "OK", 256);
	}

	send(connectfd, msg, sizeof(MSG), 0);
	
	return;
}

//查找单词
int  do_searchword(int connectfd, MSG *msg)
{
	FILE *fp;
	char temp[300];
	char *p;
	int len, result;
	
	//得到要查找单词的长度
	len = strlen(msg->data);

	//打开文档
	if((fp = fopen("dict.txt", "r")) == NULL)
	{
		strcpy(msg->data, "dict can not open");
		send(connectfd, msg, sizeof(MSG), 0);
	}

	printf("query word is %s len = %d\n", msg->data, len);
	
	//fgets每次只可以读一行，规定文档里面最后一行有300个字节
	while(fgets(temp, 300, fp) != NULL)
	{
		//usleep(10);
		result = strncmp(msg->data, temp, len);
		
		//排除单词短的情况
		//比如查找break  但是后bre这个单词
		if(result > 0) 
		{
			continue;
		}

		//排除单词长的情况以及单词后不跟空格的情况
		if(result < 0 || temp[len] != ' ')
		{
			break;
		}

		//指针指向单词后面的位置
		p = temp + len;

		//从第一个有数据的位置开始读，排除单词与解释之间的空格
		while(*p == ' ')
		{
			p++;
		}

		strcpy(msg->data, p);
		
		fclose(fp);
		
		puts("return  1");

		return 1;
	}

	fclose(fp);
	return 0;
}

//查询历史记录
void do_history(int connectfd, MSG *msg, sqlite3 *db)
{
	char sqlstr[128], *errmsg;

	sprintf(sqlstr, "select * from record where name = '%s'", msg->name);

	if (sqlite3_exec(db, sqlstr, history_callback, (void *)&connectfd, &errmsg) != SQLITE_OK)
	{
		printf("error : %s\n", errmsg);
		sqlite3_free(errmsg);
	}
	
	msg->data[0] = '\0';
	
	send(connectfd, msg, sizeof(MSG), 0);
	
	return;
}

//回调函数
int history_callback(void *arg, int f_num, char **f_value, char **f_name)
{
	int connectfd;
	MSG msg;
	
	connectfd = *(int *)arg;
	
	sprintf(msg.data, "%s : %s", f_value[1], f_value[2]);
	
	send(connectfd, &msg, sizeof(msg), 0);
	
	return 0;
}

//获取时间
void getdata(char data[])
{
	time_t t;
	struct tm *tp;
	time(&t);
	tp = localtime(&t);
	
	//将数据保存在data里面
	sprintf(data, "%d-%d-%d %d:%d:%d", 
		tp->tm_year + 1900, tp->tm_mon + 1, tp->tm_mday, 
		tp->tm_hour, tp->tm_min, tp->tm_sec);

	return ;
}

//查询
void do_query(int connectfd, MSG *msg, sqlite3 *db)
{
	puts("query");
	char sqlstr[128], *errmsg;
	int found = 0;
	char date[128], word[128];
	
	strcpy(word, msg->data);
	
	//查询成功返回1
	found =do_searchword(connectfd, msg);

	if(found)
	{
		//时间
		getdata(date);
		
		sprintf(sqlstr, "insert into record values('%s', '%s', '%s')", msg->name, date, word);
		
		if(sqlite3_exec(db, sqlstr, NULL, NULL, &errmsg) != SQLITE_OK)
		{
			printf("error : %s\n", errmsg);
		}
	}
	else
	{
		strcpy(msg->data, "not found");
	}
	send(connectfd, msg, sizeof(MSG), 0);
	
	return;
}

void do_client(int connectfd, sqlite3 *db)
{
	MSG msg;

	//根据接收到的type判断对应执行的代码
	while (recv(connectfd, &msg, sizeof(MSG), 0) > 0)  // receive request
	{
		printf("type = %d\n", msg.type);
		printf("type = %s\n", msg.data);
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
