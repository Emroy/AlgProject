#include "data.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "List.h"

/*------------HAMMING DATA-------------*/
typedef struct HammingData{
	uint64_t id;
	uint64_t bits;
}HammingData;

static unsigned int** hamming_distance_matrix = NULL;
static unsigned int hamming_distance_matrix_size = 0;
static unsigned int hamming_dimention = 0;

unsigned int hamming_data_distance(HammingData* a,HammingData* b);
/*Calculate the distance between HammingData a and b*/

/*------------EUCLIDEAN DATA-----------*/
typedef struct EucliudeanData{
	uint64_t id; 		/*the number of this element*/
	unsigned int ID; 	/*calculated by hashFunction*/
	uint8_t id_flag;	/*0 if ID is not set, 1 otherwise*/
	double* vector;
}EuclideanData;

static unsigned short euclidean_dim = 0;
static double** euclidean_distance_matrix = NULL;
static unsigned int euclidean_distance_matrix_size = 0;

double euclidean_data_distance(EuclideanData* a,EuclideanData* b);
/*Calculate the distance between EuclideanData a and b*/

unsigned int euclidean_data_get_dimention(){
	return euclidean_dim;
}

/*-----------COSINE DATA---------------*/
typedef struct CosineData{
	uint64_t id;
	double* vector;
}CosineData;

static unsigned short cosine_dim = 0;
static double** cosine_distance_matrix = NULL;
static unsigned int cosine_distance_matrix_size = 0;

double cosine_data_distance(CosineData* a,CosineData* b);
/*Calculate the distance between CosineData a and b*/

unsigned int cosine_data_get_dimention(){
	return cosine_dim;
}

/*--------------MATRIX DATA------------*/
typedef struct MatrixData{
	uint64_t id;
}MatrixData;

static unsigned int** matrix_distance_matrix = NULL;
static unsigned int matrix_distance_matrix_size = 0;

/*--------------GENERIC DATA-----------*/
typedef struct GenData{
	HammingData* hData;
	EuclideanData* eData;
	CosineData* cData;
	MatrixData* mData;
}GenericData;

Data hamming_data_create(char* itemString){
	Data retVal = malloc(sizeof(GenericData));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Data");
		return NULL;
	}

	retVal->hData = malloc(sizeof(HammingData));
	if(retVal->hData == NULL){
		perror("Failed to allocate memory for new Hamming Data");
		free(retVal);
		return NULL;
	}

	retVal->eData = NULL;
	retVal->cData = NULL;
	retVal->mData = NULL;

	char* itemID = strtok(itemString," \t");
	char* bitString = strtok(NULL," \t");

	retVal->hData->id = 0;
	unsigned short int i = 0;
	while(itemID[i]){
		if(itemID[i] >= '0' && itemID[i] <= '9'){
			retVal->hData->id *= 10;
			retVal->hData->id += itemID[i]-'0';
		}
		i++;
	}

	retVal->hData->bits = 0;
	unsigned short int bitCount = 0;
	i=0;
	while(bitString[i]){
		if(bitString[i] == '1'){
			retVal->hData->bits = retVal->hData->bits << 1;
			retVal->hData->bits++;
			bitCount++;
		}
		else if(bitString[i] == '0'){
			retVal->hData->bits = retVal->hData->bits << 1;
			bitCount++;
		}
		if(bitCount > 64){
			fprintf(stderr,"hamming_data_create: Can't have hamming data longer than 64 bits\n");
			free(retVal->hData);
			free(retVal);
			return NULL;
		}

		i++;
	}

	if(hamming_dimention == 0) hamming_dimention = bitCount;

	return retVal;
}

