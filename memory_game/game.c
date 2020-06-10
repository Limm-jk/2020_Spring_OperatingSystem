#include<stdio.h>
#include<stdlib.h>
#include<wiringPi.h>
#include<stdlib.h>
#include<time.h>

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

int main(void){
    int level = 0;
    int mem[5];


    init();

    //start~
    start();

    while(level < 5){
        level++;

        srand(time(NULL));
        for(int i = 0; i<=level; i++){
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

    // init Thread
    pthread_t pt[4];
    int status;
    sem_init(&input, 0, 0)
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
