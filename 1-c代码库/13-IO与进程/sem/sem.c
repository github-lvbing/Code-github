#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

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

	int semid;
	union semun mysemun;
	if((semid = semget(key, 2, IPC_CREAT|IPC_EXCL|0664)) < 0){
		if(errno != EEXIST){
			perror("fail to semget");
			return -1;
		}
		else{
			semid = semget(key, 2, 0664);
		}
	}

	mysemun.val = 10;
	if(semctl(semid, 0, SETVAL, mysemun) < 0){
		perror("fail to semctl");
		return -1;
	}
	if(semctl(semid, 0, GETVAL, mysemun) > 0){
		printf("val = %d\n", mysemun.val);
	}

	mysemun.val = 5;
	if(semctl(semid, 1, SETVAL, mysemun) < 0){
		perror("fail to semctl");
		return -1;
	}
	if(semctl(semid, 1, GETVAL, mysemun) > 0){
		printf("val = %d\n", mysemun.val);
	}

	return 0;
}
