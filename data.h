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

unsigned int hamming_data_distance(HammingData a,HammingData b);
/*Calculate the distance between HammingData a and b*/

void hamming_data_destroy(HammingData data);
/*Destroy Hamming Data created by hamming_data_create*/

/*------------EUCLIDEAN DATA-----------*/
EuclideanData euclidean_data_create(HashDescriptor hd,double* p);
/*Create new Euclidean Data*/

double euclidean_data_distance(EuclideanData a,EuclideanData b);
/*Calculate the distance between EuclideanData a and b*/

void euclidean_data_destroy(EuclideanData data);
/*Derstroy Euclidean Data created by euclidean_data_create*/

/*------------COSINE DATA--------------*/
CosineData cosine_data_create(double* p);
/*Create new Cosine Data*/

double cosine_data_distance(CosineData a,CosineData b);
/*Calculate the distance between CosineData a and b*/

void cosine_data_destroy(CosineData data);
/*Destroy Cosine Data created by cosine_data_create*/

/*------------MATRIX DATA--------------*/
void matrix_data_set_distance_matrix(unsigned int** distance_matr);
/*Set a common distance matrix for all Matrix Data
  Memory management for the distance matrix is left to the caller*/

MatrixData matrix_data_create();
/*Create new Matrix Data*/

unsigned int matrix_data_distance(MatrixData a,MatrixData b);
/*Get distance of point a from point b from the ditance matrix*/

void matrix_data_destroy(MatrixData data);
/*Destroy Matrix Data created by matrix_data_create*/
#endif