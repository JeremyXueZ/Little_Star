/*************************************************************************
 > File Name: key_test.c
 > Author: chen
 > Email: stevenchen49@163.com
 > Created Time: Fri 02 Mar 2018 14:25:09 CST
************************************************************************/

#include <stdio.h>
#include <wiringPi.h>


#define PULL_UP_ALL    3


/* 各行列对应 wiringPi 引脚
 * e.g. 第一列C[1] 对应引脚 wPi pin_25 
 * Notes: 需要根据实际接线修改 */
int C[4]={0, 25, 24, 23};
int R[4]={0, 29, 28, 27};


/* gpioInit
 * brief: GPIO初始化
 * args : void
 * rtn  : void
 * */
void gpioInit(void)
{
    int i;
    for(i=1;i<=3;i++){
        pinMode(C[i], OUTPUT);
        pinMode(R[i], INPUT);
    }
}


/* pullUp
 * brief: 列输出上拉
 * args : column: 列号C[1]-C[3] 或者 PULL_UP_ALL 拉高所有列
 * rtn  : void
 * */
void pullUp(int column)
{
    int i;
    if(column == PULL_UP_ALL){
        for(i=1;i<=3;i++){
            digitalWrite(C[i], HIGH);
            delay(2);
        }
        return;
    }
    for(i=1;i<=3;i++){
        digitalWrite(C[i], LOW);
        delay(2);
    }
    digitalWrite(column, HIGH);
}


/* rowScan
 * brief: 行扫描并给出该行按键情况
 * args : row: 行号R[1]-R[3]
 * rtn  : key: 该行按键情况
 *             e.g.:
 *                 000 无按键
 *                 100 该行第一列有按下
 *                 110 该行第一列第二列均有按下
 * */
int rowScan(int row)
{
    int key = 0;
    
    pullUp(PULL_UP_ALL);
    if(digitalRead(row) == HIGH){
        pullUp(C[1]);
        if(digitalRead(row) == HIGH) key|=0x100;
        pullUp(C[2]);
        if(digitalRead(row) == HIGH) key|=0x010;
        pullUp(C[3]);
        if(digitalRead(row) == HIGH) key|=0x001;
    }
    return key;
}


/* main
 * */
int main(void)
{
    int i;
    int scan;
    int r[4] = {0,0};

    wiringPiSetup();
    gpioInit();

    while(1){

        for(i=1;i<=3;i++){
            if(digitalRead(R[i]) == HIGH){
                scan |= 1<<(i-1);
            }
        }
        if(scan != 0) delay(100);

        if(scan&0x001 == 0x001)  r[1] = rowScan(R[1]); // 扫描第一行
        if(scan&0x010 == 0x010)  r[2] = rowScan(R[2]); // 扫描第二行
        if(scan&0x100 == 0x100)  r[3] = rowScan(R[3]); // 扫描第三行

        if((r[1]==0)&&(r[2]==0)&&(r[3]==0)) continue;  // 无按键不输出

        printf("----------\n");
        for(i=1;i<=3;i++){
            printf("line_%d:%3x\n",i,r[i]);
        }
        printf("----------\n");
    }
}

