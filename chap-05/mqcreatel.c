#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>

#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
int main(int argc, char **argv) {
	int c, flags;
	mqd_t mqd;
	flags = O_RDWR | O_CREAT;
	
	while( (c = getopt(argc, argv, "e")) != -1) {
		switch (c) {
			case 'e':
				flags |= O_EXCL;
				break;
		}
	}
	if (optind != argc - 1){
		printf("error");		
	}
	mqd = mq_open(argv[optind], flags, FILE_MODE, NULL);
	mq_close(mqd);
}
