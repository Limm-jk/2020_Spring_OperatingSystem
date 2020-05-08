#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE	1337
#define FIFO_FILE "./fifo"
int main(void)
{
	int cnt = 0;
	int fd;
	int score = 100;
	char buf[BUF_SIZE];
	char pingstr[] = "ping";
	sem_t* sem;
	int* ptr;

	mkfifo(FIFO_FILE,0666);
	fd = open(FIFO_FILE, O_RDWR|O_CREAT,S_IRWXU);
	//ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd,0);

	sem_unlink("mysema");

	if((sem = sem_open("mysema", O_CREAT, 0644, 1)) == SEM_FAILED){
		perror("open");
		exit(1);
	}

	/*
	 * To-do : Round1 ping
	 * without [opponent] string
	 */
	sem_wait(sem);
	printf("Your turn!\n");
	scanf("%s",buf);
	write(fd, buf, strlen(buf));

	if (strcmp(buf, pingstr)){
		printf("wrong! -20\n");
		score -= 20;
	}
	sem_post(sem);
	sleep(1);

	for (cnt=1; cnt<5; cnt++)
	{
		sem_wait(sem);
		memset(buf, 0x00, BUF_SIZE);
		read(fd, buf, BUF_SIZE);
		printf("[opponent] %s\n", buf);
		printf("Your turn!\n");
		memset(buf, 0x00, BUF_SIZE);
		/*
		 * To-do : complete game process
		 */
		scanf("%s",buf);
		write(fd, buf, strlen(buf));
		if (strcmp(buf, pingstr))
		{
			/*
			 * To-do : handle wrong case
			 */
			printf("wrong! -20\n");
			score -= 20;
		}
		sem_post(sem);
		sleep(1);
	}

	printf("Done! Your score : %d\n", score);

	close (fd);

	return 0;
	
}
