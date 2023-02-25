#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main()
{
    int fd;
    char buf[10] = "Hello";
    char readbuf[6];
    fd = open("/dev/chardrv_dev_file", O_RDWR);
    if (fd < 0)
    {
        printf("Failed to open the device %d\n", fd);
        return fd;
    }
    int ret = write(fd, buf, strlen(buf));
    printf("Written %d bytes to driver\n", ret);

    ret = read(fd , readbuf , 5);
    readbuf[ret] = '\0';
    
    printf("Read from the read buffer: %s\n",readbuf);
    close(fd);
}
