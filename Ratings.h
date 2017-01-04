#ifndef RATINGS_H
#define RATINGS_H
typedef struct RateData* Ratings;

Ratings readRatings(char* inputFilePath);

void normalizeRatings(Ratings ratingData,char metric);

void destroyRatings(Ratings ratings);
#endif