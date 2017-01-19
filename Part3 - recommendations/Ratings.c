#include "Ratings.h"
#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BUFFER_BLOCK 4096

static unsigned int n; /*number of users*/
static unsigned int m; /*number of items*/

static unsigned short P;

static User* users;

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

typedef struct AllRatings* AllRatings;
struct AllRatings{
	Rating* ratings;
	unsigned int numberOfRatings;

};

static AllRatings allRatings = NULL;

double validateResults(Ratings train,Ratings test){
	unsigned int i;
	unsigned int j;

	double sum = 0.0;
	for(i=0;i<n;i++){
		for(j=0;j<m;j++)
			if(test->userRatings[i]->flags[j]){
				if((test->userRatings[i]->ratings[j] - train->userRatings[i]->ratings[j]) >= 0){
					sum += (double)(test->userRatings[i]->ratings[j] - train->userRatings[i]->ratings[j]);
				}
				else{
					sum += (double)(train->userRatings[i]->ratings[j] - test->userRatings[i]->ratings[j]);
				}
			}
	}

	return sum/(double)(allRatings->numberOfRatings/NUM_OF_FOLDS);
}

static unsigned short foldCount = 0;

Ratings getRatingTestSet(){
	Ratings retVal = malloc(sizeof(struct RateData));
	if(retVal == NULL){
		perror("Failed to create training set");
		return NULL;
	}

	retVal->n = n;
	retVal->m = m;

	retVal->userRatings = malloc(sizeof(User)*retVal->n);
	if(retVal->userRatings == NULL){
		perror("Failed to allocate memory for userRatings on ratings");
		free(retVal);
		return NULL;
	}

	unsigned int i;
	unsigned int j;
	for(i=0;i<retVal->n;i++) retVal->userRatings[i] = NULL;

	unsigned int testRangeLow = foldCount*(allRatings->numberOfRatings/NUM_OF_FOLDS);
	unsigned int testRangeHigh = (foldCount+1)*(allRatings->numberOfRatings/NUM_OF_FOLDS);
	for(i=0;i<allRatings->numberOfRatings;i++){
		if(i>=testRangeLow && i < testRangeHigh){
			Rating currentRating = allRatings->ratings[i];
			User currentUser = retVal->userRatings[currentRating->userID-1];

			/*if user doesn't exist create one*/
			if(retVal->userRatings[currentRating->userID-1] == NULL){
				retVal->userRatings[currentRating->userID-1] = malloc(sizeof(struct UserStruct));
				if(retVal->userRatings[currentRating->userID-1] == NULL){
					perror("Failed to allocate memory for new user");
					for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
						free(retVal->userRatings[j]->ratings);
						free(retVal->userRatings[j]);
					}
					free(retVal->userRatings);
					free(retVal);
					return NULL;
				}

				currentUser = retVal->userRatings[currentRating->userID-1];

				currentUser->ratings = malloc(sizeof(int8_t)*retVal->m+1);
				if(currentUser->ratings == NULL){
					perror("Failed to allocate memory for new user ratings");
					for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
						free(retVal->userRatings[j]->ratings);
						free(retVal->userRatings[j]->flags);
						free(retVal->userRatings[j]);
					}
					free(currentUser);
					free(retVal->userRatings);
					free(retVal);
					return NULL;
				}

				currentUser->flags = malloc(sizeof(int8_t)*retVal->m);
				if(currentUser->flags == NULL){
					perror("Failed to allocate memory for new user ratings");
					for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
						free(retVal->userRatings[j]->ratings);
						free(retVal->userRatings[j]->flags);
						free(retVal->userRatings[j]);
					}
					free(currentUser->ratings);
					free(currentUser);
					free(retVal->userRatings);
					free(retVal);
					return NULL;
				}

				for(j=0;j<retVal->m;j++) currentUser->flags[j] = 0;
			}

			currentUser->ratings[currentRating->itemID-1] = currentRating->rating;
			currentUser->flags[currentRating->itemID-1] = 1;
		}
	}

	for(i=0;i<retVal->n;i++){
		if(retVal->userRatings[i]) retVal->userRatings[i]->ratings[retVal->m] = RATINGS_END;
		else{
			retVal->userRatings[i] = malloc(sizeof(struct UserStruct));
			if(retVal->userRatings[i] == NULL){
				perror("Failed to create user ratings on set creation");
				for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
					free(retVal->userRatings[j]->ratings);
					free(retVal->userRatings[j]->flags);
					free(retVal->userRatings[j]);
				}
				free(retVal->userRatings);
				free(retVal);
				return NULL;
			}

			retVal->userRatings[i]->ratings = malloc(sizeof(int8_t)*(retVal->m+1));
			if(retVal->userRatings[i]->ratings == NULL){
				perror("Failed to create user ratings on set creation");
				for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
					free(retVal->userRatings[j]->ratings);
					free(retVal->userRatings[j]->flags);
					free(retVal->userRatings[j]);
				}
				free(retVal->userRatings);
				free(retVal);
				return NULL;
			}

			retVal->userRatings[i]->flags = malloc(sizeof(int8_t)*(retVal->m));
			if(retVal->userRatings[i]->flags == NULL){
				perror("Failed to create user ratings on set creation");
				for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
					free(retVal->userRatings[j]->ratings);
					free(retVal->userRatings[j]->flags);
					free(retVal->userRatings[j]);
				}
				free(retVal->userRatings);
				free(retVal);
				return NULL;
			}

			for(j=0;j<retVal->m;j++){
				retVal->userRatings[i]->flags[j] = 0;
			}

			retVal->userRatings[i]->ratings[retVal->m] = RATINGS_END;
		}
		retVal->userRatings[i]->id = i+1;
	}

	foldCount++;

	return retVal;
}

