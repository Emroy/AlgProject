#include "HashGen.h"
#include <stdio.h>
#include <stdlib.h>
#include "RNG.h"
#define HASH_SIZE_DIV 2
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

struct HashDesc{
	HammingDescriptor* hd;
	EuclideanDescriptor* ed;
	CosineDescriptor* cd;
};

/*------------------------------FUNCTIONS---------------------------*/

unsigned int hash_apply(HashDescriptor hd,Data x){
	unsigned int retVal = 0;
	int i;

	/*Hamming case*/
	if(hd->hd != NULL){
		int8_t* data = data_getVector(x);
		unsigned int j;
		for(i=0;i<hd->hd->size;i++){
			j = 0;
			while(!data_vectorEnd(data[j])){
				if(j == hd->hd->g[i]){
					retVal++;
					retVal << 1;
				}
				j++;
			}
		}

		return retVal >> 1;
	}
	/*Euclidean case*/
	else if(hd->ed != NULL){
		unsigned int sigId = 0;
		if(euclidean_data_is_set(x)){
			sigId = euclidean_data_get_sigID(x);
		}
		else{
			int8_t* p = data_getVector(x);
			unsigned int j;
			for(i=0;i<hd->ed->k;i++){
				unsigned int h = hd->ed->t[i];
				for(j=0;j<hd->ed->d;j++)
					h += (double)(p[j])*hd->ed->v[i][j];
				h /= W;
				sigId += hd->ed->r[i]*h;
			}
			sigId = sigId % M;
			euclidean_data_set_sigID(x,sigId);
		}

		return sigId % hd->ed->n/HASH_SIZE_DIV;
	}
	/*Cosine case*/
	else if(hd->cd != NULL){
		int8_t* data = data_getVector(x);
		int h,j;
		for(i=0;i<hd->cd->k;i++){
			h=0;
			for(j=0;j<hd->cd->d;j++) h+=(double)(data[j])*hd->cd->r[i][j];
			if(h>=0) retVal+=1;
			retVal << 1;
		}
		return retVal >> 1;
	}
	else{
		fprintf(stderr,"Invalid HashDescriptor given on hash_apply\n");
		return -1;
	}
}

HashDescriptor hamming_hash_create(int d,int k){
	HashDescriptor retVal = malloc(sizeof(struct HashDesc));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Hamming HashDescriptor");
		return NULL;
	}

	retVal->hd = malloc(sizeof(HammingDescriptor));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Hamming HashDescriptor");
		return NULL;
	}

	retVal->hd->g = malloc(sizeof(uint8_t)*k);
	if(retVal->hd->g == NULL){
		perror("Failed to allocate memory for new Hamming HashDescriptor");
		return NULL;
	}
	retVal->hd->size=k;

	retVal->ed = NULL;
	retVal->cd = NULL;

	int i;
	for(i=0;i<k;i++) retVal->hd->g[i] = (uint8_t)integerUniform(d);

	return retVal;
}

HashDescriptor euclidean_hash_create(int d,int k,int n){
	HashDescriptor retVal = malloc(sizeof(struct HashDesc));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Euclidean HashDescriptor");
		return NULL;
	}

	retVal->ed = malloc(sizeof(EuclideanDescriptor));
	if(retVal->ed == NULL){
		perror("Failed to allocate memory for new Euclidean HashDescriptor");
		free(retVal);
		return NULL;
	}

	retVal->ed->k = k;
	retVal->ed->t = malloc(sizeof(float)*k);
	if(retVal->ed->t == NULL){
		perror("Failed to allocate memory for random values for new Euclidean HashDescriptor");
		free(retVal->ed);
		free(retVal);
		return NULL;
	}

	retVal->ed->r = malloc(k*sizeof(int));
	if(retVal->ed->r == NULL){
		perror("Failed to allocate memory for random values for new Euclidean HashDescriptor");
		free(retVal->ed->t);
		free(retVal->ed);
		free(retVal);
		return NULL;
	}

	retVal->ed->v = malloc(sizeof(float*)*k);
	if(retVal->ed->v == NULL){
		perror("Failed to allocate memory for random vectors for new Euclidean HashDescriptor");
		free(retVal->ed->t);
		free(retVal->ed->r);
		free(retVal->ed);
		free(retVal);
		return NULL;
	}

	retVal->ed->d = d;
	int i,j;
	for(i=0;i<k;i++){
		retVal->ed->v[i] = malloc(sizeof(float)*d);
		if(retVal->ed->v[i] == NULL){
			perror("Failed to allocate memory for random vectors for new Euclidean HashDescriptor");
			free(retVal->ed->t);
			free(retVal->ed->v);
			free(retVal->ed);
			free(retVal);
			return NULL;
		}
		for(j=0;j<d;j++) retVal->ed->v[i][j] = realUniform(-1,1,0);
	}

	retVal->hd = NULL;
	retVal->cd = NULL;

	for(i=0;i<k;i++){
		retVal->ed->t[i] = realUniform(0,W,2);
		retVal->ed->r[i] = integerUniform(-1);
	}
	retVal->ed->n = n;

	return retVal;
}

