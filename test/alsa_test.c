/*************************************************************************
 > File Name: alsa.c
 > Author: 
 > Email: 
 > Created Time: Tue 06 Mar 2018 10:57:44 CST
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "/usr/local/tinyalsa/include/tinyalsa/pcm.h"


int C[4]={0, 25, 24, 23};
int R[4]={0, 29, 28, 27};


/* 音频测试文件路径定义 */
char Audio[]={"../audio/DJI.wav"};


/* 声卡设备定义，PCM设置 */
unsigned int Card = 0;
unsigned int Device = 0;
int Flags = PCM_OUT;

const struct pcm_config Config = {
    .channels = 2,
    .rate = 44050,
    .format = PCM_FORMAT_S16_LE,
    .period_size = 1024,
    .period_count = 2,
    .start_threshold = 1024,
    .silence_threshold = 1024*2,
    .stop_threshold = 1024*2
};


/* file_size 
 * brief: 读取文件大小
 * args:  file: 文件描述符
 * rtn:   file_size: 文件大小
 * */
static long int file_size(FILE * file)
{
    if (fseek(file, 0, SEEK_END) < 0) {
        return -1;
    }
    long int file_size = ftell(file);
    if (fseek(file, 0, SEEK_SET) < 0) {
        return -1;
    }
    return file_size;
}


/* read_file 
 * brief: 读取文件
 * args:  frames: 帧地址
 *        audio: 目标音频文件路径
 * rtn:   size: 文件大小
 * */
static size_t read_file(void ** frames, char *audio)
{
    FILE * input_file = fopen(audio, "r");
    if (input_file == NULL) {
        perror("failed to open file for writing");
        return 0;
    }

    long int size = file_size(input_file);
    if (size < 0) {
        perror("failed to get file size of 'audio.raw'");
        fclose(input_file);
        return 0;
    }

    *frames = malloc(size);
    if (*frames == NULL) {
        fprintf(stderr, "failed to allocate frames\n");
        fclose(input_file);
        return 0;
    }

    size = fread(*frames, 1, size, input_file);

    fclose(input_file);

    return size;
}


/* write_frames 
 * brief: 帧写入
 * args: frames: 帧空间
 *       byte_count: 字节数
 * rtn:  0：写入成功
 *      -1：写入失败
 * */
static int write_frames(const void * frames, size_t byte_count)
{
    struct pcm * pcm = pcm_open(Card, Device, Flags, &Config);
    if (pcm == NULL) {
        fprintf(stderr, "failed to allocate memory for PCM\n");
        return -1;
    } else if (!pcm_is_ready(pcm)){
        pcm_close(pcm);
        fprintf(stderr, "failed to open PCM\n");
        return -1;
    }

    unsigned int frame_count = pcm_bytes_to_frames(pcm, byte_count);

    pcm_writei(pcm, frames, frame_count);

    pcm_close(pcm);

    return 0;
}


int main(void)
{
    void *frames;
    size_t size;

    int audio_flag;

    wiringPiSetup();
    pinMode(C[1], OUTPUT);
    pinMode(R[1], INPUT);

    while(1){
        audio_flag = 0;
        if(digitalRead(R[1]) == HIGH){
            delay(100);
            if(digitalRead(R[1]) == HIGH){
                audio_flag = 1;
            }
        }

        if(audio_flag == 1){
            size = read_file(&frames, Audio);
            printf("audio file size:%d\n",size);
            if (size == 0) {
                printf("failed to read file\n");
            }

            if (write_frames(frames, size) < 0) {
                printf("failed to write frames\n");
            }

            free(frames);
        }
    }

    return 0;
}

