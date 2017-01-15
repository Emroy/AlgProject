#include "data.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct HammingData* HammingData;
typedef struct EuclideanData* EuclideanData;
typedef struct CosineData* CosineData;

struct DataStruct{
	HammingData hd;
	EuclideanData ed;
	CosineData cd;
};

struct HammingData{
	User user;
};

Data hammingData_create(User user){
	Data d = malloc(sizeof(struct DataStruct));
	if(d == NULL){
		perror("Failed to allocate memory for new Hamming Data");
		return NULL;
	}

	HammingData retVal = malloc(sizeof(struct HammingData));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Hamming Data");
		return NULL;
	}

	retVal->user = user;
	d->hd = retVal;
	d->ed = NULL;
	d->cd = NULL;

	return d;
}

struct EuclideanData{
	User user;
	unsigned int sig;
	int8_t is_set_sig; //true if sig is set, false otherwise
};

Data euclideanData_create(User user){
	Data d = malloc(sizeof(struct DataStruct));
	if(d == NULL){
		perror("Failed to allocate memory for new Euclidean Data");
		return NULL;
	}

	EuclideanData retVal = malloc(sizeof(struct EuclideanData));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Euclidean Data");
		return NULL;
	}

	retVal->user = user;
	retVal->sig = 0;
	retVal->is_set_sig = 0;
	d->hd = NULL;
	d->ed = retVal;
	d->cd = NULL;

	return d;
}

struct CosineData{
	User user;
};

Data cosineData_create(User user){
	Data d = malloc(sizeof(struct DataStruct));
	if(d == NULL){
		perror("Failed to allocate memory for new Cosine Data");
		return NULL;
	}

	CosineData retVal = malloc(sizeof(struct CosineData));
	if(retVal == NULL){
		perror("Failed to allocate memory for new Cosine Data");
		return NULL;
	}

	retVal->user = user;
	d->hd = NULL;
	d->ed = NULL;
	d->cd = retVal;

	return d;
}

void data_destroy(Data data){
	if(data->hd) free(data->hd);
	else if(data->ed) free(data->ed);
	else if(data->cd) free(data->cd);
	free(data);
}

int8_t* data_getVector(Data d){
	if(d->hd){
		return user_getRatingsVector(d->hd->user);
	}
	else if(d->ed){
		return user_getRatingsVector(d->ed->user);
	}
	else if(d->cd){
		return user_getRatingsVector(d->cd->user);
	}

	return NULL; /*should never happen*/
}

int data_vectorEnd(int8_t current){
	return current == RATINGS_END;
}

int euclidean_data_is_set(Data d){
	if(d->ed == NULL) return 0; /*this should never happen*/

	return d->ed->is_set_sig;
}

unsigned int euclidean_data_get_sigID(Data d){
	if(d->ed == NULL) return 0; /*this should never happen*/

	return d->ed->sig;
}

void euclidean_data_set_sigID(Data d,unsigned int sigID){
	if(d->ed == NULL) return; /*this should never happen*/

	d->ed->sig = sigID;
	d->ed->is_set_sig = 1;
}

unsigned int data_hammingDistance(Data a,Data b){
	if(a->hd == NULL) return 0; /*this should never happen*/
	if(b->hd == NULL) return 0; /*this should never happen*/

	return user_hammingDistance2(a->hd->user,b->hd->user);
}

double data_euclideanDistance(Data a,Data b){
	if(a->ed == NULL) return 0.0; /*this should never happen*/
	if(b->ed == NULL) return 0.0; /*this should never happen*/

	return user_euclideanDistance2(a->ed->user,b->ed->user);
}

double data_cosineDistance(Data a,Data b){
	User user1;
	User user2;

	if(a->hd) user1 = a->hd->user;
	else if(a->ed) user1 = a->ed->user;
	else if(a->cd) user1 = a->cd->user;
	if(a->hd) user2 = b->hd->user;
	else if(b->ed) user2 = b->ed->user;
	else if(b->cd) user2 = b->cd->user;

	return user_cosineDistance2(user1,user2);
}

unsigned int data_getUserID(Data d){
	if(d->hd) return user_getUserID(d->hd->user);
	if(d->ed) return user_getUserID(d->ed->user);
	if(d->cd) return user_getUserID(d->cd->user);
}

User data_getUser(Data d){
	if(d->hd) return d->hd->user;
	if(d->ed) return d->ed->user;
	if(d->cd) return d->cd->user;
}