#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#define FIFO1   "/tmp/fifo.1"
#define    FILE_MODE    (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define    MAXLINE        4096    /* max text line length */
#define FIFO2   "/tmp/fifo.2"

void client(int readfd, int writefd);

void server(int readfd, int writefd);

int main(int argc, char **argv) {
    int readfd, writefd;
    pid_t childpid;

    if ((mkfifo(FIFO1, FILE_MODE) < 0) && (errno != EEXIST)) {
        printf("can't create %s \n", FIFO1);
    }
    if ((mkfifo(FIFO2, FILE_MODE) < 0) && (errno != EEXIST)) {
        unlink(FIFO1);
        printf("can't create %s \n", FIFO2);
    }

    if ((childpid = fork()) == 0) {
        readfd = open(FIFO1, O_RDONLY, 0);
        writefd = open(FIFO2, O_WRONLY, 0);

        server(readfd, writefd);
        exit(0);
    }

//  parent
    /**
     * 这里有一个点: 如果我们对换父进程中两个 open 调用的顺序，该程序就不工作。起原因在于:
     * 如果当前没有任何进程打开某个 FIFO 来写，那么打开该 FIFO 来读的进程将阻塞。对换父进程中两个
     * open 调用顺序后，父子进程都将打开同一个 FIFO 来读，然而当时没有任何进程打开该文件来写，于是父子进程
     * 都会阻塞。这种现象称为死锁(deadlock)。
     */
    writefd = open(FIFO1, O_WRONLY, 0);
    readfd = open(FIFO2, O_RDONLY, 0);

    client(readfd, writefd);

    waitpid(childpid, NULL, 0);
    close(readfd);
    close(writefd);

    unlink(FIFO1);
    unlink(FIFO2);
    exit(0);
}

void client(int readfd, int writefd) {
    size_t len;
    ssize_t n;
    char buff[MAXLINE];

    /* 4read pathname */
    fgets(buff, MAXLINE, stdin);
    len = strlen(buff);        /* fgets() guarantees null byte at end */
    if (buff[len - 1] == '\n')
        len--;                /* delete newline from fgets() */

    /* 4write pathname to IPC channel */
    write(writefd, buff, len);

    /* 4read from IPC, write to standard output */
    while ((n = read(readfd, buff, MAXLINE)) > 0)
        write(STDOUT_FILENO, buff, n);
}

void server(int readfd, int writefd) {
    int fd;
    ssize_t n;
    char buff[MAXLINE + 1];

    /* 4read pathname from IPC channel */
    if ((n = read(readfd, buff, MAXLINE)) == 0)
        printf("end-of-file while reading pathname");
    buff[n] = '\0';        /* null terminate pathname */

    if ((fd = open(buff, O_RDONLY)) < 0) {
        /* 4error: must tell client */
        snprintf(buff + n, sizeof(buff) - n, ": can't open, %s\n",
                 strerror(errno));
        n = strlen(buff);
        write(writefd, buff, n);

    } else {
        /* 4open succeeded: copy file to IPC channel */
        while ((n = read(fd, buff, MAXLINE)) > 0)
            write(writefd, buff, n);
        close(fd);
    }
}