Data euclidean_data_create(char* itemString){
	Data retVal = malloc(sizeof(GenericData));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Data");
		return NULL;
	}

	retVal->eData = malloc(sizeof(EuclideanData));
	if(retVal->eData == NULL){
		perror("Failed to allocate memory for new Euclidean Data");
		free(retVal);
		return NULL;
	}

	retVal->hData = NULL;
	retVal->cData = NULL;
	retVal->mData = NULL;

	char* itemID = strtok(itemString," \t");
	char* vectorString = strtok(NULL," \t");

	/*set item id*/
	retVal->eData->id_flag = 0;
	retVal->eData->id = 0;
	unsigned short i = 0;
	while(itemID[i]){
		if(itemID[i] >= '0' && itemID[i] <= '9'){
			retVal->eData->id *= 10;
			retVal->eData->id += itemID[i]-'0';
		}
		i++;
	}

	if(euclidean_dim == 0){
		List l = list_create();
		char* str;
		while(str = strtok(NULL," \t")){
			double* element = malloc(sizeof(double));
			if(element == NULL){
				perror("Failed to allocate memory for Euclidean Data vector");
				free(retVal->eData);
				free(retVal);
				while(!list_isEmpty(l)) free(list_pop(l));
				list_destroy(l);
				return NULL;
			}
			*element = strtod(str,NULL);
			list_pushEnd(l,element);
		}

		euclidean_dim = list_length(l);
		retVal->eData->vector = malloc(euclidean_dim*sizeof(double));
		if(retVal->eData->vector == NULL){
			perror("Failed to allocate memory for Euclidean Data vector");
			free(retVal->eData);
			free(retVal);
			while(!list_isEmpty(l)) free(list_pop(l));
			list_destroy(l);
			return NULL;
		}

		i=0;
		while(!list_isEmpty(l)){
			double* temp = (double*)list_pop(l);
			retVal->eData->vector[i] = *temp;
			free(temp);
			i++;
		}

		list_destroy(l);
		return retVal;
	}

	retVal->eData->vector = malloc(euclidean_dim*sizeof(double));
	if(retVal->eData->vector == NULL){
		perror("Failed to allocate memory for Euclidean Data vector");
		free(retVal->eData);
		free(retVal);
		return NULL;
	}

	char* vec_element;
	for(i=0;i<euclidean_dim;i++){
		vec_element = strtok(NULL," \t");
		if(vec_element == NULL){
			fprintf(stderr,"Inconsistent dimention given on Euclidean Data vectors\n");
			free(retVal->eData->vector);
			free(retVal->eData);
			free(retVal);
			return NULL;
		}
		retVal->eData->vector[i] = strtod(vec_element,NULL);
	}

	return retVal;
}

Data cosine_data_create(char* itemString){
	Data retVal = malloc(sizeof(GenericData));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Data");
		return NULL;
	}

	retVal->cData = malloc(sizeof(CosineData));
	if(retVal->cData == NULL){
		perror("Failed to allocate memory for new Euclidean Data");
		free(retVal);
		return NULL;
	}

	retVal->hData = NULL;
	retVal->eData = NULL;
	retVal->mData = NULL;

	char* itemID = strtok(itemString," \t");
	char* vectorString = strtok(NULL," \t");

	/*set item id*/
	retVal->cData->id = 0;
	unsigned short i = 0;
	while(itemID[i]){
		if(itemID[i] >= '0' && itemID[i] <= '9'){
			retVal->cData->id *= 10;
			retVal->cData->id += itemID[i]-'0';
		}
		i++;
	}

	if(cosine_dim == 0){
		List l = list_create();
		char* str;
		while(str = strtok(NULL," \t")){
			double* element = malloc(sizeof(double));
			if(element == NULL){
				perror("Failed to allocate memory for Euclidean Data vector");
				free(retVal->cData);
				free(retVal);
				while(!list_isEmpty(l)) free(list_pop(l));
				list_destroy(l);
				return NULL;
			}
			*element = strtod(str,NULL);
			list_pushEnd(l,element);
		}

		cosine_dim = list_length(l);
		retVal->cData->vector = malloc(cosine_dim*sizeof(double));
		if(retVal->cData->vector == NULL){
			perror("Failed to allocate memory for Euclidean Data vector");
			free(retVal->cData);
			free(retVal);
			while(!list_isEmpty(l)) free(list_pop(l));
			list_destroy(l);
			return NULL;
		}

		i=0;
		while(!list_isEmpty(l)){
			double* temp = (double*)list_pop(l);
			retVal->cData->vector[i] = *temp;
			free(temp);
			i++;
		}

		list_destroy(l);
		return retVal;
	}

	retVal->cData->vector = malloc(cosine_dim*sizeof(double));
	if(retVal->cData->vector == NULL){
		perror("Failed to allocate memory for Euclidean Data vector");
		free(retVal->cData);
		free(retVal);
		return NULL;
	}

	char* vec_element;
	for(i=0;i<cosine_dim;i++){
		vec_element = strtok(NULL," \t");
		if(vec_element == NULL){
			fprintf(stderr,"Inconsistent dimention given on Euclidean Data vectors\n");
			free(retVal->cData->vector);
			free(retVal->cData);
			free(retVal);
			return NULL;
		}
		retVal->cData->vector[i] = strtod(vec_element,NULL);
	}

	return retVal;
}

