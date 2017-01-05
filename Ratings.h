#ifndef RATINGS_H
#define RATINGS_H
#include <stdint.h>
typedef struct RateData* Ratings;

typedef struct UserStruct* User;

Ratings readRatings(char* inputFilePath);

void normalizeRatings(Ratings ratingData,char metric);

User getNextUser(Ratings ratingData);
/*returns the next user. If no more users, returns NULL*/

int8_t* getUserVector(User user);

void destroyRatings(Ratings ratings);
#endif