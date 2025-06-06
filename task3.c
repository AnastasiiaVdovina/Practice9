#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pwd.h>

int main(){
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;

	char user_file[512];
	char copy_file[512];

	snprintf(user_file, sizeof(user_file), "%s/user_file.txt", homedir);
	snprintf(copy_file, sizeof(copy_file), "%s/copied_by_root.txt", homedir);


	//creating file as a regular user
	FILE *f = fopen(user_file, "w");
	if(!f) {
		perror("create");
		return 1;
	}
	fprintf(f, "File created by user\n");
	fclose(f);
	printf("User created the file: %s\n", user_file);
	
	//copying file by root
	printf("Let`s do it as a root\nClick Enter\n");
	getchar();

	if(getuid() == 0){
		char cmd[1024];
		snprintf(cmd, sizeof(cmd), "cp %s %s && chowm root:wheel %s && chmod 664 %s", user_file, copy_file, copy_file, copy_file);
		
		int ret = system(cmd);
		if(ret != 0){
			fprintf(stderr, "Failed to copy file by root\n");
			return 1;
		}

		printf("File was copied into %s by root\n", copy_file);
		return 0;
	}

	

	//trying to edit file
	printf("\nTrying to edit file as a user...\n");
	f = fopen(copy_file, "a");
	if(!f){
		perror("Change isn`t allowed");
	} else{
		fprintf(f, "A line was added\n");
		fclose(f);
		printf("Change was successful\n");
	}

	
	//trying to delete file
	printf("\nTrying to delete file as user...\n");
	if(unlink(copy_file) != 0){
		perror("Deletion isn`t allowed");
	}else{
		printf("File was deleted\n");
	}

	return 0;

}	
