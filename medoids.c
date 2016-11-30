/* SOFTWARE DEVELOPMENT FOR ALGORITHMIC PROBLEMS
   ASSIGNMENT 2
   LELEGIANNIS IOANNIS: 1115201200090
   POULIDIS NIKOLAOS: 1115200000111 */
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"
#include "List.h"
#define BUFFER_BLOCK 4096*10  /*Block size*/

typedef struct ConfParams{
	int k;
	int hashFuncNum;
	int hashTableNum;
	int claransFrac;
	int claransIter;
}ConfParams;

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
		
		fgets(buffer+bytes_read,BUFFER_BLOCK,file);
		bytes_read+=BUFFER_BLOCK-1; /*if we read less chars than that we exit the loop*/
	}while((feof(file) == 0) && (strchr(buffer,'\n') == NULL));

	return buffer;
}

Data* evalInput(char* inputFilePath,unsigned int* n,char* metric){
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
	if(!strcmp(token,"hamming")){
		*metric = 'h';
	}
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
		if(!strcmp(token,"euclidean")){
			*metric = 'e';
		}
		else if(!strcmp(token,"cosine")){
			*metric = 'c';
		}
		else{
			fprintf(stderr,"Invalid metric \"%s\" given on input file.\n",token);
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}
	}
	else if(!strcmp(token,"matrix")) *metric = 'm';
	else{
		fprintf(stderr,"Invalid metric space \"%s\" given.\n",token);
		fclose(inputFile);
		free(line);
		line = NULL;
		return NULL;
	}

	/*Start reading and creating data*/
	if(*metric != 'm'){
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
			if(*metric == 'h') newData = hamming_data_create(line);
			else if(*metric == 'e') newData = euclidean_data_create(line);
			else if(*metric == 'c') newData = cosine_data_create(line);
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
		
		data_create_distance_matrix(dataArray,*n);

		fclose(inputFile);
		free(line);
		line = NULL;
		return dataArray;
	}
	else{
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

		unsigned int** distance_matrix = malloc((*n)*sizeof(unsigned int*));
		if(distance_matrix == NULL){
			perror("Failed to allocate memory for matrix distance matrix");
			fclose(inputFile);
			free(line);
			line=NULL;
			return NULL;
		}
		for(i=0;i<(*n);i++){
			distance_matrix[i]=malloc((*n)*sizeof(unsigned int));
			if(distance_matrix[i]==NULL){
				perror("Failed to allocate memory for matrix distance matrix");
				fclose(inputFile);
				free(line);
				line=NULL;
				unsigned int j;
				for(j=i-1;j>=0;j--) free(distance_matrix[j]);
				free(distance_matrix);
				return NULL;
			}
		}

		i=0;
		while(!feof(inputFile)){
			if(i>=(*n)) fprintf(stderr,"something is wrong (i)\n");
			line = readLine(inputFile);
			if(line == NULL){
				perror("Could not allocate memory to read next line of input file");
				fclose(inputFile);
				return NULL;
			}
			if(line[0]=='\0') break;

			token = strtok(line," \n\t");
			unsigned int j = 0;
			while(token){
				if(j>=(*n)) fprintf(stderr,"something is wrong (j)\n");
				distance_matrix[i][j] = atoi(token);
				token = strtok(NULL," \t\n");
				j++;
			}
			i++;
		}

		data_set_distance_matrix(distance_matrix);

		fclose(inputFile);
		free(line);
		line = NULL;
		return dataArray;
	}
}

