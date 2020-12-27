#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define TRANS_SIZE 10
#define BUF_SIZE 4096

char *user_buf = "This is OS Lab 4-3 by "
                 "U2018"
                 "\x31\x34\x38\x36"
                 "7.\n"
                 "UNIX is basically a simple operating system, but you have to be a genius to understand the simplicity. Dennis Ritchie";
char buf[BUF_SIZE];
int pos = 0;
int write_p = 0;

int main(void)
{
    int i, j, re, head, tail;
    int fd = open("/dev/bufio_driver", O_RDWR);
    if (fd < 0)
    {
        printf("fail to open device\n");
        return 0;
    }
    /* write */
    printf("\n\n====================Begin Write====================\n\n");
    i = 0;
    head = 0;
    while (1)
    {
        i++;
        re = write(fd, user_buf + write_p, TRANS_SIZE);
        printf("\n\nthe %d time write %d bits\nwrite buffer is:\n", i, re);
        head += re;
        for (j = 0; j < head; j++)
            printf("%c", user_buf[j]);
        if (re < TRANS_SIZE)
        {
            printf("\n\ndevice buffer is full\n");
            break;
        }
        write_p += TRANS_SIZE;
    }
    /* read */
    printf("\n\n====================Begin Read====================\n\n");
    i = 0;
    pos = 0;
    while (1)
    {
        i++;
        re = read(fd, buf + pos, TRANS_SIZE);
        printf("\n\nthe %d time read %d bits\nread buffer is:\n", i, re);
        for (j = 0; j < pos + re; j++)
            printf("%c", buf[j]);
        if (re < TRANS_SIZE)
        {
            printf("\n\ndevice buffer is empty\n");
            break;
        }
        pos += TRANS_SIZE;
    }
    return 0;
}