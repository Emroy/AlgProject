#ifndef DATA_H
#define DATA_H
#include <stdint.h>

typedef struct GenData *Data;

Data hamming_data_create(char* itemString);
/*itemString must cointain an id and a bitstring
  seperated by spaces and/or tabs.
  Returns Data cointaining a hamming element on success.
  Returns NULL on failure*/

int is_hamming_data(Data d);
/*returns 0 if d doesn't represent hamming data and
  a value != 0 otherwise.*/

unsigned int hamming_data_get_dimention();
/*Returns dimention if it has been set, 0 otherwise*/

uint64_t hamming_data_get_bits(Data d);
/*Get a 64-bit unsigned integer with the binary representation
  of the hamming element. Undefined Behavior if d is not a
  hamming element. Check with is_hamming_data beforehand.*/

Data euclidean_data_create(char* itemString);
/*itemString must cointain an id and the coordinates
  of the euclidean element in that order and seperated
  by spaces and/or tabs.
  Returns Data containing an euclidean element on success.
  Returns NULL on failure*/

unsigned int euclidean_data_get_dimention();
/*Returns dimention if it has been set, 0 otherwise*/

int is_euclidean_data(Data d);
/*returns 0 if d doesn't represent euclidean data and
  a value != 0 otherwise.*/

int euclidean_data_is_set(Data d);
/*returns 0 if id for this euclidean data hasn't been set
  and a value != 0 otherwise. Undefined behavior if the
  data pointed to by d is not an euclidean element.*/

double* euclidean_data_getVector(Data d);
/*Get the vector representation of the euclidean element d
  on the plane. If d is not a euclidean element the
  behavior is undefined.*/

unsigned int euclidean_data_get_sigID(Data d);
/*Get the signature ID of euclidean element represented
  by d. If d is not a euclidean element or the signature
  of said element is not set by a call to
  euclidean_data_set_sigID, the behaviour is undefined.*/

void euclidean_data_set_sigID(Data d,unsigned int sigID);
/*Set the signature id of euclidean element d to value
  sigID. This function should be called by hash_apply only.
  If d is not an euclidean element the behavior is undefined.*/

Data cosine_data_create(char* itemString);
/*itemString must cointain an id and the coordinates
  of the cosine element in that order and seperated
  by spaces and/or tabs.
  Returns Data containing an cosine element on success.
  Returns NULL on failure*/

unsigned int cosine_data_get_dimention();
/*Returns dimention if it has been set, 0 otherwise*/

int is_cosine_data(Data d);
/*returns 0 if d doesn't represent cosine data and
  a value != 0 otherwise.*/

double* cosine_data_getVector(Data d);
/*Get the vector representation of the cosine element d
  on the plane. If d is not a cosine element the
  behavior is undefined.*/

Data matrix_data_create(char* itemID);
/*itemID must contain a number representing the id of
  the matrix element. This id must be the same as the
  one cooresponding to the distance matrix.
  Non-numeric characters on itemID are ignored.
  Returns Data containing a matrix element on success.
  Returns NULL on failure.*/

int is_matrix_data(Data d);
/*returns 0 if d doesn't represent matrix data and
  a value != 0 otherwise.*/

unsigned int matrix_data_get_id(Data d);
/*Returns the identifier of matrix element d that
  represents it's position on the ditance matrix.
  If d is not a matrix element the behavior is undefined.*/

void data_destroy(Data d);
/*Destroys a Data element created by one of the following:
  hamming_data_create
  euclidean_data_create
  cosine_data_create
  matrix_data_create*/

void* data_distance(Data a,Data b);
/*Calculate the distance between element a and element b.
  a and b must be of the same type.
  Returns an unsigned int representing the distance if
  a and b are hamming or matrix elements and a double
  if a and b are euclidean or cosine elements.
  Returns NULL on failure.
  CAUTION: The return value of this function points to
  static memory and thus will CHANGE with subsequent
  calls of this function.*/

void data_create_distance_matrix(Data* data,unsigned int n);
/*Constructs a distance matrix so that data_distance can
  be executed faster. Works only for:
  Hamming Elements
  Euclidean Elements
  Cosine Elements*/

void data_set_distance_matrix(unsigned int** distanceMatrix);
/*Set distance matrix as the distance matrix for matrix metric.
  Work as*/

void data_destroy_distance_matrix();
/*Destroys a matrix created by data_create_distance_matrix.
  Use when a distance matrix is no longer needed.
  CAUTION: if more than one distance matrices are created, there
  is no guarantee that the first one created will be the first deleted.*/

unsigned int data_getIdDistance(uint64_t a,uint64_t b);
/*get the distance between data with id a and data with id b.
  Currently works only for matrix data.
  If data_set_distance_matrix was not called before this function
  the behavior is undefined.*/

uint64_t data_getID(Data d);
#endif