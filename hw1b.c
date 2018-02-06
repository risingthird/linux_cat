#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

//int before;
//int after;
void signal_handler(int sig);
int getint();

int main(int argc, char **argv){
	int before, after;
	signal(SIGALRM, signal_handler);
	if(argc > 3 || argc == 2){
		printf("Wrong argument number!");
		exit(0);
	}
	else if(argc == 3 && !strcmp(argv[1], "-s")){
		if(!atoi(argv[2])){
			printf("You should type a positive integer!\n");
			exit(0);
		}
		else if(atoi(argv[2]) && strchr(argv[2],'.')){
			printf("Float input is not accepted! Try Integers!\n");
		}
		else{
			struct itimerval new_value, old_value;      
    			new_value.it_value.tv_sec = 1;    
    			new_value.it_value.tv_usec = 0;  
    			new_value.it_interval.tv_sec = atoi(argv[2]);  
    			new_value.it_interval.tv_usec = 0;  
    			if(setitimer(ITIMER_REAL, &new_value, &old_value)<0){
				printf("Setting timer failed!!\n");
				exit(0);
			}
			for(;;){
				before = getint();
				pause();
				after = getint();
				printf("The number of interrupts in the previous %d second is %d\n", atoi(argv[2]), after-before);
				printf("The number of interrupts after since boot is %d\n", after);
			}
		}
	}
	else if(argc == 3 && strcmp(argv[1], "-s")){
		printf("Flag is wrong, to turn on flag, please type \"-s\"\n");
		exit(0);
	}
	else{
		struct itimerval new_value, old_value;
    		new_value.it_value.tv_sec = 1;
    		new_value.it_value.tv_usec = 0;
    		new_value.it_interval.tv_sec = 1;
    		new_value.it_interval.tv_usec = 0;
    		//signal(SIGALRM, signal_handler);
		if(setitimer(ITIMER_REAL, &new_value, 0)<0){
			printf("Setting timer failed!!\n");
			exit(0);
		}
		while(1){
			//printf("Normal enter.\n");
			before = getint();
			pause();
			after = getint();
			printf("The number of interrupts in the previous second is %d\n", after-before);
			printf("The number of interrupts since last boot is %d\n", after);
		
		}

	}
	return 0;


}


void signal_handler(int sig){
	printf("Signal %d is caught.\n", sig);

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
