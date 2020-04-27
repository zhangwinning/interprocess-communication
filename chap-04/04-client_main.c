
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define FIFO1   "/tmp/fifo.1"
#define FIFO2   "/tmp/fifo.2"
#define MAXLINE     4096

void client(int, int);

int main() {
    int readfd, writefd;

    writefd = open(FIFO1, O_WRONLY, 0);
    readfd = open(FIFO2, O_RDONLY, 0);

    client(readfd, writefd);

    close(readfd);
    close(writefd);

    unlink(FIFO1);
    unlink(FIFO2);

}


void client(int readfd, int writefd) {
    size_t len;
    ssize_t n;
    char buff[MAXLINE];

    /**
     * read pathname
     */
    fgets(buff, MAXLINE, stdin);
    len = strlen(buff);
    if (buff[len - 1] == '\n') {
        len--;
    }
    printf("this is buff %s", buff);
    write(writefd, buff, len);
    while ((n = read(readfd, buff, MAXLINE)) > 0) {
        write(STDOUT_FILENO, buff, n);
    }

}