#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

struct shmbuf{
	int a;
	char b;
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

	if((shm = shmat(shmid, NULL, 0)) > 0){
		printf("shm = %p\n", shm);
	}

	printf("a = %d\n", shm->a);
	printf("b = %d\n", shm->b);


	if(shmdt(shm) < 0){
		perror("fail to shmdt");
		return -1;
	}

	shmctl(shmid, IPC_RMID, NULL);
	system("ipcs -m");
	return 0;
}
