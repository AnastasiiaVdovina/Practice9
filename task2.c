#include <stdlib.h>
#include <stdio.h>

int main(){
	int status = system("sudo cat /etc/master.passwd");
	if(status == -1){
		perror("system");
		return 1;
	}

	return 0;
}
