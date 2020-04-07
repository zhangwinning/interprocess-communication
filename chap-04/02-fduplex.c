#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include    <errno.h>

ssize_t writen(int fd, const void *vptr, size_t n);

int main(int argc, char **argv) {
    int fd[2], n;
    char c;
    pid_t childpid;

    pipe(fd);

    if ((childpid = fork()) == 0) {
        sleep(3);
        if ((n = read(fd[0], &c, 1)) != 1) {
            printf("child: read return %d \n", n);
        }
        printf("child read %c \n", c);
        write(fd[0], "c", 1);
        exit(0);
    }
//  parent
    write(fd[1], "p", 1);
    if ((n = read(fd[1], &c, 1)) != 1) {
        printf("parent: read return %d \n", n);
    }
    printf("parent read %c \n", c);
    exit(0);
}