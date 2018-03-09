#include <stdio.h>
#include <pthread.h>
#include "../key_test.h"

/* 用于存储按键扫描的结果，确认无误的放入confirm数组中，传递给play线程，并将新按键按下标志置1 */
static int key[10];
int confirmed_key[10];
int new_key_is_pushed = 0;

/* 用于确保按键扫描结果传递安全的互斥量 */
pthread_mutex_t key_scan_lock = PTHREAD_MUTEX_INITIALIZER;

/* 内部函数声明 */
static void send_key(void);

/* key_thread_fn
 * brief: 按键扫描线程
 * args:  arg: 线程入口函数的固定参数，如果有需要转换成相应的数据类型读取
 * */
void *key_thread_fn(void *arg)
{
    wiringPiSetup();
    gpioInit();

    while (1) {
        if (!keyboardScan(key)) continue;
        send_key();
    }
}

/* send_key
 * brief: 发送按键扫描的结果，用互斥锁保证数据安全
 * */
static void send_key(void)
{
    pthread_mutex_lock(&key_scan_lock);
    for (int i = 1; i <= 9; i++)
        confirmed_key[i] = key[i];
    new_key_is_pushed = 1;
    pthread_mutex_unlock(&key_scan_lock);
}
