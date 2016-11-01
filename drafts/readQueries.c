#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 128

int main(int argc,char* argv[]){
	if(argc != 3){
		fprintf(stderr,"Wrong number of arguements\n");
		return -1;
	}

	FILE* queryFile = fopen(argv[1],"r");
	if(queryFile == NULL){
		perror("Failed to open query file");
		return -2;
	}

	FILE* outputFile = fopen(argv[2],"w");
	if(outputFile == NULL){
		perror("Failed to open/create output file");
		return -3;
	}

	char buffer[BUFFER_SIZE];

	fgets(buffer,BUFFER_SIZE,queryFile);
	unsigned short hamming_radius = atoi(strtok(buffer,"Radius: \t\n"));
	long long int q;

	while(fgets(buffer,BUFFER_SIZE,queryFile)){
		char* item_name=strtok(buffer," \t\n");
		q = strtoll(strtok(NULL," \t\n"),NULL,2);
		/*do calculations*/
		fputs("Query: ",outputFile);
		fputs(item_name,outputFile);
		fputc('\n',outputFile);
		fputs("R-near neighbors:\n",outputFile);
		/*output neighbors*/
		fputs("Nearest neighbor: ",outputFile);
		/*output nearest neighbor*/
		fputc('\n',outputFile);
		fputs("distanceLSH: ",outputFile);
		/*output distance LSH*/
		fputc('\n',outputFile);
		fputs("distanceTrue: ",outputFile);
		/*output true distance*/
		fputc('\n',outputFile);
		fputs("tLSH: ",outputFile);
		/*output tLSH*/
		fputc('\n',outputFile);
		fputs("tTrue: ",outputFile);
		/*output tTrue*/
		fputc('\n',outputFile);
	}

	fclose(queryFile);
	fclose(outputFile);
	return 0;
}