#include "Ratings.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BUFFER_BLOCK 4096

static unsigned int n; /*number of users*/
static unsigned int m; /*number of items*/

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
	unsigned short P; //number of nearest neighbors
	User* userRatings;
	unsigned int n;
	unsigned int m;
};

typedef struct RatingStruct* Rating;
struct RatingStruct{
	unsigned int userID;
	unsigned int itemID;
	int8_t rating;
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

	Ratings retVal = malloc(sizeof(struct RateData));
	if(retVal == NULL){
		perror("Failed to allocate memory for ratings");
		readLine(NULL);
		fclose(inputFile);
		return NULL;
	}

	List l;
	unsigned int prevUser;

	if(strcmp(token,"P:")){
		token = strtok(NULL," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file first line\n");
			readLine(NULL);
			fclose(inputFile);
			free(retVal);
			return NULL;
		}

		retVal->P = atoi(token);

		l = list_create();
		prevUser = 0;
		retVal->n = 0;
		retVal->m = 0;
	}
	else{
		retVal->P = 20;

		Rating newRating = malloc(sizeof(struct RatingStruct));
		if(newRating == NULL){
			perror("Failed to allocate memory for new User");
			readLine(NULL);
			fclose(inputFile);
			free(retVal);
			return NULL;
		}

		newRating->userID = atoi(token);
		prevUser = newRating->userID;
		retVal->n = 1;

		token = strtok(NULL," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file line %d\n",retVal->n+1);
			readLine(NULL);
			fclose(inputFile);
			free(retVal);
			return NULL;
		}

		newRating->itemID = atoi(token);
		retVal->m = newRating->itemID;

		token = strtok(NULL," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file line %d\n",retVal->n+1);
			readLine(NULL);
			fclose(inputFile);
			free(retVal);
			return NULL;
		}

		newRating->rating = atoi(token);
		l = list_create();
		list_pushEnd(l,newRating);

	}
		
	while(line = readLine(inputFile)){
		Rating newRating = malloc(sizeof(struct RatingStruct));
		if(newRating == NULL){
			perror("Failed to allocate memory for new User");
			readLine(NULL);
			fclose(inputFile);
			free(retVal);
			while(!list_isEmpty(l)) free(list_pop(l));
			list_destroy(l);
			return NULL;
		}

		token = strtok(line," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file line %d\n",retVal->n+1);
			readLine(NULL);
			fclose(inputFile);
			free(retVal);
			while(!list_isEmpty(l)) free(list_pop(l));
			list_destroy(l);
			free(newRating);
			return NULL;
		}

		newRating->userID = atoi(token);
		if(prevUser != newRating->userID){
			retVal->n++;
			prevUser = newRating->userID;
		}

		token = strtok(NULL," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file line %d\n",retVal->n+1);
			readLine(NULL);
			fclose(inputFile);
			free(retVal);
			while(!list_isEmpty(l)) free(list_pop(l));
			list_destroy(l);
			free(newRating);
			return NULL;
		}

		newRating->itemID = atoi(token);
		if(newRating->itemID > retVal->m) retVal->m = newRating->itemID;

		token = strtok(NULL," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file line %d\n",retVal->n+1);
			readLine(NULL);
			fclose(inputFile);
			free(retVal);
			while(!list_isEmpty(l)) free(list_pop(l));
			list_destroy(l);
			free(newRating);
			return NULL;
		}

		newRating->rating = atoi(token);
		list_pushEnd(l,newRating);
	}

	readLine(NULL);
	fclose(inputFile);

	retVal->userRatings = malloc(sizeof(User)*retVal->n);
	if(retVal->userRatings == NULL){
		perror("Failed to allocate memory for userRatings on ratings");
		free(retVal);
		while(!list_isEmpty(l)) free(list_pop(l));
		list_destroy(l);
		return NULL;
	}
	unsigned int i;
	for(i=0;i<retVal->n;i++) retVal->userRatings[i] = NULL;

	while(!list_isEmpty(l)){
		Rating currentRating = list_pop(l);
		User currentUser = retVal->userRatings[currentRating->userID-1];

		/*if user doesn't exist create one*/
		if(currentUser == NULL){
			currentUser = malloc(sizeof(struct UserStruct));
			if(retVal->userRatings[currentRating->userID-1] == NULL){
				perror("Failed to allocate memory for new user");
				for(i=0;i<retVal->n;i++) if(retVal->userRatings[i] != NULL){
					free(retVal->userRatings[i]->ratings);
					free(retVal->userRatings[i]);
				}
				free(retVal->userRatings);
				free(retVal);
				while(!list_isEmpty(l)) free(list_pop(l));
				list_destroy(l);
				return NULL;
			}

			currentUser->ratings = malloc(sizeof(int8_t)*retVal->m);
			if(currentUser->ratings == NULL){
				perror("Failed to allocate memory for new user ratings");
				for(i=0;i<retVal->n;i++) if(retVal->userRatings[i] != NULL){
					free(retVal->userRatings[i]->ratings);
					free(retVal->userRatings[i]);
				}
				free(currentUser);
				free(retVal->userRatings);
				free(retVal);
				while(!list_isEmpty(l)) free(list_pop(l));
				list_destroy(l);
				return NULL;
			}
		}

		currentUser->ratings[currentRating->itemID-1] = currentRating->rating;
		free(currentRating);
	}

	list_destroy(l);

	n = retVal->n;
	m = retVal->m;

	users = retVal->userRatings;

	return retVal;
}

