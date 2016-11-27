#ifndef DATA_H
#define DATA_H
typedef GenericData* Data;

Data hamming_data_create(char* itemString);
/*Returns Data */

Data euclidean_data_create(char* itemString);

Data cosine_data_create(char* itemString);

Data matrix_data_create(char* itemID);

void data_destroy(Data d);

void* data_distance(Data a,Data b);

void data_create_distance_matrix(Data* data,int n);

void data_set_distance_matrix(unsigned int** distanceMatrix,int n);

void data_destroy_distance_matrix();
#endif