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
int clk_count = 0;
int level = 0; // game level
int game_status = 0;

sem_t input;
sem_t memory;

int main(void){
    int end_signal=5; 
    init();
    // init Thread
    pthread_t pt[5];
    sem_init(&input, 0, 0);
    sem_init(&memory, 0, 0);
	
    start();
   
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

    if(pthread_join(pt[0], (void **)&end_signal) == end_signal) return 0; 
    //0번스레드, 즉 memorize함수가 종료되면 프로그램 종료

    //버튼
	pthread_join(pt[1], NULL);
	pthread_join(pt[2], NULL);
	pthread_join(pt[3], NULL);
    pthread_join(pt[4], NULL);
}

void *Memorize(void *arg){
    while(level < 5){
        level++;
	    clk_count = 0;
        srand(time(NULL));
        for(int i = 0; i<level; i++){
            mem[i] = (rand() % 3) + 1;
            switch(mem[i]){
                case 1:
                    digitalWrite(LED_R, 1);
	                delay(500);
	                digitalWrite(LED_R, 0);
                    break;
                case 2:
                    digitalWrite(LED_Y, 1);
	                delay(500);
	                digitalWrite(LED_Y, 0);
                    break;
                case 3:
                    digitalWrite(LED_G, 1);
	                delay(500);
	                digitalWrite(LED_G, 0);
                    break;
            }
	    delay(200);
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

void *clk_R(void *arg){
    int val;
    while(1){
        sem_getvalue(&input, &val);
        if(digitalRead(SW_R) == 0 & val != 0){
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
void *clk_Y(void *arg){
    int val;
    while(1){
        sem_getvalue(&input, &val);
        if(digitalRead(SW_Y) == 0 & val != 0){
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
void *clk_G(void *arg){
    int val;
    while(1){
        sem_getvalue(&input, &val);
        if(digitalRead(SW_G) == 0 & val != 0){
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
void *clk_W(void *arg){
    int val;
    while(1){
        sem_getvalue(&input, &val);
        if(digitalRead(SW_W) == 0 & val != 0){
            sem_wait(&input);
            for(int i = 0; i < level; i++){
                if(mem[i] != clk_arr[i]){
                    game_status = 1;
                    break;
                }
            }
	        delay(250);
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

    delay(500);
}

void fail(void){
    int f = 0;

    while(f<3){
        digitalWrite(LED_R, 1);
        digitalWrite(LED_Y, 1);
        digitalWrite(LED_G, 1);

        delay(250);

        digitalWrite(LED_R, 0);
        digitalWrite(LED_Y, 0);
        digitalWrite(LED_G, 0);
        delay(250);
	    f++;
    }
}