void normalizeRatings(Ratings ratings,char metric){
	unsigned int i,j;
	if(metric == 'e' || metric == 'c'){
		unsigned int sum;
		unsigned int count;
		unsigned int avg;
		for(i=0;i<ratings->n;i++){
			sum = 0;
			count = 0;

			for(j=0;j<ratings->m;j++){
				if(ratings->userRatings[i]->flags[j]){
					sum += ratings->userRatings[i]->ratings[j];
					count++;
				}
			}

			avg = sum/count;

			for(j=0;j<ratings->m;j++){
				if(ratings->userRatings[i]->flags[j])
					ratings->userRatings[i]->ratings[j] -= avg;
				else
					ratings->userRatings[i]->ratings[j] = 0;
			}
		}
	}
	else if(metric == 'h'){
		for(i=0;i<ratings->n;i++)
			for(j=0;j<ratings->m;j++){
				if(ratings->userRatings[i]->flags[j]){
					if(ratings->userRatings[i]->ratings[j] > 2)
						ratings->userRatings[i]->ratings[j] = 1;
					else
						ratings->userRatings[i]->ratings[j] = 0;
				}
				else{
					ratings->userRatings[i]->ratings[j] = 0;
				}
			}
	}
	else{
		fprintf(stderr,"Invalid metric %c given on normalizeRatings\n",metric);
		exit(-1);
	}
}

void destroyUser(User user){
	free(user->ratings);
	free(user->flags);
	free(user);
}

void destroyRatings(Ratings ratings){
	unsigned int i;
	for(i=0;i<ratings->n;i++){
		destroyUser(ratings->userRatings[i]);
	}

	free(ratings);
}

static unsigned int currentUser = 0;

User getNextUser(Ratings ratingData){
	if(currentUser == ratingData->n){
		currentUser = 0;
		return NULL;
	}

	currentUser++;
	return ratingData->userRatings[currentUser-1];
}

static User* users;

unsigned int user_hammingDistance(unsigned int uid1,unsigned int uid2){
	User user1 = users[uid1];
	User user2 = users[uid2];
	
	unsigned int distance = 0;
	
	unsigned int i;
	for(i=0;i<m;i++){
		if(user1->ratings[i] != user2->ratings[i]) distance++;
	}

   	return distance;
}

double user_euclideanDistance(unsigned int uid1,unsigned int uid2){
	User user1 = users[uid1];
	User user2 = users[uid2];

	unsigned int i;
	double sum=0.0;
	int8_t* x = user1->ratings;
	int8_t* y = user2->ratings;

	for(i=0;i<m;i++)
	{
		sum+=((double)x[i]-(double)y[i])*((double)x[i]-(double)y[i]);
	}
	return sum;
}

double user_cosineDistance(unsigned int uid1,unsigned int uid2){
	User user1 = users[uid1];
	User user2 = users[uid2];

	unsigned int i;
	double xy=0.0,xx=0.0,yy=0.0;
	int8_t* x = user1->ratings;
	int8_t* y = user2->ratings;
	
	for(i=0;i<=m;i++)
	{
		xy+=(double)x[i]*(double)y[i];
	}
	for(i=0;i<=m;i++)
	{
		xx+=(double)x[i]*(double)x[i];
	}
	xx=sqrt(xx);
	for(i=0;i<=m;i++)
	{
	    yy+=(double)y[i]*(double)y[i];
	}
	yy=sqrt(yy);
	return 1-(xy/(xx*yy));
}

unsigned int ratings_getNumberOfUsers(Ratings ratings){
	return ratings->n;
}

unsigned short ratings_getNumberOfNeighbors(Ratings ratings){
	return ratings->P;
}

/*------------------HAMMING DATA-------------------*/
struct HammingData{
	int8_t* data;
	unsigned int size;
};

HammingData user_createHammingData(User user){
	HammingData retVal = malloc(sizeof(struct HammingData));
	if(retVal == NULL){
		perror("Failed to allocate memory for temporary data");
		return NULL;
	}

	retVal->data = user->ratings;
	retVal->size = m;

	return retVal;
}

void hammingData_destroy(HammingData data){
	free(data);
}

/*-----------------EUCLIDEAN DATA------------------*/
struct EuclideanData{
	int8_t* data;
	unsigned int sigId;
	unsigned int size;
	int sigId_flag;
};

EuclideanData user_createEuclideanData(User user){
	EuclideanData retVal = malloc(sizeof(struct EuclideanData));
	if(retVal == NULL){
		perror("Failed to allocate memory for temporary data");
		return NULL;
	}

	retVal->data = user->ratings;
	retVal->size = m;
	retVal->sigId_flag = 0;

	return retVal;
}

int euclideanData_isSet(EuclideanData data){
	return data->sigId_flag;
}

unsigned int euclideanData_getSigId(EuclideanData data){
	return data->sigId;
}

void euclideanData_setSigId(EuclideanData data){
	data->sigId
}

void euclideanData_destroy(EuclideanData data){
	free(data);
}

/*------------------COSINE DATA--------------------*/
struct HammingData{
	int8_t* data;
	unsigned int size;
};

HammingData user_createHammingData(User user){

}

void hammingData_destroy(HammingData data){

}