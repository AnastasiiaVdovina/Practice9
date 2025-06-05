#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

#define UID_THRESHOLD 1000
#define MAX_LINE_LENGTH 1024

char* get_current_username(){
	uid_t uid = getuid();
	struct passwd *pw = getpwuid(uid);
	if(pw){
		return pw->pw_name;
	}
	return NULL;
}

int is_regular_user(const char* line, const char* current_user){
	char line_copy[MAX_LINE_LENGTH];
	strncpy(line_copy, line, MAX_LINE_LENGTH);
	
	char* username = strtok(line_copy, ":");
	strtok(NULL, ":") ; //skip password
	char* uid_str = strtok(NULL, ":");
	
	if(!username || !uid_str) return 0;
	
	int uid = atoi(uid_str);
	if(uid >= UID_THRESHOLD && strcmp(username, current_user) != 0){
		printf("Regular user found: %s (UID : %d)\n", username, uid);
		return 1;
	}

	return 0;

}


int main(){
	FILE *fp = popen("getent passwd", "r");
	if(!fp){
		perror("getent failed");
		return 1;
	}

	char* current_user = get_current_username();
	if(!current_user){
		fprintf(stderr, "Failed to get current username.\n");
		return 1;
	}

	char line[MAX_LINE_LENGTH];
	int found = 0;
	while(fgets(line, sizeof(line), fp)){
		if(is_regular_user(line, current_user)){
			found = 1;
		}

		
	}

	if(!found){
		printf("Other regular users not found.\n");
	}

	pclose(fp);
	return 0;

}
