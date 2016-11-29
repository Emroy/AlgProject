#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>
//#include "HashGen.h"
//#include "HashTable.h"
#include "../data.h"
#include "../List.h"
#define BUFFER_BLOCK 4096  /*Block size*/
#define PATH_BUFFER_SIZE 100

/*user should free the returned pointer after use*/
char* readLine(FILE* file){
	static char* buffer = NULL;
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
		
		fgets(buffer+bytes_read,buffer_size,file);
		bytes_read+=BUFFER_BLOCK; /*if we read less chars than that we exit the loop*/

	}while(!feof(file) && !strchr(buffer+bytes_read-BUFFER_BLOCK,'\n'));

	return buffer;
}

Data* evalInput(const char* inputFilePath,unsigned int* n){
	FILE* inputFile = fopen(inputFilePath,"r");
	if(inputFile == NULL){
		perror("Failed to open input file");
		exit(-1);
	}

	char* line = readLine(inputFile);
	if(line == NULL){
		perror("Could not allocate memory to read next line of input file");
		fclose(inputFile);
		return NULL;
	}
	char* token = strtok(line," \t\n");
	if(strcmp("@metric_space",token)){
		fprintf(stderr,"Invalid command \"%s\" given on input file\n",token);
		fclose(inputFile);
		free(line);
		line = NULL;
		return NULL;
	}

	token = strtok(NULL," \t\n");
	char metric = 0;
	if(!strcmp(token,"hamming")) metric = 'h';
	else if(!strcmp(token,"euclidean") || !strcmp(token,"vector")){
		line = readLine(inputFile);
		if(line == NULL){
			perror("Could not allocate memory to read next line of input file");
			fclose(inputFile);
			return NULL;
		}
		token = strtok(line," \t\n");
		if(strcmp("@metric",token)){
			fprintf(stderr,"Invalid command \"%s\" given for metric space euclidean on input file.\n",token);
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}

		token = strtok(NULL," \t\n");
		if(!strcmp(token,"euclidean")) metric = 'e';
		else if(!strcmp(token,"cosine")) metric = 'c';
		else{
			fprintf(stderr,"Invalid metric \"%s\" given on input file.\n",token);
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}
	}
	else if(!strcmp(token,"matrix")) metric = 'm';
	else{
		fprintf(stderr,"Invalid metric space \"%s\" given.\n",token);
		fclose(inputFile);
		free(line);
		line = NULL;
		return NULL;
	}

	/*Start reading and creating data*/
	if(metric != 'm'){
		List l = list_create();
		/*must do something if list could not be created*/
		while(!feof(inputFile)){
			line = readLine(inputFile);
			if(line == NULL){
				perror("Could not allocate memory to read next line of input file");
				fclose(inputFile);
				return NULL;
			}
			if(line[0]=='\0') break;

			Data newData;
			if(metric == 'h') newData = hamming_data_create(line);
			else if(metric == 'e') newData = euclidean_data_create(line);
			else if(metric == 'c') newData = cosine_data_create(line);
			else{/*This should never happen*/
				fprintf(stderr,"Unexpected error while reading input file.\n");
				fclose(inputFile);
				free(line);
				line = NULL;
				return NULL;
			}

			if(newData == NULL) return NULL;

			list_pushEnd(l,newData);
		}

		*n = list_length(l);
		Data* dataArray = malloc((*n)*sizeof(Data));
		if(dataArray == NULL){
			perror("Failed to allocate memory for data array");
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}

		unsigned int i;
		for(i=0;i<(*n);i++){
			dataArray[i] = list_pop(l);
			if(dataArray[i] == NULL) break;
		}

		list_destroy(l);
		fclose(inputFile);
		free(line);
		line = NULL;
		return dataArray;
	}
	else if(metric == 'm'){
		line = readLine(inputFile);
		if(line == NULL){
			perror("Could not allocate memory to read next line of input file");
			fclose(inputFile);
			return NULL;
		}

		token = strtok(line," ,\t\n");
		if(strcmp(token,"@items")){
			fprintf(stderr,"Invalid command \"%s\" given on matrix input file\n",token);
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}

		List l = list_create();
		while(token = strtok(NULL," ,\t\n")){
			Data newData = matrix_data_create(token);
			list_pushEnd(l,newData);
		}

		*n = list_length(l);
		Data* dataArray = malloc((*n)*sizeof(Data));
		if(dataArray == NULL){
			perror("Failed to allocate memory for data array");
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}

		unsigned int i;
		for(i=0;i<(*n);i++){
			dataArray[i] = list_pop(l);
			if(dataArray[i] == NULL) break;
		}

		list_destroy(l);
		fclose(inputFile);
		free(line);
		line = NULL;
		return dataArray;
	}
	else{/*This should never happen*/
		fprintf(stderr,"Unexpected error while reading input file.\n");
		fclose(inputFile);
		free(line);
		line = NULL;
		return NULL;
	}
}

void evalQuery(const char* queryFilePath){

}

void evalOutput(const char* outputFilePath){

}

int main(int argc,char* argv[]){
	if(argc != 2){
		printf("Wrong number of arguements\n");
		return 0;
	}
	Data* temp;
	unsigned int n;
	if((temp = evalInput(argv[1],&n))!=NULL) printf("Success!\n");
	else printf("Failure\n");

	unsigned int i;
	for(i=0;i<n;i++) data_destroy(temp[i]);
	free(temp);

	return 0;
}