Data matrix_data_create(char* itemID){
	Data retVal = malloc(sizeof(GenericData));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Data");
		return NULL;
	}

	retVal->mData = malloc(sizeof(MatrixData));
	if(retVal->mData == NULL){
		perror("Failed to allocate memory for new Matrix Data");
		free(retVal);
		return NULL;
	}

	retVal->hData = NULL;
	retVal->eData = NULL;
	retVal->cData = NULL;

	retVal->mData->id = 0;
	unsigned short i = 0;
	while(itemID[i]){
		if(itemID[i] >= '0' && itemID[i] <= '9'){
			retVal->mData->id *= 10;
			retVal->mData->id += itemID[i]-'0';
		}
		i++;
	}

	return retVal;
}

void data_destroy(Data d){
	if(d->hData) free(d->hData);
	else if(d->eData){
		free(d->eData->vector);
		free(d->eData);
	}
	else if(d->cData){
		free(d->cData->vector);
		free(d->cData);
	}
	else if(d->mData) free(d->mData);

	free(d);
}

void* data_distance(Data a,Data b){
	static unsigned int uRetVal;
	static double dRetVal;

	if(a->hData){
		if(b->hData == NULL){
			fprintf(stderr,"Could not calculate sitance: Data types of a and b are incompatible\n");
			return NULL;
		}

		if(hamming_distance_matrix)
			uRetVal = hamming_distance_matrix[a->hData->id][b->hData->id];
		else
			uRetVal = hamming_data_distance(a->hData,b->hData);

		return &uRetVal;
	}
	else if(a->eData){
		if(b->eData == NULL){
			fprintf(stderr,"Could not calculate sitance: Data types of a and b are incompatible\n");
			return NULL;
		}

		if(euclidean_distance_matrix)
			dRetVal = euclidean_distance_matrix[a->eData->id][b->eData->id];
		else
			dRetVal = euclidean_data_distance(a->eData,b->eData);

		return &dRetVal;

	}
	else if(a->cData){
		if(b->cData == NULL){
			fprintf(stderr,"Could not calculate sitance: Data types of a and b are incompatible\n");
			return NULL;
		}

		if(cosine_distance_matrix)
			dRetVal = cosine_distance_matrix[a->cData->id][b->cData->id];
		else
			dRetVal = cosine_data_distance(a->cData,b->cData);

		return &dRetVal;

	}
	else if(a->mData){
		if(b->mData == NULL){
			fprintf(stderr,"Could not calculate sitance: Data types of a and b are incompatible\n");
			return NULL;
		}

		if(matrix_distance_matrix == NULL){
			fprintf(stderr,"Can't calculate distance on matrix Data without a distance matrix\n");
			return NULL;
		}

		uRetVal = matrix_distance_matrix[a->mData->id][b->mData->id];
		return &uRetVal;
	}
	else{
		fprintf(stderr,"Invalid Data given on data_distance\n");
		return NULL;
	}
}

void data_create_distance_matrix(Data* data,unsigned int n){
	unsigned int i,j;

	if(data[0]->hData){
		hamming_distance_matrix = malloc(n*sizeof(unsigned int*));
		if(hamming_distance_matrix == NULL){
			perror("Failed to allocate memory for new distance matrix");
			return;
		}

		for(i=0;i<n;i++){
			hamming_distance_matrix[i] = malloc(n*sizeof(unsigned int));
			if(hamming_distance_matrix[i] == NULL){
				perror("Failed to allocate memory for new distance matrix");
				return;
			}
		}

		for(i=0;i<n;i++)
			for(j=0;j<n;j++){
				if(i==j){
					hamming_distance_matrix[i][j] = 0;
					continue;
				}
				hamming_distance_matrix[i][j] = hamming_data_distance(data[i]->hData,data[j]->hData);
			}

		hamming_distance_matrix_size = n;
	}
	else if(data[0]->eData){
		euclidean_distance_matrix = malloc(n*sizeof(double*));
		if(euclidean_distance_matrix == NULL){
			perror("Failed to allocate memory for new distance matrix");
			return;
		}

		for(i=0;i<n;i++){
			euclidean_distance_matrix[i] = malloc(n*sizeof(double));
			if(euclidean_distance_matrix[i] == NULL){
				perror("Failed to allocate memory for new distance matrix");
				return;
			}
		}

		for(i=0;i<n;i++)
			for(j=0;j<n;j++){
				if(i==j){
					euclidean_distance_matrix[i][j] = 0.0;
					continue;
				}
				euclidean_distance_matrix[i][j] = euclidean_data_distance(data[i]->eData,data[j]->eData);
			}

		euclidean_distance_matrix_size = n;
	}
	else if(data[0]->cData){
		cosine_distance_matrix = malloc(n*sizeof(double*));
		if(cosine_distance_matrix == NULL){
			perror("Failed to allocate memory for new distance matrix");
			return;
		}

		for(i=0;i<n;i++){
			cosine_distance_matrix[i] = malloc(n*sizeof(double));
			if(cosine_distance_matrix[i] == NULL){
				perror("Failed to allocate memory for new distance matrix");
				return;
			}
		}

		for(i=0;i<n;i++)
			for(j=0;j<n;j++){
				if(i == j){
					cosine_distance_matrix[i][j] = 0.0;
					continue;
				}
				cosine_distance_matrix[i][j] = cosine_data_distance(data[i]->cData,data[j]->cData);
			}

		cosine_distance_matrix_size = n;
	}
	else{
		fprintf(stderr,"Invalid data type given on data_create_distance_matrix\n");
		return;
	}
}

