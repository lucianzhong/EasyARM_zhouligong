#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <string.h>

#define TRIGGER				"trigger"
#define LED_PATH 			"/sys/class/leds/"   ////path //path//path//path 为 trigger triggertriggertriggertrigger路
#define LED_STATUS			"brightness"
#define TRIGGER_NONE			"none"


// C程序中操作 程序中操作 程序中操作 LEDLED ，首先需要设置 ，首先需要设置 ，首先需要设置 ，首先需要设置 ，首先需要设置 triggertriggertriggertrigger 属性。如下代码片段，将 属性。如下代码片段，将 属性。如下代码片段，将 属性。如下代码片段，将 属性。如下代码片段，将 属性。如下代码片段，将 属性。如下代码片段，将 属性。如下代码片段，将 LEDLED 灯设置为用户 灯设置为用户 灯设置为用户 灯设置为用户 控制方式 控制方式
// 然后操作 然后操作 brightness brightnessbrightness brightness brightness属性 ，设置 LEDLED 点亮或熄灭 点亮或熄灭 点
int main(int argc,char **argv)
{
	char path[20],data[2];
	int fd, ret, flag;
	if(argv[1] == NULL) {
		printf("usage : ./led led-run  or  ./led led-err");
		return 0;
	}

	strcpy(path, LED_PATH);
	strcat(path, argv[1]);
	strcat(path, "/" TRIGGER);
	fd = open(path, O_RDWR);
	if(fd < 0) {
		perror("open");
		return -1;
	}
	ret = write(fd, TRIGGER_NONE, strlen(TRIGGER_NONE));
	if(ret < 0) {
		perror("write");
		return -1;
	}
	close(fd);	
	strcpy(path, LED_PATH);
	strcat(path, argv[1]);
	strcat(path, "/" LED_STATUS);
	fd = open(path, O_WRONLY);
        if(fd < 0) {
                perror("open");
                return -1;
        }
	// ，首先设置 ，首先设置 ，首先设置 LED LED LED triggertriggertriggertrigger 属性为“ 属性为“ none ”，然后设置 ”，然后设置 ”，然后设置 ”，然后设置 brightness brightnessbrightness brightness brightness属性交替为 属性交替为 属性交替为 0和 1。实现了 。实现了 。实现了 LEDLED 每隔 1s 点亮一次。 点亮一次。
	for(;;)
	{
		data[0] = flag ? '0': '1'; 
	ret = write(fd, data, 1); //熄灭 LEDLED
	if(ret < 0) {
	perror("write");
	return -1;
	}
		flag = !flag;
		sleep(1);
	}
	return 0;
}
