#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 128

int main(int argc,char* argv[]){
	if(argc != 2){
		fprintf(stderr,"Wrong number of arguements\n");
		return -1;
	}

	FILE* queryFile = fopen(argv[1],"r");
	char buffer[BUFFER_SIZE];

	fgets(buffer,BUFFER_SIZE,queryFile);
	unsigned short hamming_radius = atoi(strtok(buffer,"Radius: \t\n"));
	long long int q;

	while(fgets(buffer,BUFFER_SIZE,queryFile)){
		strtok(buffer," \t\n");
		q = strtoll(strtok(NULL," \t\n"),NULL,2);
		/*do calculations*/
	}

	fclose(queryFile);
	return 0;
}
