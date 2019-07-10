#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <fcntl.h>

#define DEV_NAME  "/dev/char_cdev"

 int main(int argc, char *argv[]) {
	int i;
    int fd = 0;
    int dat = 0;

 fd = open (DEV_NAME, O_RDWR);
 if (fd < 0) {
 perror("Open "DEV_NAME" Failed!\n");
 exit(1);

}


 i = read(fd, &dat, 1);
 if (!i) {
 perror("read "DEV_NAME" Failed!\n");
 exit(1);
}

 dat = 0;
 i = write(fd, &dat, 1);
 if (!i) {
 perror("write "DEV_NAME" Failed!\n");
 exit(1);

}

 i = ioctl(fd, NULL, NULL);
 if (!!i) {
 perror("ioctl "DEV_NAME" Failed!\n");
 exit(1);


 close(fd);
 return 0;

}