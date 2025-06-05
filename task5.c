#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	char template[] = "/tmp/mytempfileXXXXXX";
	int fd = mkstemp(template);
	if(fd < 0){
		perror("Failed to create temp file");
		return 1;
	}

	printf("Temp file created: %s\n", template);

	close(fd);

	struct passwd *pwd = getpwnam("anastasiia");
	if(!pwd){
		fprintf(stderr, "User anastasiia not found\n");
		unlink(template);
		return 1;
	}

	if(chown(template, pwd->pw_uid, pwd->pw_gid) < 0){
		perror("chown failed");
		unlink(template);
		return 1;
	}

	printf("Owner changed to anastasiia\n");

	if(chmod(template, 0664) < 0){
		perror("chmod failed");
		unlink(template);
		return 1;
	}

	printf("Access roots set on 0664\n");

	pid_t pid = fork();
	if(pid < 0){
		perror("fork failed");
		unlink(template);
		return 1;
	}

	if(pid == 0){
		//child process
		if(setgid(pwd->pw_gid) < 0 ){
			perror("setgid failed");
			exit(1);
		}

		if(setuid(pwd->pw_uid) < 0 ){
			perror("setuid failed");
			exit(1);
		}

		printf("The child pricess runs on behalf of anastasiia(uid=%d, gid=%d)\n", pwd->pw_uid, pwd->pw_gid);
		
		FILE *f = fopen(template, "r+");
		if(f){
			printf("File opened for reading and writing\n");
			fclose(f);
		} else{
			printf("Failed to open file for reading and writing: %s\n", strerror(errno));
		}

		exit(0);
	} else{
		//parent process

		int status;
		waitpid(pid, &status, 0);
	}

	unlink(template);
	printf("Temp file deleted\n");
	
	return 0;

}
