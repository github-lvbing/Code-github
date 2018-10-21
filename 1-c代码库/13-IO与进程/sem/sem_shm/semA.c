#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/sem.h>

#define N 128
struct shmbuf{
	char buf[N];
};

union semun{
	int val;
};
int main(int argc, char *argv[])
{
	key_t key;

	if((key = ftok(".", 'a')) < 0){
		perror("fail to ftok");
		return -1;
	}

	int shmid;
	struct shmbuf *shm;
	if((shmid = shmget(key, 512, IPC_CREAT|IPC_EXCL|0664)) < 0){
		if(errno != EEXIST){
			perror("fail to shmget");
			return -1;
		}
		else{
			shmid = shmget(key, 512, 0664);
		}
	}

	if((shm = shmat(shmid, NULL, 0)) < 0){
		perror("fail to shmat");
		return -1;
	}

	int semid;
	union semun mysemun;
	struct sembuf sem;
	semid = semget(key, 2, IPC_CREAT|IPC_EXCL|0664);

	if(semid < 0){
		if(errno != EEXIST){
			perror("fail to semget");
			return -1;
		}
		else{
			semid = semget(key, 2, 0664);
		}
	}
	else{
		//读操作信号量
		mysemun.val = 0;
		semctl(semid, 0, SETVAL, mysemun);
		//写操作信号量
		mysemun.val = 1;
		semctl(semid, 1, SETVAL, mysemun);
	}

	while(1){
		//申请写信号量，此时值为1
		sem.sem_num = 1;
		sem.sem_op = -1;
		sem.sem_flg = 0;
		semop(semid, &sem, 1);

		fgets(shm->buf, N, stdin);

		//释放读信号量
		sem.sem_num = 0;
		sem.sem_op = 1;
		sem.sem_flg = 0;
		semop(semid, &sem, 1);

		if(strncmp(shm->buf, "quit", 4) == 0){
			goto ERROR;
		}
	}
ERROR:
	shmdt(shm);
	return 0;
}
