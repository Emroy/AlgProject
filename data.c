#include "data.h"
#include <stdint.h>
#include <stdio.h>

/*------------HAMMING DATA-------------*/
struct HammingData{
	uint64_t bits;
};

HammingData hamming_data_create(char* bitString){
	HammingData retVal = malloc(sizeof(struct HammingData));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Hamming Data");
		return NULL;
	}

	retVal->bits = 0;
	unsigned short int i = 0;
	unsigned short int bitCount = 0;
	while(bitString[i]){
		if(bitString[i] == '1'){
			retVal->bits = retVal->bits << 1;
			retVal->bits++;
			bitCount++;
		}
		else if(bitString[i] == '0'){
			retVal->bits = retVal->bits << 1;
			bitCount++;
		}
		if(bitCount >= 64){
			fprintf(stderr,"hamming_data_create: Can't have hamming data longer than 64 bits\n");
			free(retVal);
			return NULL;
		}

		i++;
	}

	return retVal;
}

void hamming_data_destroy(HammingData data){
	free(data);
}

/*------------EUCLIDEAN DATA-----------*/
struct EucliudeanData{
	int id;
	double* data;
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

void euclidean_data_destroy(EuclideanData data){
	free(data);
}

/*------------COSINE DATA--------------*/
struct CosineData{
	double *data;
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