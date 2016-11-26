#ifndef DATA_H
#define DATA_H
#include "HashGen.h"

typedef struct HammingData *HammingData;
typedef struct EucliudeanData *EuclideanData;
typedef struct CosineData *CosineData;
typedef struct MatrixData *MatrixData;

/*------------HAMMING DATA-------------*/
HammingData hamming_data_create(char* bitString);
/*Create new Hamming Data.
  bitString is a sequence of 01 characters representing hamming bits
  bitString can contain other characters which will be ignored
  bitString cannot contain more than 64 01 characters.
  RETURNS: new Hamming Data on success or NULL on error*/

void hamming_data_destroy(HammingData data);
/*Destroy Hamming Data created by hamming_data_create*/

/*------------EUCLIDEAN DATA-----------*/
EuclideanData euclidean_data_create(HashDescriptor hd,double* p);
/*Create new Euclidean Data*/

void euclidean_data_destroy(EuclideanData data);
/*Derstroy Euclidean Data created by euclidean_data_create*/

/*------------COSINE DATA--------------*/
CosineData cosine_data_create(double* p);
/*Create new Cosine Data*/

void cosine_data_destroy(CosineData data);
/*Destroy Cosine Data created by cosine_data_create*/

/*------------MATRIX DATA--------------*/
void matrix_data_set_distance_matrix(unsigned int** distance_matr);
/*Set a common distance matrix for all Matrix Data
  Memory management for the distance matrix is left to the caller*/

MatrixData matrix_data_create();
/*Create new Matrix Data*/

void matrix_data_destroy(MatrixData data);
/*Destroy Matrix Data created by matrix_data_create*/
#endif