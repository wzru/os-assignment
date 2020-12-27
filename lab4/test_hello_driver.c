#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(void)
{
    int user_buf[2] = {20, 40};
    int read_buf;
    int fd = open("/dev/hello_driver", O_RDWR);
    write(fd, &user_buf, 2 * sizeof(int));
    read(fd, &read_buf, sizeof(int));
    printf("result = %d\n", read_buf);
    return 0;
}