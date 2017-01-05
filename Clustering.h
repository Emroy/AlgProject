#ifndef CLUSTERING_H
#define CLUSTERING_H
#include "Ratings.h"

/*NN-LSH*/
typedef struct NearestNeighbors* Neighbors;

void lsh_init(Ratings ratings,char metric);

Neighbors lsh_getNeighbors(User user);

void lsh_terminate();

/*Clustering*/
typedef struct Cluster* Cluster;

void clustering_init(Ratings ratings,char metric);

Cluster clustering_getNeighbors(User user);

void clustering_terminate();

#endif