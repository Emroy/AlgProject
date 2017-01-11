#include "HashGen.h"
#include "Ratings.h"

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

unsigned int hash_apply(HashDescriptor hd,User x){
	unsigned int retVal = 0;
	int i;
	int8_t* data = user_getUserRatings(x);

	/*Hamming case*/
	if(hd->hamming != NULL){

		for(i=0;i<hd->hamming->size;i++){
			retVal += data[hd->hamming->g[i]];
			retVal << 1;
		}

		return retVal >> 1;
	}
	/*Euclidean case*/
	else if(hd->euclidean != NULL){

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

		int h,j;
		for(i=0;i<hd->cosine->k;i++){
			h=0;
			for(j=0;j<hd->cosine->d;j++) h+=(double)(data[j])*hd->cosine->r[i][j];
			if(h>=0) retVal+=1;
			retVal << 1;
		}
		return retVal >> 1;
	}
}