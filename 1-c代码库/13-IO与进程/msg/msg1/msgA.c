#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define N 128
#define SIZE (sizeof(struct msgbuf) - sizeof(long))

#define TYPE1 100
#define TYPE2 200
struct msgbuf{
	long mtype;
	char buf[N];
};
int main(int argc, char *argv[])
{
	key_t key;

	if((key = ftok(".", 'a')) < 0){
		perror("fail to ftok");
		return -1;
	}

	int msqid;
	struct msgbuf msg_snd, msg_rcv;
	if((msqid = msgget(key, IPC_CREAT|IPC_EXCL|0664)) < 0){
		if(errno != EEXIST){
			perror("fail to msgget");
			return -1;
		}
		else{
			msqid = msgget(key, 0664);
		}
	}
	pid_t pid;

	pid = fork();

	if(pid < 0){
		perror("fail to fork");
		return -1;
	}
	else if(pid == 0){
		msg_snd.mtype = TYPE2;
		while(1){
			fgets(msg_snd.buf, N, stdin);
			msgsnd(msqid, &msg_snd, SIZE, 0);

			if(strncmp(msg_snd.buf, "quit", 4) == 0){
				kill(getppid(), SIGKILL);
				goto ERROR;
			}
		}
	}
	else{
		while(1){
			msgrcv(msqid, &msg_rcv, SIZE, TYPE1, 0);
			
			if(strncmp(msg_rcv.buf, "quit", 4) == 0){
				kill(pid, SIGKILL);
				wait(NULL);
				break;
			}
			printf("msgB:%s\n", msg_rcv.buf);
		}
	}
ERROR:
	msgctl(msqid, IPC_RMID, NULL);
	return 0;
}
