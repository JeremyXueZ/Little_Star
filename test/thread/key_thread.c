#include <stdio.h>
#include <pthread.h>
#include "../key_test.h"

static int key[10];
int confirmed_key[10];
int new_key_is_pushed = 0;

pthread_mutex_t key_scan = PTHREAD_MUTEX_INITIALIZER;

static void send_key(void);

void *key_thread_fn(void *arg)
{
    wiringPiSetup();
    gpioInit();

    while (1) {
        if (!keyboardScan(key)) continue;
        send_key();
    }
}

static void send_key(void)
{
    pthread_mutex_lock(&key_scan);
    for (int i = 1; i <= 9; i++)
        confirmed_key[i] = key[i];
    new_key_is_pushed = 1;
    pthread_mutex_unlock(&key_scan);
}
