#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

void sighandler()
{

}

int main()
{
    int fd;
    char buf[15]={0};
    fd_set fds;

    fd = open("/dev/chardrv_dev_file", O_RDONLY);
    if(fd < 0)
    {
        printf("Failed to open the device %d\n", fd);
        return fd;
    }
    
    // // Clear the fdset variable
    // FD_ZERO(&fds);

    // // Add fd entry to the fdset
    // FD_SET(fd, &fds);

    // // To invoke the poll method in driver
    // select(fd+1, &fds, 0, 0, 0);
     

    // signal(SIGIO, sighandler);

    // //Set the pid of the process to file objects f_owner
    // fcntl(fd,F_SETOWN, getpid());

    // // Get the initial value of file objects's f_flags
    // flag = fcntl(fd, F_GETLF);

    // // Set the fasync flag and write back to f_flags
    // fcntl(fd, F_SETFL, flag|FASYNC);

    // pause();

    // if(FD_ISSET(fd, &fds) > 0)
    // {
        int ret = read(fd, buf, 11);
        buf[ret] = '\0';
        printf("Read from the buffer: %s bytes: %d\n",buf,ret);
    // }
    close(fd);
}