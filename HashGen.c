#include <stdio.h>
#include <stdlib.h>
#include "HashGen.h"
#include "RNG.h"
#define W 4
#define M 4294967291

typedef struct HammingDesc{
	uint8_t* g;
	unsigned short size;	
} HammingDescriptor;

/*-----------------------------*/

typedef struct EuclideanDesc{
	float* t;
	float** v;
	int* r;
	int k;
	int d;
	int n;
} EuclideanDescriptor;

struct EuData{
	int id;
	double* data;
};

/*-----------------------------*/

typedef struct CosineDesc{
} CosineDescriptor;

/*-----------------------------*/

typedef struct MatrixDesc{
} MatrixDescriptor;

/*-----------------------------*/

struct HashDesc{
	HammingDescriptor* hamming;
	EuclideanDescriptor* euclidean;
	CosineDescriptor* cosine;
	MatrixDescriptor* matrix;
};

/*-----------------------------*/

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

/*-------------------GENERAL--------------------*/

int hash_apply(HashDescriptor hd,void* x){
	unsigned int retVal = 0;
	int i;

	if(hd->hamming != NULL){
	/*Hamming case*/
		long long int* data = x;
		long long int temp=1;
		for(i=0;i<hd->hamming->size;i++){
			temp = temp << hd->hamming->g[i];
			retVal += (temp & *data) != 0;
			retVal << 1;
			temp = 1;
		}

		return retVal >> 1;
	}
	else if(hd->euclidean != NULL){
	/*Euclidean case*/
		EuclideanData data = x;
		return data->id % hd->n/HASH_SIZE_DIV;
	}
	else if(hd->cosine != NULL){
	/*Cosine case*/
	}
	else if(hd->matrix != NULL){
	/*Matrix case*/
	}
	else{
		fprintf(stderr,"Invalid HashDescriptor given on hash_apply\n");
		return -1;
	}
}

/*------------------HAMMING----------------------------*/

HashDescriptor hamming_hash_create(int k,int d){
	HashDescriptor retVal = malloc(sizeof(struct HashDesc));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Hamming HashDescriptor");
		return NULL;
	}

	retVal->hamming = malloc(sizeof(HammingDescriptor));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Hamming HashDescriptor");
		return NULL;
	}

	retVal->hamming->g = malloc(sizeof(uint8_t)*k);
	if(retVal->hamming->g == NULL){
		perror("Failed to allocate memory for new Hamming HashDescriptor");
		return NULL;
	}
	retVal->hamming->size=k;

	retVal->euclidean = NULL;
	retVal->cosine = NULL;
	retVal->matrix = NULL;

	int i;
	for(i=0;i<k;i++) retVal->hamming->g[i] = (uint8_t)integerUniform(d);

	return retVal;
}

int hamming_is_equal(HashDescriptor a,HashDescriptor b){
	/*check if a and b are Hamming HashDescriptors*/
	if(a->hamming->size != b->hamming->size) return 0;

	int i;
	for(i=0;i<a->hamming->size;i++)
		if(a->hamming->g[i] != b->hamming->g[i]) return 0;

	return 1;
}

void hamming_destroy(HashDescriptor hd){
	free(hd->hamming->g);
	free(hd->hamming);
	free(hd);
	hd = NULL;
}

/*--------------------------EUCLIDEAN-----------------------------*/

HashDescriptor euclidean_hash_create(int d,int k,int n){
	HashDescriptor retVal = malloc(sizeof(struct HashDesc));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Euclidean HashDescriptor");
		return NULL;
	}

	retVal->euclidean = malloc(sizeof(EuclideanDescriptor));
	if(retVal->euclidean == NULL){
		perror("Failed to allocate memory for new Euclidean HashDescriptor");
		free(retVal);
		return NULL;
	}

	retVal->euclidean->k = k;
	retVal->euclidean->t = malloc(sizeof(float)*k);
	if(retVal->euclidean->t == NULL){
		perror("Failed to allocate memory for random values for new Euclidean HashDescriptor");
		free(retVal->euclidean);
		free(retVal);
		return NULL;
	}

	retVal->euclidean->v = malloc(sizeof(float*)*k);
	if(retVal->euclidean->v == NULL){
		perror("Failed to allocate memory for random vectors for new Euclidean HashDescriptor");
		free(retVal->euclidean->t);
		free(retVal->euclidean);
		free(retVal);
		return NULL;
	}

	retVal->euclidean->d = d;
	int i,j;
	for(i=0;i<k;i++){
		retVal->euclidean->v[i] = malloc(sizeof(float)*d);
		if(retVal->euclidean->v[i] == NULL){
			perror("Failed to allocate memory for random vectors for new Euclidean HashDescriptor");
			free(retVal->euclidean->t);
			free(retVal->euclidean->v);
			free(retVal->euclidean);
			free(retVal);
			return NULL;
		}
		for(j=0;j<d;j++) retVal->euclidean->v[i][j] = realUniform(-1,1,0);
	}

	retVal->hamming = NULL;
	retVal->cosine = NULL;
	retVal->matrix = NULL;

	for(i=0;i<k;i++){
		retVal->euclidean->t[i] = realUniform(0,W,2);
		retVal->euclidean->r[i] = integerUniform(-1);
	}
	retVal->euclidean->n = n;

	return retVal;
}

void euclidean_hash_destroy(HashDescriptor hd){
	int i;
	for(i=0;i<hd->euclidean->k;i++) free(hd->euclidean->v[i]);
	free(hd->euclidean->v);
	free(hd->euclidean->t);
	free(hd->euclidean);
	free(hd);
	hd=NULL;
}
