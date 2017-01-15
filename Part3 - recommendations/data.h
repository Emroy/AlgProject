#ifndef DATA_H
#define DATA_H
#include "Ratings.h"
typedef struct DataStruct* Data;

Data hammingData_create(User user);

Data euclideanData_create(User user);

Data cosineData_create(User user);

void data_destroy(Data d);

int8_t* data_getVector(Data d);

int data_vectorEnd(int8_t current);

int euclidean_data_is_set(Data d);

unsigned int euclidean_data_get_sigID(Data d);

void euclidean_data_set_sigID(Data d,unsigned int sigID);

unsigned int data_getUserID(Data d);

User data_getUser(Data d);

/*DISTANCES*/

unsigned int data_hammingDistance(Data a,Data b);
double data_euclideanDistance(Data a,Data b);
double data_cosineDistance(Data a,Data b);
#endif