Ratings getRatingTrainSet(){
	Ratings retVal = malloc(sizeof(struct RateData));
	if(retVal == NULL){
		perror("Failed to create training set");
		return NULL;
	}

	retVal->n = n;
	retVal->m = m;

	retVal->userRatings = malloc(sizeof(User)*retVal->n);
	if(retVal->userRatings == NULL){
		perror("Failed to allocate memory for userRatings on ratings");
		free(retVal);
		return NULL;
	}

	unsigned int i;
	unsigned int j;
	for(i=0;i<retVal->n;i++) retVal->userRatings[i] = NULL;

	unsigned int testRangeLow = foldCount*(allRatings->numberOfRatings/NUM_OF_FOLDS);
	unsigned int testRangeHigh = (foldCount+1)*(allRatings->numberOfRatings/NUM_OF_FOLDS);
	for(i=0;i<allRatings->numberOfRatings;i++){
		if(i>=testRangeLow && i < testRangeHigh) continue;

		Rating currentRating = allRatings->ratings[i];
		User currentUser = retVal->userRatings[currentRating->userID-1];

		/*if user doesn't exist create one*/
		if(retVal->userRatings[currentRating->userID-1] == NULL){
			retVal->userRatings[currentRating->userID-1] = malloc(sizeof(struct UserStruct));
			if(retVal->userRatings[currentRating->userID-1] == NULL){
				perror("Failed to allocate memory for new user");
				for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
					free(retVal->userRatings[j]->ratings);
					free(retVal->userRatings[j]);
				}
				free(retVal->userRatings);
				free(retVal);
				return NULL;
			}

			currentUser = retVal->userRatings[currentRating->userID-1];

			currentUser->ratings = malloc(sizeof(int8_t)*(retVal->m+1));
			if(currentUser->ratings == NULL){
				perror("Failed to allocate memory for new user ratings");
				for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
					free(retVal->userRatings[j]->ratings);
					free(retVal->userRatings[j]->flags);
					free(retVal->userRatings[j]);
				}
				free(currentUser);
				free(retVal->userRatings);
				free(retVal);
				return NULL;
			}

			currentUser->flags = malloc(sizeof(int8_t)*retVal->m);
			if(currentUser->flags == NULL){
				perror("Failed to allocate memory for new user ratings");
				for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
					free(retVal->userRatings[j]->ratings);
					free(retVal->userRatings[j]->flags);
					free(retVal->userRatings[j]);
				}
				free(currentUser->ratings);
				free(currentUser);
				free(retVal->userRatings);
				free(retVal);
				return NULL;
			}

			for(j=0;j<retVal->m;j++) currentUser->flags[j] = 0;
		}

		currentUser->ratings[currentRating->itemID-1] = currentRating->rating;
		currentUser->flags[currentRating->itemID-1] = 1;
	}

	for(i=0;i<retVal->n;i++){
		if(retVal->userRatings[i]) retVal->userRatings[i]->ratings[retVal->m] = RATINGS_END;
		else{
			retVal->userRatings[i] = malloc(sizeof(struct UserStruct));
			if(retVal->userRatings[i] == NULL){
				perror("Failed to create user ratings on set creation");
				for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
					free(retVal->userRatings[j]->ratings);
					free(retVal->userRatings[j]->flags);
					free(retVal->userRatings[j]);
				}
				free(retVal->userRatings);
				free(retVal);
				return NULL;
			}

			retVal->userRatings[i]->ratings = malloc(sizeof(int8_t)*(retVal->m+1));
			if(retVal->userRatings[i]->ratings == NULL){
				perror("Failed to create user ratings on set creation");
				for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
					free(retVal->userRatings[j]->ratings);
					free(retVal->userRatings[j]->flags);
					free(retVal->userRatings[j]);
				}
				free(retVal->userRatings);
				free(retVal);
				return NULL;
			}

			retVal->userRatings[i]->flags = malloc(sizeof(int8_t)*(retVal->m));
			if(retVal->userRatings[i]->flags == NULL){
				perror("Failed to create user ratings on set creation");
				for(j=0;j<retVal->n;j++) if(retVal->userRatings[j] != NULL){
					free(retVal->userRatings[j]->ratings);
					free(retVal->userRatings[j]->flags);
					free(retVal->userRatings[j]);
				}
				free(retVal->userRatings);
				free(retVal);
				return NULL;
			}

			for(j=0;j<retVal->m;j++){
				retVal->userRatings[i]->flags[j] = 0;
			}

			retVal->userRatings[i]->ratings[retVal->m] = RATINGS_END;
		}

		retVal->userRatings[i]->id = i+1;
	}

	users = retVal->userRatings;

	return retVal;
}

