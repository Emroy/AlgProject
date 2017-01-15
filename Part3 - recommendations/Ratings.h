#ifndef RATINGS_H
#define RATINGS_H
#include <stdint.h>
#define RATINGS_END -10
typedef struct RateData* Ratings;

typedef struct UserStruct* User;

/*--------------RATINGS-------------------*/
Ratings readRatings(char* inputFilePath);

void normalizeRatings(Ratings ratingData,char metric);

void destroyRatings(Ratings ratings);

unsigned int ratings_getNumberOfUsers(Ratings ratings);

unsigned short ratings_getNumberOfNeighbors(Ratings ratings);

unsigned int ratings_calculateHammingDim(Ratings ratings);

unsigned int ratings_calculateEuclideanDim(Ratings ratings);

User ratings_getUser(Ratings ratings,unsigned int uid);

unsigned short ratings_getNeighborsNum(Ratings ratings);

/*---------------USER---------------------*/
User getNextUser(Ratings ratingData);
/*returns the next user. If no more users, returns NULL*/

unsigned int user_hammingDistance(unsigned int uid1,unsigned int uid2);

double user_euclideanDistance(unsigned int uid1,unsigned int uid2);

double user_cosineDistance(unsigned int uid1,unsigned int uid2);

int8_t* user_getRatingsVector(User user);

int8_t* user_getRatingFlags(User user);
#endif