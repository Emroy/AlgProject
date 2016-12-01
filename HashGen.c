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

/*-----------------------------*/

typedef struct CosineDesc{
	float** r;
	int k;
	int d;
} CosineDescriptor;

/*-----------------------------*/

typedef struct MatrixDesc{
	unsigned int** distanceMatrix;
	int n;
	uint64_t* x1;
	uint64_t* x2;
	double* t1; /*mesos apo h gia ola ta shmeia*/
	int k;
} MatrixDescriptor;

/*-----------------------------*/

struct HashDesc{
	HammingDescriptor* hamming;
	EuclideanDescriptor* euclidean;
	CosineDescriptor* cosine;
	MatrixDescriptor* matrix;
};

/*-------------------GENERAL--------------------*/

unsigned int hash_apply(HashDescriptor hd,Data x){
	unsigned int retVal = 0;
	int i;

	/*Hamming case*/
	if(hd->hamming != NULL){
		if(!is_hamming_data(x)){
			fprintf(stderr,"Incompatible hash function and data metrics\n");
			exit(-10);
		}

		uint64_t data = hamming_data_get_bits(x);
		uint64_t temp=1;
		for(i=0;i<hd->hamming->size;i++){
			temp = temp << hd->hamming->g[i];
			if((temp & data) != 0) retVal++;
			retVal << 1;
			temp = 1;
		}

		return retVal >> 1;
	}
	/*Euclidean case*/
	else if(hd->euclidean != NULL){
		if(!is_euclidean_data(x)){
			fprintf(stderr,"Incompatible hash function and data metrics\n");
			exit(-10);
		}

		unsigned int sigId = 0;
		if(euclidean_data_is_set(x)){
			sigId = euclidean_data_get_sigID(x);
		}
		else{
			double* p = euclidean_data_getVector(x);
			unsigned int j;
			for(i=0;i<hd->euclidean->k;i++){
				unsigned int h = hd->euclidean->t[i];
				for(j=0;j<hd->euclidean->d;j++)
					h += p[j]*hd->euclidean->v[i][j];
				h /= W;
				sigId += hd->euclidean->r[i]*h;
			}
			sigId = sigId % M;
			euclidean_data_set_sigID(x,sigId);
		}

		return sigId % hd->euclidean->n/HASH_SIZE_DIV;
	}
	/*Cosine case*/
	else if(hd->cosine != NULL){
		if(!is_cosine_data(x)){
			fprintf(stderr,"Incompatible hash function and data metrics\n");
			exit(-10);
		}

		double* data = cosine_data_getVector(x);
		int h,j;
		for(i=0;i<hd->cosine->k;i++){
			h=0;
			for(j=0;j<hd->cosine->d;j++) h+=data[j]*hd->cosine->r[i][j];
			if(h>=0) retVal+=1;
			retVal << 1;
		}
		return retVal >> 1;
	}
	/*Matrix case*/
	else if(hd->matrix != NULL){
		if(!is_matrix_data(x)){
			fprintf(stderr,"Incompatible hash function and data metrics\n");
			exit(-10);
		}

		unsigned int data = matrix_data_get_id(x)-1;
		int i;
		for(i=0;i<hd->matrix->k;i++){
			unsigned int* dist = data_getIdDistance(data,hd->matrix->x1[i]);
			unsigned long int temp = *dist;
			temp = temp*temp;
			dist = data_getIdDistance(data,hd->matrix->x2[i]);
			temp += (*dist)*(*dist);
			dist = data_getIdDistance(hd->matrix->x1[i],hd->matrix->x2[i]);
			temp -= (*dist)*(*dist);
			double h = (double)temp/(2*(*dist));
			if(h >= hd->matrix->t1[i]) retVal+=1;
			retVal << 1;
		}
		return retVal >> 1;
	}
	else{
		fprintf(stderr,"Invalid HashDescriptor given on hash_apply\n");
		return -1;
	}
}

/*------------------HAMMING----------------------------*/

