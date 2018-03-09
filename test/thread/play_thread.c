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

char p_do[]={"../audio/0.5s/do.wav"};
char p_re[]={"../audio/0.5s/re.wav"};
char p_mi[]={"../audio/0.5s/mi.wav"};;
char p_fa[]={"../audio/0.5s/fa.wav"};
char p_so[]={"../audio/0.5s/so.wav"};
char p_la[]={"../audio/0.5s/la.wav"};
char p_si[]={"../audio/0.5s/si.wav"};


static void *snd_thread_fn(void *arg)
{
    int key = (int)arg;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    pthread_cleanup_push(stop_last_play, NULL);
    switch (key) {
    case 1:
        openAudio(p_do);
        break;
    case 2:
        openAudio(p_re);
        break;
    case 3:
        openAudio(p_mi);
        break;
    case 4:
        openAudio(p_fa);
        break;
    case 5:
        openAudio(p_so);
        break;
    case 6:
        openAudio(p_la);
        break;
    case 7:
        openAudio(p_si);
        break;
    case 8:
    case 9:
        openAudio("../audio/1s/do.wav");
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
