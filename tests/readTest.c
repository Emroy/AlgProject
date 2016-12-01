#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_BLOCK 4096  /*Block size*/

char* readLine(FILE* file){
	static char* buffer = NULL;

	if(file == NULL){
		free(buffer);
		buffer = NULL;
		return NULL;
	}

	if(buffer != NULL){
		free(buffer);
		buffer = NULL;
	}

	unsigned int buffer_size = 0;
	unsigned int bytes_read = 0;
	unsigned int i;

	do{
		buffer_size += BUFFER_BLOCK;

		buffer = realloc(buffer,buffer_size);
		if(buffer == NULL) return NULL;
		for(i=buffer_size-BUFFER_BLOCK;i<buffer_size;i++) buffer[i] = 0;

		fgets(buffer+bytes_read,BUFFER_BLOCK,file);
		bytes_read+=BUFFER_BLOCK-1; /*if we read less chars than that we exit the loop*/
	}while((feof(file) == 0) && (strchr(buffer,'\n') == NULL));

	return buffer;
}

int main(int argc,char* argv[]){
	FILE* fp = fopen(argv[1],"r");
	if(fp == NULL){
		perror("Failed to open file");
		return 1;
	}

	FILE* outFile = fopen(argv[2],"w");
	if(outFile == NULL){
		perror("Failed to open out file");
		return 2;
	}

	char* line = readLine(fp);
	line = readLine(fp);

	char* token = strtok(line," \t\n");

	while(token = strtok(NULL," \t\n")){
		fprintf(outFile,"%s ",token);
	}

	fprintf(outFile,"\n");

	return 0;
}