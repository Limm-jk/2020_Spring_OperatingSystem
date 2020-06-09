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
#define FIFO_FILE1 "./file1"//cl to ser
#define FIFO_FILE2 "./file2"//ser to cl

int play_369(int num);

int cl1_pid = -1;
int cl2_pid = -1;
int cur_pid = -1;
int game = 0;

int main(void)
{
	int cnt = 0;
	int fd1;
	int fd2;
	char buf[BUF_SIZE];
	char pid_buf[BUF_SIZE];
	char pingstr[] = "pong";
	sem_t* sem1;
	sem_t* sem2;
	char* ptr;
	char* ptr2;
	


	printf("Please wait until server is ready...");

	mkfifo(FIFO_FILE1,0666);
	mkfifo(FIFO_FILE2,0666);

/* file open */	
	fd1 = open(FIFO_FILE1, O_RDWR|O_CREAT,S_IRWXU);
	if(ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0) == -1){
		perror("mmap error");
		exit(1);
	}
	fd2 = open(FIFO_FILE2, O_RDWR|O_CREAT,S_IRWXU);
	if(ptr2 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0) == -1){
		perror("mmap error");
		exit(1);
	} 
/* sema open */
	if((server = sem_open("server", O_CREAT, 0777, 1)) == SEM_FAILED){
		perror("open");
		exit(1);
	}
	if((client = sem_open("client", O_CREAT, 0777, 1)) == SEM_FAILED){
		perror("open");
		exit(1);
	}
	printf("Done \n");
	//client connect
	int cli_input = 1;

	while(cli_input){
		sem_wait(client);
		memset(buf, 0x00, BUF_SIZE);
		if(read(fd1, buf, BUF_SIZE)>=1){
			if(cl1_pid == -1){
				cl1_pid = atoi(buf);
				sprintf(buf,"first");
			}
			else if(cl2_pid == -1){
				cl2_pid = atoi(buf);
				sprintf(buf,"second");
				cli_input = 0;
			}
		write(fd2, buf, strlen(buf));
		}
		sem_post(client);
	}
	printf("Server %d is ready for play!\n", getpid());
	printf("Client %d hi with slot 0 \n", cl1_pid);
	printf("Client %d hi with slot 1 \n", cl2_pid);

	cur_pid = cl1_pid;
	while(1){
		game++;
		if(cur_pid == cl1_pid) cur_pid = cl2_pid;
		memset(buf, 0x00, BUF_SIZE); 
		char inp = read(fd1, buf, BUF_SIZE);
		result = play_369(atoi(inp));
		if(result != 1) {
			write(fd2, pid_buf, strlen(buf));
			break;
		}
	}
	sleep(3);
	printf("Player %d lose!", cur_pid);


	return 0;
}
int play_369(int num){
	int cur = game;
	int check;
	int status = game;
	while(1){
		check = cur%10;
		if(check == 3 || check == 6 || check == 9){
			status = -1;
			break;
		}
		cur /= 10;
		if(cur == 0) break;
	}
	if(status != game) return -1;
	else return 1;
}