HashDescriptor cosine_hash_create(int d,int k){
	HashDescriptor retVal = malloc(sizeof(struct HashDesc));
	if(retVal == NULL){
		perror("Failed to allocate memory for new cosine HashDescriptor");
		return NULL;
	}

	retVal->hd = NULL;
	retVal->ed = NULL;
	retVal->cd = malloc(sizeof(CosineDescriptor));
	if(retVal == NULL){
		perror("Failed to allocate memory for new cosine HashDescriptor");
		free(retVal);
		return NULL;
	}

	retVal->cd->k = k;
	retVal->cd->d = d;
	retVal->cd->r = malloc(sizeof(float*)*k);
	if(retVal->cd->r == NULL){
		perror("Failed to allocate memory for random vector r while creating new cosine HashDescriptor");
		free(retVal->cd);
		free(retVal);
		return NULL;
	}

	int i,j;
	for(i=0;i<k;i++){
		retVal->cd->r[i] = malloc(sizeof(float)*d);
		if(retVal->cd->r[i] == NULL){
			perror("Failed to allocate memory for random vector r while creating new cosine HashDescriptor");
			for(j=i-1;j>=0;j--) free(retVal->cd->r[j]);
			free(retVal->cd->r);
			free(retVal->cd);
			free(retVal);
			return NULL;
		}

		for(j=0;j<d;j++) retVal->cd->r[i][j] = Gaussian01();
	}

	return retVal;
}

int same_type(HashDescriptor a,HashDescriptor b){
	if(a->hd != NULL){
		return b->hd != NULL;
	}
	if(a->ed != NULL){
		return b->ed != NULL;
	}
	if(a->cd != NULL){
		return b->cd != NULL;
	}
	return 0; /*should never happen*/
}

int hash_isEqual(HashDescriptor a,HashDescriptor b){
	if(!same_type(a,b))
		return 0;

	/*Hamming case*/
	if(a->hd != NULL){
		if(a->hd->size != b->hd->size) return 0;

		int i;
		for(i=0;i<a->hd->size;i++)
			if(a->hd->g[i] != b->hd->g[i]) return 0;

		return 1;
	}

	/*Euclidean case*/
	if(a->ed != NULL){
		int i,j;
		for(i=0;i<a->ed->k;i++){
			if(a->ed->t[i] != b->ed->t[i]) return 0;
			if(a->ed->r[i] != b->ed->r[i]) return 0;
			for(j=0;j<a->ed->d;j++)
				if(a->ed->v[i][j] != b->ed->v[i][j]) return 0;
		}
		return 1;
	}

	/*Cosine case*/
	if(a->cd != NULL){
		int i,j;
		for(i=0;i<a->cd->k;i++)
			for(j=0;j<a->cd->d;j++)
				if(a->cd->r[i][j] != b->cd->r[i][j]) return 0;
		return 1;
	}

	return 0; /*should never happen*/
}

void hash_destroy(HashDescriptor hd){
	/*Hamming case*/
	if(hd->hd != NULL){
		free(hd->hd->g);
		free(hd->hd);
		free(hd);
		hd = NULL;
	}

	/*Euclidean case*/
	if(hd->ed != NULL){
		int i;
		for(i=0;i<hd->ed->k;i++) free(hd->ed->v[i]);
		free(hd->ed->v);
		free(hd->ed->t);
		free(hd->ed->r);
		free(hd->ed);
		free(hd);
		hd=NULL;
		return;
	}

	/*Cosine case*/
	if(hd->cd != NULL){
		int i;
		for(i=0;i<0;i++) free(hd->cd->r[i]);
		free(hd->cd->r);
		free(hd->cd);
		free(hd);
		hd=NULL;
	}
}