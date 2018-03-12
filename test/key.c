/*************************************************************************
 > File Name: key_test.c
 > Author: 
 > Email: 
 > Created Time: Fri 02 Mar 2018 14:25:09 CST
************************************************************************/

#include "key.h"


/* 各行列对应 wiringPi 引脚
 * e.g. 第一列C[0] 对应引脚 wPi pin_25
 * Notes: 需要根据实际接线修改 */
int C[3]={25, 24, 23};
int R[3]={29, 28, 27};


/* gpioInit
 * brief: GPIO初始化
 * args : void
 * rtn  : void
 * */
void gpioInit(void)
{
    int i;
    for(i=0;i<3;i++){
        pinMode(C[i], OUTPUT);
        pinMode(R[i], INPUT);
    }
}


/* pullUp
 * brief: 列输出上拉
 * args : column: 列号C[0]-C[2] 或者 PULL_UP_ALL 拉高所有列
 * rtn  : void
 * */
void pullUp(int column)
{
    int i;
    if(column == PULL_UP_ALL){
        for(i=0;i<3;i++){
            digitalWrite(C[i], HIGH);
            delay(2);
        }
        return;
    }
    for(i=0;i<3;i++){
        digitalWrite(C[i], LOW);
        delay(2);
    }
    digitalWrite(column, HIGH);
}


/* rowScan
 * brief: 行扫描并给出该行按键情况
 * args : row: 行号R[0]-R[2}
 * rtn  : key: 该行按键情况
 *             e.g.:
 *                 000 无按键
 *                 100 该行第一列有按下
 *                 110 该行第一列第二列均有按下
 * */
int  rowScan(int row)
{
    int key = 0;

    pullUp(PULL_UP_ALL);
    if(digitalRead(row) == HIGH){
        pullUp(C[0]);
        if(digitalRead(row) == HIGH) key |= 0x100;
        pullUp(C[1]);
        if(digitalRead(row) == HIGH) key |= 0x010;
        pullUp(C[2]);
        if(digitalRead(row) == HIGH) key |= 0x001;
    }
    return key;
}


/* keyboardScan
 * brief: 返回按键情况
 * args:  key: 指向按键数组
 * rtn:   0 没有按键按下
 *        1 有按键按下
 * */
int keyboardScan(int *key)
{
    int i;
    int r1[3];
    int r2[3];
    int same = 0;

    static int pre_r[3];

    for(i=0;i<9;i++){
        key[i] = 0;
    }

    for(i=0;i<3;i++){
        r1[i] = rowScan(R[i]);
    }
    delay(10);
    for(i=0;i<3;i++){
        r2[i] = rowScan(R[i]);
        r1[i] &= r2[i];
    }

    if((r1[0]==0)&&(r1[1]==0)&&(r1[2]==0)){
        for(i=0;i<3;i++){
            pre_r[i] = 0;
        }
        return 0;
    }

    for(i=0;i<3;i++){
        if(pre_r[i] == r1[i]) same++;
    }

    for(i=0;i<3;i++){
        pre_r[i] = r1[i];
    }
    if(same == 3) return 0;
    
    for(i=0;i<3;i++){
        if(r1[i]&0x100) key[0+i*3] = 1;
        if(r1[i]&0x010) key[1+i*3] = 1;
        if(r1[i]&0x001) key[2+i*3] = 1;
    }

    return 1;
}


/* test_main
 * */
//int main(void)
//{
//    int i;
//    int key[9];
//    int press;
//
//    wiringPiSetup();
//    gpioInit();
//
//    while(1){
//
//        press = keyboardScan(key);
//        if(!press) continue; 
//        printf("----------\n");
//        for(i=0;i<9;i++){
//            printf("%d",key[i]);
//        }
//        printf("\n");
//    }
//}

