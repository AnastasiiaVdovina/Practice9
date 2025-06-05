#include <stdio.h>
#include <stdlib.h>

int main(){
	printf("Command whoami: \n");
	int whoami_status = system("whoami");

	if(whoami_status == -1){
		perror("whoami command error");
	}

	printf("\nCommand id: \n");
	int id_status = system("id");

	if(id_status == -1){
		perror("Id command error");
	}

	return 0;

}
