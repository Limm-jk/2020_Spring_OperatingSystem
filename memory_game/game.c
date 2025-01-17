#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>
#include<stdlib.h>
#include<time.h>
#include<semaphore.h>
#include <pthread.h>

#define LED_R 3
#define LED_Y 2
#define LED_G 0
#define SW_R 6
#define SW_Y 5
#define SW_G 4
#define SW_W 27

void init(void);
void off(void);
void start(void);
void fail(void);

// Thread
void *Memorize(void *arg);
void *clk_R(void *arg);
void *clk_Y(void *arg);
void *clk_G(void *arg);
void *clk_W(void *arg);

int mem[5]; // memory arr
int clk_arr[5];
int clk_count = 0
int level = 0; // game level
int game_status = 0;

sem_t input;
sem_t memory;

int main(void){
    
    init();

    // init Thread
    pthread_t pt[5];
    int status;

    sem_init(&input, 0, 0);
    sem_init(&memory, 0, 0);

    if(pthread_create(&pt[0],NULL,Memorize,NULL)){
		printf("Set memory fail");
	}
    if(pthread_create(&pt[1],NULL,clk_R,NULL)){
		printf("Red button fail");
	}
    if(pthread_create(&pt[2],NULL,clk_Y,NULL)){
		printf("Yellow button fail");
	}
    if(pthread_create(&pt[3],NULL,clk_G,NULL)){
		printf("Green button fail");
	}
    if(pthread_create(&pt[4],NULL,clk_W,NULL)){
		printf("White button fail");
	}

    pthread_join(pt[0], NULL); 
	pthread_join(pt[1], NULL);
	pthread_join(pt[2], NULL);
	pthread_join(pt[3], NULL);
    pthread_join(pt[4], NULL);



}

void Memorize(void){

    start();

    while(level < 5){
        level++;
        srand(time(NULL));
        for(int i = 0; i<level; i++){
            mem[i] = (rand() % 3) + 1;
            switch(mem[i]){
                case 1:
                    digitalWrite(LED_R, 1);
	                delay(250);
	                digitalWrite(LED_R, 0);
                    break;
                case 2:
                    digitalWrite(LED_Y, 1);
	                delay(250);
	                digitalWrite(LED_Y, 0);
                    break;
                case 3:
                    digitalWrite(LED_G, 1);
	                delay(250);
	                digitalWrite(LED_G, 0);
                    break;
            }
        }
        sem_post(&input);
        sem_wait(&memory);
        if(game_status){
            break;
        }
    }
    if(game_status){
        fail();
    }
    else{
        start();
    }
}

void clk_R(void){
    int val;
    while(1){
        sem_getvalue(&input, &val)
        if(digitalRead(SW_R) == 0 & sem_getvalue != 0){
            digitalWrite(LED_R, 1);
            if(clk_count < level){
                clk_arr[clk_count] = 1;
                clk_count++;
            }
            delay(250);
	        digitalWrite(LED_R, 0);
        }
    }
}
void clk_Y(void){
    int val;
    while(1){
        sem_getvalue(&input, &val)
        if(digitalRead(SW_Y) == 0 & sem_getvalue != 0){
            digitalWrite(LED_Y, 1);
            if(clk_count < level){
                clk_arr[clk_count] = 2;
                clk_count++;
            }
            delay(250);
	        digitalWrite(LED_Y, 0);
        }
    }
}
void clk_G(void){
    int val;
    while(1){
        sem_getvalue(&input, &val)
        if(digitalRead(SW_G) == 0 & sem_getvalue != 0){
            digitalWrite(LED_G, 1);
            if(clk_count < level){
                clk_arr[clk_count] = 3;
                clk_count++;
            }
            delay(250);
	        digitalWrite(LED_G, 0);
        }
    }
}
void clk_W(void){
    int val;
    while(1){
        sem_getvalue(&input, &val)
        if(digitalRead(SW_R) == 0 & sem_getvalue != 0){
            sem_wait(&input);
            for(int i = 0; i < level; i++){
                if(mem[i] != clk_arr[i]){
                    game_status = 1;
                    break;
                }
            }
            sem_post(&memory);
        }
    }
}


void init(void){
    if(wiringPiSetup() == -1){
	puts("Setup Fail");
	exit(1);
    }
    pinMode(SW_R, INPUT);
    pinMode(SW_Y, INPUT);
    pinMode(SW_G, INPUT);
    pinMode(SW_W, INPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_Y, OUTPUT);
    pinMode(LED_G, OUTPUT);
    off();


}

void off(void){
    digitalWrite(LED_R, 0);
    digitalWrite(LED_Y, 0);
    digitalWrite(LED_G, 0);
}

void start(void){
    int i = 0;

    while(i<3){
	digitalWrite(LED_R, 1);
	delay(250);
	digitalWrite(LED_R, 0);
	
	digitalWrite(LED_Y, 1);
	delay(250);
	digitalWrite(LED_Y, 0);
	
	digitalWrite(LED_G, 1);
	delay(250);
	digitalWrite(LED_G, 0);

	i++;
    }
}

void fail(void){
    int i = 0;

    while(i<3){
	digitalWrite(LED_R, 1);
    digitalWrite(LED_Y, 1);
    digitalWrite(LED_G, 1);

	delay(250);

	digitalWrite(LED_R, 0);
	digitalWrite(LED_Y, 0);
	digitalWrite(LED_G, 0);

	i++;
    }
}
