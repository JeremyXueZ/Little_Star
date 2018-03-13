/*************************************************************************
 > File Name: main.c
 > Author:
 > Email:
 > Created Time: Wed 07 Mar 2018 17:24:59 CST
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "inc/key.h"
#include "inc/tinyalsa.h"
#include "inc/key_thread.h"
#include "inc/play_thread.h"

int main(int argc, char const *argv[])
{
    pthread_t tid1, tid2;
    int ret;

    openAudio(8);

    ret = pthread_create(&tid1, NULL, key_thread_fn, NULL);
    if (ret != 0) {
        printf("create key thread failed!\n");
        exit(-1);
    }
    ret = pthread_create(&tid2, NULL, play_thread_fn, NULL);
    if (ret != 0) {
        printf("create play thread failed!\n");
        exit(-1);
    }
    while(1){
    }

    return 0;
}
