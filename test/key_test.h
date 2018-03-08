/*************************************************************************
 > File Name: key_test.h
 > Author: 
 > Email: 
 > Created Time: Wed 07 Mar 2018 20:05:47 CST
************************************************************************/

#ifndef __KEY_TEST_H__
#define __KEY_TEST_H__ 

#include <stdio.h>
#include <wiringPi.h>

#define PULL_UP_ALL    3

void gpioInit(void);
void pullUp(int column);
int rowScan(int row);
int keyboardScan(int *key);

#endif

