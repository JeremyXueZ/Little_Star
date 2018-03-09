/*************************************************************************
 > File Name: alsa_test.h
 > Author:
 > Email:
 > Created Time: Wed 07 Mar 2018 17:21:43 CST
************************************************************************/
#ifndef __ALSA_TEST_H__
#define __ALSA_TEST_H__

#include <stdio.h>
#include <stdlib.h>
#include "/usr/local/tinyalsa/include/tinyalsa/pcm.h"


int openAudio(int key);
void stopAudio(int key);

#endif
