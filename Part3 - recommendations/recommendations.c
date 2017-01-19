#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Ratings.h"
#include "Clustering.h"
#include <time.h>

int main(int argc,char* argv[]){
	int i = 1;
	char* outputFilePath;
	short validate = 0;

	while(i < argc){
		if(!strcmp(argv[i],"-d")){
			readRatings(argv[i+1]);
			i+=2;
		}
		else if(!strcmp(argv[i],"-o")){
			outputFilePath = argv[i+1];
			i+=2;
		}
		else if(!strcmp(argv[i],"-validate")){
			validate = 1;
			i++;
		}
		else{
			fprintf(stderr,"\"%s\" is not a valid command line command\n",argv[i]);
			return -1;
		}
	}

	FILE* outputFile = fopen(outputFilePath,"w");
	if(outputFile == NULL){
		perror("Failed to open output file");
		return -1;
	}
	fclose(outputFile);

	double hammingLSH_error = 0.0;
	double euclideanLSH_error = 0.0;
	double cosineLSH_error = 0.0;
	for(i=0;i<NUM_OF_FOLDS;i++){
		Ratings train = getRatingTrainSet();
		Ratings test = getRatingTestSet();

		lsh_init(train,'h');

		outputFile = fopen(outputFilePath,"a");
		if(outputFile == NULL){
			perror("Failed to open outputFile");
		}

		fprintf(outputFile,"Hamming LSH fold number %d\n",i+1);

		unsigned int numOfUsers = ratings_getNumberOfUsers(train);
		unsigned int j;
		clock_t timeElapsed = clock();
		for(j=0;j<numOfUsers;j++){
			User currentUser = ratings_getUser(train,j+1);
			unsigned int* recommendedItems = lsh_getRecommendedItems(currentUser);
			fprintf(outputFile,"%u",user_getUserID(currentUser));
			unsigned int k;
			for(k=0;k<5;k++) fprintf(outputFile,"\t%u",recommendedItems[k]);
			fprintf(outputFile,"\n");
		}
		timeElapsed -= clock();

		hammingLSH_error += validateResults(train,test);
		lsh_terminate();
		fprintf(outputFile,"Execution Time: %f\n",1000*(((float)timeElapsed)/CLOCKS_PER_SEC));

		lsh_init(train,'e');

		fprintf(outputFile,"Euclidean LSH fold number %d\n",i+1);

		numOfUsers = ratings_getNumberOfUsers(train);
		timeElapsed = clock();
		for(j=0;j<numOfUsers;j++){
			User currentUser = ratings_getUser(train,j+1);
			unsigned int* recommendedItems = lsh_getRecommendedItems(currentUser);
			fprintf(outputFile,"%u",user_getUserID(currentUser));
			unsigned int k;
			for(k=0;k<5;k++) fprintf(outputFile,"\t%u",recommendedItems[k]);
			fprintf(outputFile,"\n");
		}
		timeElapsed -= clock();

		euclideanLSH_error += validateResults(train,test);
		lsh_terminate();
		fprintf(outputFile,"Execution Time: %f\n",1000*(((float)timeElapsed)/CLOCKS_PER_SEC));

		lsh_init(train,'c');

		fprintf(outputFile,"Cosine LSH fold number %d\n",i+1);

		numOfUsers = ratings_getNumberOfUsers(train);
		timeElapsed = clock();
		for(j=0;j<numOfUsers;j++){
			User currentUser = ratings_getUser(train,j+1);
			unsigned int* recommendedItems = lsh_getRecommendedItems(currentUser);
			fprintf(outputFile,"%u",user_getUserID(currentUser));
			unsigned int k;
			for(k=0;k<5;k++) fprintf(outputFile,"\t%u",recommendedItems[k]);
			fprintf(outputFile,"\n");
		}
		timeElapsed -= clock();

		cosineLSH_error += validateResults(train,test);
		clustering(train,'h',outputFilePath);
	    clustering(train,'e',outputFilePath);
        clustering(train,'c',outputFilePath);
		lsh_terminate();
		fprintf(outputFile,"Execution Time: %f\n",1000*(((float)timeElapsed)/CLOCKS_PER_SEC));
	}

	if(validate){
		outputFile = fopen(outputFilePath,"a");
		if(outputFile == NULL){
			perror("Failed to open output file to write validation results");
			return -1;
		}

		fprintf(outputFile,"Hamming LSH MAE: %lf\n",hammingLSH_error/NUM_OF_FOLDS);
		fprintf(outputFile,"Euclidean LSH MAE: %lf\n",euclideanLSH_error/NUM_OF_FOLDS);
		fprintf(outputFile,"Cosine LSH MAE: %lf\n",cosineLSH_error/NUM_OF_FOLDS);
	}
    fclose(outputFile);
	return 0;
}
