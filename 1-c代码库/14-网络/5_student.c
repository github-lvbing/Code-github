#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

#define DATABASE "student.db"

#define N 128

int flag = 0;

int do_insert(sqlite3 *db)
{
	int id;
	char name[N] = {};
	int score;
	char sql[N] = {};
	char *errmsg;

	printf("input id >>> ");
	scanf("%d", &id);
	getchar();

	printf("input name >>> ");
	scanf("%s", name);
	getchar();

	printf("input score >>> ");
	scanf("%d", &score);
	getchar();

	sprintf(sql, "insert into stu values(%d, '%s', %d)", id, name, score);

	if(sqlite3_exec(db, sql, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("数据插入成功\n");
	}

	return 0;
}

int callback(void *arg, int ncolumns, char **f_value, char **f_name)
{
	int i = 0;

	if(flag == 0)
	{
		for(i = 0; i < ncolumns; i++)
		{
			printf("%-11s", f_name[i]);
		}

		flag = 1;

		putchar(10);
	}

	for(i = 0; i < ncolumns; i++)
	{
		printf("%-11s", f_value[i]);
	}
	putchar(10);

	return 0;
}

int do_select(sqlite3 *db)
{
	char sql[N] = {};
	char *errmsg;
	strcpy(sql, "select * from stu");

	if(sqlite3_exec(db, sql, callback, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("查询完毕\n");
	}

	return 0;
}

int main(int argc, const char *argv[])
{
	sqlite3 *db;
	char *errmsg;
	int cmd;

	//创建或者打开一个数据库
	if(sqlite3_open(DATABASE, &db) != SQLITE_OK)
	{
		printf("%s\n", sqlite3_errmsg(db));
	}
	else
	{
		printf("the database is open done\n");
	}

	//创建一张表
	if(sqlite3_exec(db, "create table stu(id int, name char, score int)", NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("create table is success\n");
	}

	while(1)
	{
		printf("*********************************************************\n");
		printf("*1：插入数据 2：查询数据 3：删除数据 4：修改数据 5：关闭*\n");
		printf("*********************************************************\n");
		printf("please input >>> ");

		scanf("%d", &cmd);
		getchar();

		switch(cmd)
		{
		case 1:
			do_insert(db);
			break;
		case 2:
			flag = 0;
			do_select(db);
			break;
		case 3:
			//do_delete();
			break;
		case 4:
			//do_change();
			break;
		case 5:
			exit(0);
		default:
			printf("请输入正确的选项\n");
		}
	}
	
	return 0;
}
