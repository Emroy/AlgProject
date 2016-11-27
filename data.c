#include "data.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "List.h"

/*------------HAMMING DATA-------------*/
typedef struct HammingData{
	uint64_t id;
	uint64_t bits;
}HammingData;

static unsigned int** hamming_distance_matrix = NULL;

/*------------EUCLIDEAN DATA-----------*/
typedef struct EucliudeanData{
	uint64_t id; 		/*the number of this element*/
	unsigned int ID; 	/*calculated by hashFunction*/
	uint8_t id_flag;	/*0 if ID is not set, 1 otherwise*/
	double* vector;
}EuclideanData;

static unsigned short euclidean_dim = 0;
static double** euclidean_distance_matrix = NULL;

/*-----------COSINE DATA---------------*/
typedef struct CosineData{
	uint64_t id;
	double* vector;
}CosineData;

static unsigned short cosine_dim = 0;
static double** cosine_distance_matrix = NULL;

/*--------------MATRIX DATA------------*/
static unsigned int** matrix_distance_matrix = NULL;

/*--------------GENERIC DATA-----------*/
typedef struct GenData{
	HammingData* hData;
	EuclideanData eData;
	CosineData cData;
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

	char* itemID = strtok(itemString," \n\t");
	char* bitString = strtok(NULL," \n\t");

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
		if(bitCount >= 64){
			fprintf(stderr,"hamming_data_create: Can't have hamming data longer than 64 bits\n");
			free(retVal->hData);
			free(retVal);
			return NULL;
		}

		i++;
	}

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

	char* itemID = strtok(itemString," \t\n");
	char* vectorString = strtok(NULL," \t\n");

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
		while(str = strtok(NULL," \t\n")){
			double* element = malloc(sizeof(double));
			if(element == NULL){
				perror("Failed to allocate memory for Euclidean Data vector");
				free(retVal->eData);
				free(retVal);
				while(!list_empty(l)) free(list_pop(l));
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
			while(!list_empty(l)) free(list_pop(l));
			list_destroy(l);
			return NULL;
		}

		i=0;
		while(!list_empty(l)){
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
		vec_element = strtok(NULL," \t\n");
		if(vec_element == NULL){
			fprintf(stderr,"Inconsistent dimention given on Euclidean Data vectors\n");
			free(retVal->eData->vector);
			free(retVal->eData);
			free(retVal);
			return NULL;
		}
		retVal->eData->vector = strtod(vec_element,NULL);
	}

	return retVal;
}

Data cosine_data_create(char* itemString){

}

void* data_distance(){

}

unsigned int hamming_data_distance(HammingData a,HammingData b){
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

void hamming_data_destroy(HammingData data){
	free(data);
}

/*------------EUCLIDEAN DATA-----------*/
struct EucliudeanData{
	int id;
	double* data; /*data vector*/
	int dim; /*dimention of data vector*/
};

double euclidean_data_distance(EuclideanData a,EuclideanData b){
	int i;
	double sum=0.0;
	int d = a->dim;
	double* x = a->data;
	double* y = b->data;

	for(i=0;i<=d-1;i++)
	{
		sum+=(x[i]-y[i])*(x[i]-y[i]);
	}
	return sum;
}

void euclidean_data_destroy(EuclideanData data){
	free(data);
}

/*------------COSINE DATA--------------*/
struct CosineData{
	double *data;
	int dim;
};

CosineData cosine_data_create(double* p){
	CosineData retVal = malloc(sizeof(struct CosineData));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Cosine Data");
		return NULL;
	}

	retVal = p;

	return retVal;
}

double cosine(CosineData a,CosineData b){
	int i;
	double xy=0.0,xx=0.0,yy=0.0;
	int d = a->dim;
	double* x = a->data;
	double* y = b->data;
	
	for(i=0;i<=d-1;i++)
	{
		xy+=x[i]*y[i];
	}
	for(i=0;i<=d-1;i++)
	{
		xx+=x[i]*x[i];
	}
	xx=sqrt(xx);
	for(i=0;i<=d-1;i++)
	{
	    yy+=y[i]*y[i];
	}
	yy=sqrt(yy);
	return (xy/(xx*yy));
}

void cosine_data_destroy(CosineData data){
	free(data);
}

/*------------MATRIX DATA--------------*/
static unsigned int** distance_matrix;

struct MatrixData{

};

void matrix_data_init_matrix(unsigned int** distance_matr){
	distance_matrix = distance_matr;
}