#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

void check_permissions(const char* filepath){
	struct stat st;
	if(stat(filepath, &st) != 0){
		printf("\nChecking: %s\n", filepath);
		printf("stat() error: %s\n", strerror(errno));
		return;
	}

		
	printf("\nFile: %s\n", filepath);
	
	if(access(filepath, R_OK) == 0){
		printf("Reading: allowed\n");
	}else{
		printf("Reading: not allowed %s\n", strerror(errno));
	}


	if(access(filepath, W_OK) == 0){
		printf("Writing: allowed\n");
	} else{
		printf("Writing: not allowed %s\n", strerror(errno));
	}

	if(access(filepath, X_OK) == 0){
		printf("Executing: allowed\n");
	} else{
		printf("executing: not allowed %s\n", strerror(errno));
	}

}


void process_directory(const char *dirpath){
	DIR *dir = opendir(dirpath);
	if(!dir){
		perror("Opening failed");
		return;
	}

	printf("\n=== Content %s ===\n", dirpath);
	
	char command[512];
	snprintf(command, sizeof(command), "ls -l %s", dirpath);
	system(command);

	struct dirent *entry;
	char filepath[1024];
		
	while((entry = readdir(dir)) != NULL){
		if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;

		snprintf(filepath, sizeof(filepath), "%s%s%s", dirpath, 
			(dirpath[strlen(dirpath) - 1] == '/') ? "" : "/",
			entry->d_name);

		check_permissions(filepath);	
	}

	closedir(dir);

}

int main(){
	const char *dirs[] = {
		getenv("HOME"),
		"/usr/bin",
		"/etc"
	};

	for (int i = 0; i < 3; ++i){
		process_directory(dirs[i]);
	}

	return 0;

}