void readRatings(char* inputFilePath){
	FILE* inputFile = fopen(inputFilePath,"r");
	if(inputFile == NULL){
		perror("Failed to open input file");
		return;
	}

	char* line = readLine(inputFile);
	if(line == NULL){
		perror("Failed to read first line from input file");
		readLine(NULL);
		fclose(inputFile);
		return;
	}

	char* token = strtok(line," \t\n");
	if(token == NULL){
		fprintf(stderr,"Unexpected input encountered on input file first line\n");
		readLine(NULL);
		fclose(inputFile);
		return;
	}

	if(allRatings) free(allRatings);
	allRatings = malloc(sizeof(struct AllRatings));
	if(allRatings == NULL){
		perror("Failed to allocate memory for ratings");
		readLine(NULL);
		fclose(inputFile);
		return;
	}

	List l;
	unsigned int prevUser;

	if(strcmp(token,"P:")){
		token = strtok(NULL," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file first line\n");
			readLine(NULL);
			fclose(inputFile);
			free(allRatings);
			allRatings = NULL;
			return;
		}

		P = atoi(token);

		l = list_create();
		prevUser = 0;
		n = 0;
		m = 0;
	}
	else{
		P = 20;

		Rating newRating = malloc(sizeof(struct RatingStruct));
		if(newRating == NULL){
			perror("Failed to allocate memory for new User");
			readLine(NULL);
			fclose(inputFile);
			free(allRatings);
			allRatings = NULL;
			return;
		}

		newRating->userID = atoi(token);
		prevUser = newRating->userID;
		n = 1;

		token = strtok(NULL," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file line %d\n",n+1);
			readLine(NULL);
			fclose(inputFile);
			free(allRatings);
			allRatings = NULL;
			return;
		}

		newRating->itemID = atoi(token);
		m = newRating->itemID;

		token = strtok(NULL," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file line %d\n",n+1);
			readLine(NULL);
			fclose(inputFile);
			free(allRatings);
			allRatings = NULL;
			return;
		}

		newRating->rating = atoi(token);
		l = list_create();
		list_pushEnd(l,newRating);

	}

	allRatings->numberOfRatings = 0;
		
	while(line = readLine(inputFile)){
		token = strtok(line," \t\n");
		if(token == NULL){
			break;
		}

		Rating newRating = malloc(sizeof(struct RatingStruct));
		if(newRating == NULL){
			perror("Failed to allocate memory for new User");
			readLine(NULL);
			fclose(inputFile);
			free(allRatings);
			allRatings = NULL;
			while(!list_isEmpty(l)) free(list_pop(l));
			list_destroy(l);
			return;
		}

		newRating->userID = atoi(token);
		if(prevUser != newRating->userID){
			n++;
			prevUser = newRating->userID;
		}

		token = strtok(NULL," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file line %d\n",allRatings->numberOfRatings+2);
			readLine(NULL);
			fclose(inputFile);
			free(allRatings);
			allRatings = NULL;
			while(!list_isEmpty(l)) free(list_pop(l));
			list_destroy(l);
			free(newRating);
			return;
		}

		newRating->itemID = atoi(token);
		if(newRating->itemID > m) m = newRating->itemID;

		token = strtok(NULL," \t\n");
		if(token == NULL){
			fprintf(stderr,"Unexpected input encountered on input file line %d\n",allRatings->numberOfRatings+2);
			readLine(NULL);
			fclose(inputFile);
			free(allRatings);
			allRatings = NULL;
			while(!list_isEmpty(l)) free(list_pop(l));
			list_destroy(l);
			free(newRating);
			return;
		}

		newRating->rating = atoi(token);
		list_pushEnd(l,newRating);
		allRatings->numberOfRatings++;
	}

	readLine(NULL);
	fclose(inputFile);

	allRatings->ratings = malloc(sizeof(Rating)*allRatings->numberOfRatings);
	if(allRatings == NULL){
		perror("Failed to allocate memory for ratings placeholder (read)");
		return;
	}

	unsigned int i;
	for(i=0;i<allRatings->numberOfRatings;i++){
		allRatings->ratings[i] = list_pop(l);
	}
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

