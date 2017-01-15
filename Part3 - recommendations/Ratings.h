#ifndef RATINGS_H
#define RATINGS_H
#include <stdint.h>
#define RATINGS_END -10
#define NUM_OF_FOLDS 10
typedef struct RateData* Ratings;

typedef struct UserStruct* User;

/*--------------RATINGS-------------------*/
void readRatings(char* inputFilePath);

Ratings getRatingTrainSet();

Ratings getRatingTestSet();

double validateResults(Ratings train,Ratings test);

void normalizeRatings(Ratings ratingData,char metric);

void destroyRatings(Ratings ratings);

unsigned int ratings_getNumberOfUsers(Ratings ratings);

unsigned short ratings_getNumberOfNeighbors(Ratings ratings);

unsigned int ratings_calculateHammingDim(Ratings ratings);

unsigned int ratings_calculateEuclideanDim(Ratings ratings);

User ratings_getUser(Ratings ratings,unsigned int uid);

unsigned int ratings_getNumberOfItems(Ratings ratings);

/*---------------USER---------------------*/
User getNextUser(Ratings ratingData);
/*returns the next user. If no more users, returns NULL*/

unsigned int user_hammingDistance(unsigned int uid1,unsigned int uid2);

double user_euclideanDistance(unsigned int uid1,unsigned int uid2);

double user_cosineDistance(unsigned int uid1,unsigned int uid2);

unsigned int user_hammingDistance2(User user1,User user2);

double user_euclideanDistance2(User user1,User user2);

double user_cosineDistance2(User user1,User user2);

int8_t* user_getRatingsVector(User user);

int8_t* user_getRatingFlags(User user);

unsigned int user_getUserID(User user);
#endif