void data_destroy_distance_matrix(){
	unsigned int i;
	if(hamming_distance_matrix){
		for(i=0;i<hamming_distance_matrix_size;i++) free(hamming_distance_matrix[i]);
		free(hamming_distance_matrix);
		hamming_distance_matrix = NULL;
	}
	else if(euclidean_distance_matrix){
		for(i=0;i<euclidean_distance_matrix_size;i++) free(euclidean_distance_matrix[i]);
		free(euclidean_distance_matrix);
		euclidean_distance_matrix = NULL;
	}
	else if(cosine_distance_matrix){
		for(i=0;i<cosine_distance_matrix_size;i++) free(cosine_distance_matrix[i]);
		free(cosine_distance_matrix);
		cosine_distance_matrix = NULL;
	}
	/*else if(matrix_distance_matrix){

	}*/
	else
		fprintf(stderr,"No distance matrices were created\n");
}

unsigned int hamming_data_distance(HammingData* a,HammingData* b){
	unsigned int distance = 0;
	uint64_t x = a->bits;
	uint64_t y = b->bits;
   	while(x | y){
   		if((x & 0x1)!=(y & 0x1)) distance+=1;
   		x >>= 1;
   		y >>= 1;
   	}

   	return distance;
}

double euclidean_data_distance(EuclideanData* a,EuclideanData* b){
	int i;
	double sum=0.0;
	double* x = a->vector;
	double* y = b->vector;

	for(i=0;i<=euclidean_dim-1;i++)
	{
		sum+=(x[i]-y[i])*(x[i]-y[i]);
	}
	return sum;
}

double cosine_data_distance(CosineData* a,CosineData* b){
	int i;
	double xy=0.0,xx=0.0,yy=0.0;
	double* x = a->vector;
	double* y = b->vector;
	
	for(i=0;i<=cosine_dim-1;i++)
	{
		xy+=x[i]*y[i];
	}
	for(i=0;i<=cosine_dim-1;i++)
	{
		xx+=x[i]*x[i];
	}
	xx=sqrt(xx);
	for(i=0;i<=cosine_dim-1;i++)
	{
	    yy+=y[i]*y[i];
	}
	yy=sqrt(yy);
	return 1-(xy/(xx*yy));
}

int is_hamming_data(Data d){
	return d->hData != NULL;
}

int is_euclidean_data(Data d){
	return d->eData != NULL;
}

int is_cosine_data(Data d){
	return d->cData != NULL;
}

int is_matrix_data(Data d){
	return d->mData != NULL;
}

uint64_t hamming_data_get_bits(Data d){
	return d->hData->bits;
}

int euclidean_data_is_set(Data d){
	return d->eData->id_flag != 0;
}

unsigned int euclidean_data_get_sigID(Data d){
	return d->eData->ID;
}

void euclidean_data_set_sigID(Data d,unsigned int sigID){
	d->eData->ID = sigID;
}

double* euclidean_data_getVector(Data d){
	return d->eData->vector;
}

double* cosine_data_getVector(Data d){
	return d->cData->vector;
}

unsigned int matrix_data_get_id(Data d){
	return d->mData->id;
}

void data_set_distance_matrix(unsigned int** distanceMatrix){
	matrix_distance_matrix = distanceMatrix;
}

unsigned int data_getIdDistance(uint64_t a,uint64_t b){
	return matrix_distance_matrix[a][b];
}