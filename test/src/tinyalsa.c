/*************************************************************************
 > File Name: alsa.c
 > Author:
 > Email:
 > Created Time: Tue 06 Mar 2018 10:57:44 CST
************************************************************************/

#include "../inc/tinyalsa.h"

/* 声卡设备定义，PCM设置 */
unsigned int Card = 0;
unsigned int Device = 0;
int Flags = PCM_OUT;

const struct pcm_config Config = {
    .channels = 2,
    .rate = 48000,
    .format = PCM_FORMAT_S16_LE,
    .period_size = 1024,
    .period_count = 2,
    .start_threshold = 1024,
    .silence_threshold = 1024*2,
    .stop_threshold = 1024*2
};

/* pcm接口，帧空间，文件路径数组，使用1-9，0无用 */
static struct pcm *pcm[10];
static void *frames[10];
static char audio_path[10][30] = {"../audio/DJI.wav",
                                  "../audio/0.5s/do.wav",
                                  "../audio/0.5s/re.wav",
                                  "../audio/0.5s/mi.wav",
                                  "../audio/0.5s/fa.wav",
                                  "../audio/0.5s/so.wav",
                                  "../audio/0.5s/la.wav",
                                  "../audio/0.5s/si.wav",
                                  "../audio/DJI.wav",
                                  "../audio/DJI.wav",
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
 * args:  key: 目标音频文件编号
 * rtn:   size: 文件大小
 * */
static size_t read_file(int key)
{
    FILE * input_file = fopen(audio_path[key], "r");
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

    frames[key] = malloc(size);
    if (frames[key] == NULL) {
        fprintf(stderr, "failed to allocate frames\n");
        fclose(input_file);
        return 0;
    }

    size = fread(frames[key], 1, size, input_file);

    fclose(input_file);

    return size;
}


/* write_frames
 * brief: 帧写入
 * args: byte_count: 字节数
 *       key: 目标音频文件编号
 * rtn:  0：写入成功
 *      -1：写入失败
 * */
static int write_frames(size_t byte_count, int key)
{
    pcm[key] = pcm_open(Card, Device, Flags, &Config);
    if (pcm[key] == NULL) {
        fprintf(stderr, "failed to allocate memory for PCM\n");
        return -1;
    } else if (!pcm_is_ready(pcm[key])){
        pcm_close(pcm[key]);
        fprintf(stderr, "failed to open PCM\n");
        return -1;
    }

    unsigned int frame_count = pcm_bytes_to_frames(pcm[key], byte_count);
    pcm_writei(pcm[key], frames[key], frame_count);

    pcm_close(pcm[key]);

    return 0;
}


/* openAudio
 * brief: 打开音频文件，.wav 格式
 * args:  目标音频文件编号
 * rtn:   1: 打开成功
 *        0: 打开失败
 * */
int openAudio(int key)
{
    size_t size;

    size = read_file(key);
    printf("audio file size:%d\n",size);
    if (size == 0) {
        printf("failed to read file\n");
        return 0;
    }

    if (write_frames(size, key) < 0) {
        printf("failed to write frames\n");
        return 0;
    }

    if (frames[key] != NULL) {
        free(frames[key]);
        frames[key] = NULL;
    }
    return 1;
}


/* stopAudio
 * brief: 关闭正在播放的音频文件，.wav 格式
 * args:  目标音频文件编号
 * */
void stopAudio(int key)
{
    if (frames[key] != NULL) {
        free(frames[key]);
        frames[key] = NULL;
    }

    if (pcm[key] == NULL) {
        return;
    } else {
        pcm_close(pcm[key]);
    }
}