unsigned int user_hammingDistance(unsigned int uid1,unsigned int uid2){
	return user_hammingDistance2(users[uid1-1],users[uid2-1]);
}

double user_euclideanDistance(unsigned int uid1,unsigned int uid2){
	return user_euclideanDistance2(users[uid1-1],users[uid2-1]);
}

double user_cosineDistance(unsigned int uid1,unsigned int uid2){
	return user_cosineDistance2(users[uid1-1],users[uid2-1]);
}

unsigned int user_hammingDistance2(User user1,User user2){
	unsigned int distance = 0;
	
	unsigned int i;
	for(i=0;i<m;i++){
		if(user1->ratings[i] != user2->ratings[i]) distance++;
	}

   	return distance;
}

double user_euclideanDistance2(User user1,User user2){
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

double user_cosineDistance2(User user1,User user2){
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
	return P;
}

unsigned int ratings_calculateHammingDim(Ratings ratings){
	return ratings->m;
}

unsigned int ratings_calculateEuclideanDim(Ratings ratings){
	return ratings->m;
}

int8_t* user_getRatingsVector(User user){
	return user->ratings;
}

User ratings_getUser(Ratings ratings,unsigned int uid){
	return ratings->userRatings[uid-1];
}

unsigned int ratings_getNumberOfItems(Ratings ratings){
	return ratings->m;
}

int8_t* user_getRatingFlags(User user){
	return user->flags;
}

unsigned int user_getUserID(User user){
	return user->id;
}