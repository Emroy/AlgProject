#include "data.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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

Data euclidean_data_create(){

}

Data cosine_data_create(){

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

EuclideanData euclidean_data_create(HashDescriptor hd,double* p){
	if(hd->euclidean == NULL) fprintf(stderr, "invalid HashDescriptor given on euclidean_data_create\n");

	EuclideanData retVal = malloc(sizeof(struct EuData));
	if(retVal == NULL){
		perror("Could not allocate memory for new EuclideanData");
		return NULL;
	}

	retVal->data = p;

	long int temp = 0;
	int i,j,h;
	for(i=0;i<hd->euclidean->k;i++){
		h = 0;
		for(j=0;j<hd->euclidean->d;j++) h+=p[j]*hd->euclidean->v[i][j];
		temp += hd->euclidean->r[i]*h;
	}

	retVal->id = temp % M;

	return retVal;
}

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