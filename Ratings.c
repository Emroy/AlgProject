#include "Ratings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_BLOCK 4096

/*user should free the returned pointer after use*/
char* readLine(FILE* file){
	static char* buffer = NULL;

	if(file == NULL){
		if(buffer){
			free(buffer);
			buffer = NULL;
		}
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

struct UserStruct{
	unsigned int id;
	int8_t* ratings;
	int8_t* flags; //flag in position i is true if rating in position i is not from input
};

struct RateData{
	uint8_t P; //number of nearest neighbors
	User* userRatings;
	unsigned int n; //number of users;
	unsigned int m; //number of items;
};

Ratings readRatings(char* inputFilePath){
	FILE* inputFile = fopen(inputFilePath,"r");
	if(inputFile == NULL){
		perror("Failed to open input file");
		return NULL;
	}

	char* line = readLine(inputFile);
	if(line == NULL){
		perror("Failed to read first line from input file");
		readLine(NULL);
		fclose(inputFile);
		return NULL;
	}

	char* token = strtok(line," \t\n");
	if(token == NULL){
		fprintf(stderr,"Unexpected input encountered on input file first line\n");
		readLine(NULL);
		fclose(inputFile);
		return NULL;
	}

	if(strcmp(token,"P:")){
		fprintf(stderr,"Unexpected input encountered on input file first line\n");
		readLine(NULL);
		fclose(inputFile);
		return NULL;
	}

	Ratings retVal = malloc(sizeof(struct RateData));
	if(retVal == NULL){
		perror("Failed to allocate memory for ratings");
		readLine(NULL);
		fclose(inputFile);
		return NULL;
	}

	token = strtok(NULL," \t\n");
	if(token == NULL){
		fprintf(stderr,"Unexpected input encountered on input file first line\n");
		readLine(NULL);
		fclose(inputFile);
		return NULL;
	}

	retVal->P = atoi(token);


}

void normalizeRatings(Ratings ratingData,char metric){
	if(matric == 'e' || metric == 'c'){

	}
	else if(metric == 'h'){

	}
	else{
		fprintf(stderr,"Invalid metric %c given on normalizeRatings\n",metric);
		exit(-1);
	}
}

void destroyRatings(Ratings ratings){

}