HashDescriptor hamming_hash_create(int d,int k){
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

	retVal->euclidean->r = malloc(k*sizeof(int));
	if(retVal->euclidean->r == NULL){
		perror("Failed to allocate memory for random values for new Euclidean HashDescriptor");
		free(retVal->euclidean->t);
		free(retVal->euclidean);
		free(retVal);
		return NULL;
	}

	retVal->euclidean->v = malloc(sizeof(float*)*k);
	if(retVal->euclidean->v == NULL){
		perror("Failed to allocate memory for random vectors for new Euclidean HashDescriptor");
		free(retVal->euclidean->t);
		free(retVal->euclidean->r);
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

int euclidean_is_equal(HashDescriptor a,HashDescriptor b){
	int i,j;
	for(i=0;i<a->euclidean->k;i++){
		if(a->euclidean->t[i] != b->euclidean->t[i]) return 0;
		if(a->euclidean->r[i] != b->euclidean->r[i]) return 0;
		for(j=0;j<a->euclidean->d;j++)
			if(a->euclidean->v[i][j] != b->euclidean->v[i][j]) return 0;
	}
	return 1;
}

void euclidean_hash_destroy(HashDescriptor hd){
	int i;
	for(i=0;i<hd->euclidean->k;i++) free(hd->euclidean->v[i]);
	free(hd->euclidean->v);
	free(hd->euclidean->t);
	free(hd->euclidean->r);
	free(hd->euclidean);
	free(hd);
	hd=NULL;
}

/*----------------------COSINE-----------------------------*/
HashDescriptor cosine_hash_create(int d,int k){
	HashDescriptor retVal = malloc(sizeof(struct HashDesc));
	if(retVal == NULL){
		perror("Failed to allocate memory for new cosine HashDescriptor");
		return NULL;
	}

	retVal->hamming = NULL;
	retVal->euclidean = NULL;
	retVal->matrix = NULL;
	retVal->cosine = malloc(sizeof(CosineDescriptor));
	if(retVal == NULL){
		perror("Failed to allocate memory for new cosine HashDescriptor");
		free(retVal);
		return NULL;
	}

	retVal->cosine->k = k;
	retVal->cosine->d = d;
	retVal->cosine->r = malloc(sizeof(float*)*k);
	if(retVal->cosine->r == NULL){
		perror("Failed to allocate memory for random vector r while creating new cosine HashDescriptor");
		free(retVal->cosine);
		free(retVal);
		return NULL;
	}

	int i,j;
	for(i=0;i<k;i++){
		retVal->cosine->r[i] = malloc(sizeof(float)*d);
		if(retVal->cosine->r[i] == NULL){
			perror("Failed to allocate memory for random vector r while creating new cosine HashDescriptor");
			for(j=i-1;j>=0;j--) free(retVal->cosine->r[j]);
			free(retVal->cosine->r);
			free(retVal->cosine);
			free(retVal);
			return NULL;
		}

		for(j=0;j<d;j++) retVal->cosine->r[i][j] = Gaussian01();
	}

	return retVal;
}

int cosine_is_equal(HashDescriptor a,HashDescriptor b){
	int i,j;
	for(i=0;i<a->cosine->k;i++)
		for(j=0;j<a->cosine->d;j++)
			if(a->cosine->r[i][j] != b->cosine->r[i][j]) return 0;
	return 1;
}

void cosine_hash_destroy(HashDescriptor hd){
	int i;
	for(i=0;i<0;i++) free(hd->cosine->r[i]);
	free(hd->cosine->r);
	free(hd->cosine);
	free(hd);
	hd=NULL;
}

/*--------------------------MATRIX-----------------------*/
int comparator(const void* p1,const void* p2){
	if(*(unsigned int*)p1 < *(unsigned int*)p2) return -1;
	if(*(unsigned int*)p1 == *(unsigned int*)p2) return 0;
	if(*(unsigned int*)p1 > *(unsigned int*)p2) return 1;
}

HashDescriptor matrix_hash_create(int k,int n){
	HashDescriptor retVal = malloc(sizeof(struct HashDesc));
	if(retVal == NULL){
		perror("Failed to allocate memory for new matrix HashDescriptor");
		return NULL;
	}

	retVal->hamming = NULL;
	retVal->euclidean = NULL;
	retVal->cosine = NULL;

	retVal->matrix = malloc(sizeof(MatrixDescriptor));
	if(retVal->matrix == NULL){
		perror("Failed to allocate memory for new matrix HashDescriptor");
		free(retVal);
		return NULL;
	}

	retVal->matrix->n = n;
	retVal->matrix->k = k;

	retVal->matrix->x1 = malloc(sizeof(uint64_t)*k);
	if(retVal->matrix->x1 == NULL){
		perror("Failed to allocate memory for random values x1 for matrix HashDescriptor");
		free(retVal->matrix);
		free(retVal);
		return NULL;
	}

	retVal->matrix->x2 = malloc(sizeof(uint64_t)*k);
	if(retVal->matrix->x2 == NULL){
		perror("Failed to allocate memory for random values x2 for matrix HashDescriptor");
		free(retVal->matrix->x1);
		free(retVal->matrix);
		free(retVal);
		return NULL;
	}

	retVal->matrix->t1 = malloc(sizeof(double)*k);
	if(retVal->matrix->t1 == NULL){
		perror("Failed to allocate memory for median values t1 for matrix HashDescriptor");
		free(retVal->matrix->x1);
		free(retVal->matrix->x2);
		free(retVal->matrix);
		free(retVal);
		return NULL;
	}

	int i;
	for(i=0;i<k;i++){
		retVal->matrix->x1[i] = (uint64_t)integerUniform(n);
		do{
			retVal->matrix->x2[i] = (uint64_t)integerUniform(n);
		}while(retVal->matrix->x1[i] == retVal->matrix->x2[i]);
	}

	int j;
	double temp,x1x2_dist;
	double* hsum = malloc(n*sizeof(double));
	if(hsum == NULL){
		perror("Failed to allocate memory for temporary array in matrix hash create");
		return NULL;
	}
	unsigned int* dist;
	for(i=0;i<k;i++){
		dist = data_getIdDistance(retVal->matrix->x1[i],retVal->matrix->x2[i]);
		x1x2_dist = (double)(*dist);
		for(j=0;j<n;j++){
			dist = data_getIdDistance(j,retVal->matrix->x1[i]);
			temp = (double)(*dist)*(double)(*dist);
			dist = data_getIdDistance(j,retVal->matrix->x2[i]);
			temp += (double)(*dist)*(double)(*dist);
			temp -= x1x2_dist*x1x2_dist;
			temp = temp / 2*x1x2_dist;
			hsum[j] = temp;
		}
		qsort(hsum,k,sizeof(double),comparator);

		if(n%2){
			retVal->matrix->t1[i] = (hsum[n/2-1]+hsum[n/2])/2;
		}
		else{
			retVal->matrix->t1[i] = hsum[n/2];
		}
	}

	free(hsum);
	return retVal;
}

int matrix_is_equal(HashDescriptor a, HashDescriptor b){
	int i;
	for(i=0;i<a->matrix->k;i++)
		if(a->matrix->t1[i] != b->matrix->t1[i]) return 0;
	return 1;
}

void matrix_hash_destroy(HashDescriptor hd){
	free(hd->matrix->x1);
	free(hd->matrix->x2);
	free(hd->matrix->t1);
	free(hd->matrix);
	free(hd);
	hd=NULL;
}
