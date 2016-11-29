#ifndef DATA_H
#define DATA_H
typedef struct GenData *Data;

Data hamming_data_create(char* itemString);
/*itemString must cointain an id and a bitstring
  seperated by spaces and/or tabs.
  Returns Data cointaining a hamming element on success.
  Returns NULL on failure*/

Data euclidean_data_create(char* itemString);
/*itemString must cointain an id and the coordinates
  of the euclidean element in that order and seperated
  by spaces and/or tabs.
  Returns Data containing an euclidean element on success.
  Returns NULL on failure*/

unsigned int euclidean_data_get_dimention();
/*Returns dimention if it has been set, 0 otherwise*/

Data cosine_data_create(char* itemString);
/*itemString must cointain an id and the coordinates
  of the cosine element in that order and seperated
  by spaces and/or tabs.
  Returns Data containing an cosine element on success.
  Returns NULL on failure*/

unsigned int cosine_data_get_dimention();
/*Returns dimention if it has been set, 0 otherwise*/

Data matrix_data_create(char* itemID);
/*itemID must contain a number representing the id of
  the matrix element. This id must be the same as the
  one cooresponding to the distance matrix.
  Non-numeric characters on itemID are ignored.
  Returns Data containing a matrix element on success.
  Returns NULL on failure.*/

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

void data_set_distance_matrix(unsigned int** distanceMatrix,int n);
/*Under Construction*/

void data_destroy_distance_matrix();
/*Destroys a matrix created by data_create_distance_matrix.
  Use when a distance matrix is no longer needed.
  CAUTION: if more than one distance matrices are created, there
  is no guarantee that the first one created will be the first deleted.*/
#endif