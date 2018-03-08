#include <stdio.h>
#include <pthread.h>
#include "../alsa_test.h"

extern int confirmed_key[10];
extern int new_key_is_pushed;
static int key[10];
static int last_key[10];

extern pthread_mutex_t key_scan;
static pthread_t snd[10];

static int read_key(void);
static void start_play(void);
static void stop_last_play(void *arg);

static void *snd_thread_fn(void *arg)
{
    int key = (int)arg;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    pthread_cleanup_push(stop_last_play, NULL);
    switch (key) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        openAudio("../audio/DJI.wav");
        break;
    default:
        printf("no this key!\n");
    }
    pthread_cleanup_pop(0);
    return ((void *)0);
}

void *play_thread_fn(void *arg)
{
    while (1) {
        if(!read_key())
            start_play();
    }
}

static int read_key(void)
{
    if (new_key_is_pushed) {
        pthread_mutex_lock(&key_scan);
        for (int i = 1; i <= 9; i++)
            key[i] = confirmed_key[i];
        new_key_is_pushed = 0;
        pthread_mutex_unlock(&key_scan);

        return 0;
    } else {
        return -1;
    }
}

static void start_play(void)
{
    for (int i = 1; i <= 9; i++) {
        if (key[i]) {
            if (last_key[i]) {
                pthread_cancel(snd[i]);
                pthread_join(snd[i],NULL);
                pthread_create(&snd[i], NULL, snd_thread_fn, (void *)i);
            } else {
                pthread_create(&snd[i], NULL, snd_thread_fn, (void *)i);
            }
        }
        last_key[i] = key[i];
    }
}

static void stop_last_play(void *arg)
{
    stopAudio();
}
