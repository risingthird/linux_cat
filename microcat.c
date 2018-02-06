#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1
#define MODE S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH

void getShot(int sig){
	if(0<sig<=30){
		write(STDOUT,"Help! I think I\'ve been shot!!!\n",32);
		exit(0);
	}
}


void printResult(int file_in, int file_out);
void printNoArgument(int file_out);

int main(int argc, char **argv){
	int file_out = STDOUT;
	int file_in = -1;
	char *output;
	int redirector = -1;
	//mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	for(int i = 1; i <= 30; i++) {
		if( i != 9 && i!= 19) {
			signal(i, getShot);
		}
	}
	if(argc-1){
		for(int i = 1; i < argc; i++) {
			if(!strcmp(argv[i], ">")) {
				if(i+1 == argc) {
					//sleep(1);
					write(STDOUT, "Please type in the filename you want to create!\n", 48);
					exit(0);
				}
				else if(i+2 < argc) {
					write(STDOUT, "You can only create one file at a time!\n", 40);
					exit(0);
				}
				else {
					output = argv[++i];
					if((file_out = open(output, O_WRONLY | O_CREAT | O_TRUNC, MODE)) == -1) {
						write(STDOUT, "Unable to create file!\n", 23);
						exit(0);
					}
					redirector = i-1;
					break;
				}
			}
			else if(!strcmp(argv[i], ">>")) {
				if(i+1 == argc) {
					write(STDOUT, "Please type in the filename you want to append to!\n", 51);
					exit(0);
				}
				else if(i+2 < argc) {
					write(STDOUT, "You can only append to one file at a time!\n", 43);
					exit(0);
				}
				else {
					output = argv[++i];
					if((file_out = open(output, O_WRONLY | O_APPEND | O_CREAT, MODE)) == -1) {
						write(STDOUT, "Unable to append text or to create file!\n", 40);
						exit(0);
					}
					redirector = i-1;
					break;
				}
				
			}
		}
	}

	if(argc == 1) {		// cases when "cat "
		printNoArgument(STDOUT);		
	}
	else {
		int i = 1;
		while(i < argc) {
			if(i == redirector) {
				break;
			}
			if(!strcmp(argv[i], "-")) {
				printNoArgument(file_out);
			}
			else {
				if((file_in = open(argv[i], O_RDONLY)) == -1) {
					write(STDOUT, "Unable to open file or file doesn\'t exist!\n", 43);
					exit(0);	
				}
				printResult(file_in, file_out);
				close(file_in);
			}
			i++;
		}
		close(file_out);
	}

	return 0;
}

void printResult(int file_in, int file_out){
	int bufSize = lseek(file_in, 0, SEEK_END);
	lseek(file_in, 0, SEEK_SET);
	char buf[bufSize];
	int toWrite;
	while((toWrite = read(file_in, buf, bufSize))> 0) {
		if(write(file_out, buf, toWrite)<0) {
			write(STDOUT, "Unable to write in!\n", 19);
			exit(0);
		}
	}

}

void printNoArgument(int file_out){
	char buf[5];
	int toWrite;
	//while(1){
		while((toWrite = read(STDIN, buf, 5)) > 0) {
			write(file_out, buf, toWrite);
		}
	//}
}

