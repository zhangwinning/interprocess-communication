#include <mqueue.h>
#include <stdio.h>
int main (int argc, char **argv) {
	mqd_t mqd;
	struct mq_attr attr;
	mqd = mq_open(argv[1], O_RDONLY);
	mq_getattr(mqd, &attr);
 	printf("max #msgs = %ld, max #bytes/msg = %ld, #currently on queue = %ld \n", 
	attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
	mq_close(mqd);
	return 0;	
}
