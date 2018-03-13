#include <stdio.h>
#include <pthread.h>
#include "../inc/tinyalsa.h"

/* 声明于key_thread.c用于传递按键扫描结果 */
extern int confirmed_key[9];
extern int new_key_is_pushed;
extern pthread_mutex_t key_scan_lock;

/* 内部存储按键扫描结果，以及上一次的播放是否结束（用于覆盖前一次播放的音频） */
static int key[9];
static int last_playing[9];

/* 播放对应按键音频文件的线程 */
static pthread_t snd[9];

/* 内部函数声明 */
static int read_key(void);
static void start_play(void);
static void stop_last_play(void *arg);

/* snd_thread_fn
 * brief: 内部播放音频线程
 * args:  arg: 线程入口函数的固定参数，如果有需要转换成相应的数据类型读取，这里传递音频编号key
 * */
static void *snd_thread_fn(void *arg)
{
    int key = (int)arg;

    //在音乐覆盖时，中止线程前清理动态内存空间
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_cleanup_push(stop_last_play, (void *)key);

    //播放对应的音频文件
    if ((0 <= key) && (key <= 8)) {
        openAudio(key);
    } else {
        printf("no this key!\n");
    }

    pthread_cleanup_pop(0);

    //上一次播放结束，清除标志
    last_playing[key] = 0;
    return ((void *)0);
}

/* play_thread_fn
 * brief: 处理按键扫描结果，选择播放音频的线程
 * args : arg: 线程入口函数的固定参数，如果有需要转换成相应的数据类型读取
 * rtn  : void
 * */
void *play_thread_fn(void *arg)
{
    while (1) {
        if(!read_key())
            start_play();
    }
}

/* read_key
 * brief: 接收按键扫描的结果，用互斥锁保证数据安全
 * args : void
 * rtn  : 有新按键返回0
 *        没有返回-1
 * */
static int read_key(void)
{
    if (new_key_is_pushed) {
        pthread_mutex_lock(&key_scan_lock);
        for (int i = 0; i < 9; i++)
            key[i] = confirmed_key[i];
        new_key_is_pushed = 0;
        pthread_mutex_unlock(&key_scan_lock);

        return 0;
    } else {
        return -1;
    }
}

/* start_play
 * brief: 接根据按键结果选择播放文件，创建相应的播放线程
 * args : void
 * rtn  : void
 * */
static void start_play(void)
{
    for (int i = 0; i < 9; i++) {
        if (key[i]) {
            if (last_playing[i]) {//上一次播放未结束，则中止上一次播放
                pthread_cancel(snd[i]);
                pthread_join(snd[i],NULL);
                pthread_create(&snd[i], NULL, snd_thread_fn, (void *)i);
            } else {
                pthread_create(&snd[i], NULL, snd_thread_fn, (void *)i);
            }
        }
        last_playing[i] = 1;
    }
}

/* stop_last_play
 * brief: 在音乐覆盖时，中止线程前清理动态内存空间
 * args : 固定参数，如果有需要转换成相应的数据类型读取，这里传递音频编号key
 * rtn  : void
 * */
static void stop_last_play(void *arg)
{
    int key = (int)arg;
    stopAudio(key);
}
