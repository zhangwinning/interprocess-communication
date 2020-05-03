#include <mqueue.h>
#include <unistd.h> // The getopt() function parses the command-line arguments. Its arguments argc and argv are the argument count and array as passed to the main() function on program invocation
#include <stdio.h>
#include	<stdarg.h>
int main(int argc, char **argv){
	int c, flags;
	mqd_t mqd;
	ssize_t n;
	int prio;
	void *buff;
	struct mq_attr attr;
	
	flags = O_RDONLY;
	while(( c = getopt(argc, argv, "n")) != -1 ) {
		switch (c) {
			case 'n' :
				flags |= O_NONBLOCK;
				break;	
		}	
	}
	printf("the value is %d \n", optind);	
	mqd = mq_open(argv[optind], flags);
	mq_getattr(mqd, &attr);
	buff = malloc(attr.mq_msgsize);
 	
	n = mq_receive(mqd, buff, attr.mq_msgsize, &prio);
	
	printf("read %ld bytes, priotity = %u \n", (long)n, prio);
	
	return 0;	
}
	
