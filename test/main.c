/*************************************************************************
 > File Name: main.c
 > Author: 
 > Email: 
 > Created Time: Wed 07 Mar 2018 17:24:59 CST
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "key_test.h"
#include "alsa_test.h"
#include "thread/key_thread.h"
#include "thread/play_thread.h"

int main(int argc, char const *argv[])
{
    int ret;
    ret = pthread_create(NULL, NULL, key_thread_fn, NULL);
    if (ret != 0) {
        printf("create key thread failed!\n");
        exit(-1);
    }
    ret = pthread_create(NULL, NULL, play_thread_fn, NULL);
    if (ret != 0) {
        printf("create play thread failed!\n");
        exit(-1);
    }
    while(1){
    }

    return 0;
}

