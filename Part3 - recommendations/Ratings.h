#ifndef RATINGS_H
#define RATINGS_H
#include <stdint.h>
typedef struct RateData* Ratings;

typedef struct UserStruct* User;

/*--------------RATINGS-------------------*/
Ratings readRatings(char* inputFilePath);

void normalizeRatings(Ratings ratingData,char metric);

void destroyRatings(Ratings ratings);

unsigned int ratings_getNumberOfUsers(Ratings ratings);

unsigned short ratings_getNumberOfNeighbors(Ratings ratings);

/*---------------USER---------------------*/
User getNextUser(Ratings ratingData);
/*returns the next user. If no more users, returns NULL*/

unsigned int user_hammingDistance(User user1,User user2);

double user_euclideanDistance(User user1,User user2);

double user_cosineDistance(User user1,User user2);

typedef struct HammingData* HammingData;
HammingData user_createHammingData(User user);
void hammingData_destroy(HammingData data);

typedef struct EuclideanData* EuclideanData;
EuclideanData user_createEuclideanData(User user);
int euclideanData_isSet(EuclideanData data);
unsigned int euclideanData_getSigId(EuclideanData data);
void euclideanData_setSigId(EuclideanData data);
void euclideanData_destroy(EuclideanData data);

typedef struct CosineData* CosineData;
CosineData user_createCosineData(User user);
void cosineData_destroy(CosineData data);
#endif