ConfParams* evalConf(char* confFilePath,unsigned int n)
{
	int k=0,f=0,L=0,s=0,i=0;
	double max;
    FILE* confFile;
    ConfParams *params = malloc(sizeof(ConfParams));
    if(params == NULL){
    	perror("Failed to allocate memory for configuration parameters");
    	return NULL;
    }
    
	if((confFile=fopen(confFilePath,"r"))==NULL)
	{
		perror("Failed to open configuration file");
		exit(-2);
	}
	if((params=malloc(sizeof(ConfParams)))==NULL)
	{
		perror("Failed to allocate memory for configuration parameters");
		return NULL;
	}
	fscanf(confFile,"number_of_clusters:%d",&k);
	if(!k)
	{
		printf("Wrong configuration file.\n");
		return NULL;
	}
	fscanf(confFile,"number_of_hash_functions:%d",&f);
	if(!f)
	{
	    f=4;
	}
	fscanf(confFile,"number_of_hash_tables:%d",&L);
	if(!L)
	{
		L=5;
	}
	fscanf(confFile,"clarans_set_fraction:%d",&s);
	if(!s)
	{
	    if((max=0.12*k*(n-k))>250)
	    {
	    	s=max;
	    }
	    else
	    {
	    	s=250;
	    }
	}
	fscanf(confFile,"clarans_iterations:%d",&i);
	if(!s)
	{
		i=2;
	}
	params->k=k;
	params->hashFuncNum=f;
	params->hashTableNum=L;
	params->claransFrac=s;
	params->claransIter=i;
	return params;
}

Medoids aplly_kMedoids(int mode,ConfParams* conf,unsigned int n){
	/*CLARA*/
	if(mode == 9){
		 return clara(conf->k,n);
	}

	Medoids currentMedoids;
	Medoids previousMedoids = NULL;
	/*Initialization*/
	if(mode & 0x4){
		currentMedoids = park_Jun(conf->k,n);
	}
	else{
		currentMedoids = k_MedoidsPP(conf->k,n);
	}

	do{
		/*Assignment*/
		if(mode & 0x2){
			lsh_dbh(currentMedoids,n,conf->hashFuncNum,conf->hashTableNum);
		}
		else{
			pam(currentMedoids,n);
		}

		if(previousMedoids) medoids_destroy(previousMedoids);
		previousMedoids = currentMedoids
		/*Update*/
		if(mode & 0x1){
			currentMedoids = clarans(currentMedoids,n,conf->claransIter,conf->claransFrac);
		}
		else{
			currentMedoids = lloyds(currentMedoids,n);
		}
	}while(!medoids_areSame(currentMedoids,previousMedoids));

	return currentMedoids;
}

void outputResults(char* outFilePath){
	FILE* outFile = fopen(outFilePath,"w");
	if(outFile == NULL){
		perror("Failed to open/create output file");
		exit(-3);
	}
}

int main(int argc,char* argv[]) 
{
	char* outFilePath = NULL;
	int complete = 0;
	ConfParams* confParams = NULL;
	unsigned int n;
	char metric;

	/*Read command line arguements*/
	int i;
	for(i = 1;i < argc;i++){
		if(!strcmp(argv[i],"-d")){
			i++;
			evalInput(argv[i],&n,&metric);
		}
		else if(!strcmp(argv[i],"-c")){
			i++;
			confParams = evalConf(argv[i],n);
		}
		else if(!strcmp(argv[i],"-o")){
			i++;
			outFilePath = argv[i];
		}
		else if(!strcmp(argv[i],"-complete")){
			complete = 1;
		}
		else{
			fprintf(stderr,"Invalid parameters given. Call should be:\n");
			fprintf(stderr,"./medoids -d <input file> -c <configuration file> -o <output file>\n");
			return 0;
		}
	}

	if(confParams == NULL){
		fprintf(stderr,"Configuration data were not set.\nTerminating program.\n");
		return 0;
	}
	if(outFilePath == NULL){
		printf("Path for output file was not given.\n");
		printf("Seting default output file 'outFile'\n");
		outFilePath = "./outFile";
	}
	
	Medoids medoids;
	for(i=0;i<=9;i++){
		medoids = aplly_kMedoids(i);
	}

	outputResults(outFilePath);

	data_destroy_distance_matrix();
	return 0;
}
