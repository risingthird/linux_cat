#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void signal_alarm(int sig);
int getint();

int main(int argc, char **argv){
	int before, after;
	signal(SIGALRM, signal_alarm);
	//int counter_for_test = 10;
	while(1){
	before = getint();
	alarm(1);
	pause();
	after = getint();
	printf("The number of interrupts in the previous second is %d\n",after-before);
	printf("The total number of interrupts since last boot is %d\n", after);
	//counter_for_test--;
	}

	return 0;
	
}


int getint(){
	char buf[1024];
	int fd;
	int result;
	char *split;
	fd = open("/proc/stat", O_RDONLY);
	if(fd>=0){
		read(fd, buf, 1024);
		split = strtok(buf, " \n");
		while(split != NULL && strcmp(split, "intr")){
			split = strtok(NULL, " \n");
		}
		split = strtok(NULL, " ");
		result = atoi(split);
		close(fd);
		return result;
	}
	else{
		printf("Getting proc files failed");
		exit(0);
	}
	close(fd);
	return result;

}

void signal_alarm(int signal){
	printf("Alarm is caught");
}
