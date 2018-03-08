/*************************************************************************
 > File Name: main.c
 > Author: 
 > Email: 
 > Created Time: Wed 07 Mar 2018 17:24:59 CST
************************************************************************/

#include "key_test.h" 
#include "alsa_test.h"


char audio_start[]={"../audio/DJI.wav"};


int main(int argc, char const *argv[])
{
    int i;
    int is_press;
    int key[10];

    wiringPiSetup();
    gpioInit();
    openAudio(audio_start);

    while(1){

        is_press = keyboardScan(key);

        if(!is_press) continue;

        for(i=1;i<=9;i++){
            printf("%d",key[i]);
        }
        printf("\n");
    }

    return 0;
}

