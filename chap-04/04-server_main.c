#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define FIFO1   "/tmp/fifo.1"
#define FIFO2   "/tmp/fifo.2"
#define MAXLINE     4096

#define    FILE_MODE    (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

void server(int, int);

int main() {
    int readfd, writefd;

    if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST)) {
        printf("can't create %s", FIFO1);
    }
    if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST)) {
        unlink(FIFO1);
        printf("cat't create %s \n", FIFO2);
    }

    readfd = open(FIFO1, O_RDONLY, 0);
    writefd = open(FIFO2, O_WRONLY, 0);

    server(readfd, writefd);
    exit(0);
}

void server(int readfd, int writefd) {
    int fd;
    ssize_t n;
    char buff[MAXLINE + 1];


    if ((n = read(readfd, buff, MAXLINE)) == 0) {
        printf("end of file while reading pathname");
    }
    buff[n] = '\0';
    printf("this server is buff %s \n", buff);
    if ((fd = open(buff, O_RDONLY)) < 0) {
        snprintf(buff + n, sizeof(buff) - n, ": can't open, %s \n",
                 strerror(errno));
        n = strlen(buff);
        write(writefd, buff, n);
    } else {

        while ((n = read(fd, buff, MAXLINE)) > 0) {
            write(writefd, buff, n);
        }
        printf("this is fd %d ,%zd\n", fd, n);
        close(fd);
    }
}