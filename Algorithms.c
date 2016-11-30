#include "Algorithms.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>

struct MedoidData{
	unsigned int k;
};

/*INITIALIZATIONS*/
Medoids k_MedoidsPP(unsigned int k,unsigned int n){
	Medoids retVal = malloc(sizeof(struct MedoidData));
	if(retVal == NULL){
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}


}

Medoids park_Jun(unsigned int k,unsigned int n){
	Medoids retVal = malloc(sizeof(struct MedoidData));
	if(retVal == NULL){
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}

	
}

/*ASSIGNMENTS*/
void pam(Medoids medoids,unsigned int n){

}

void lsh_dbh(Medoids medoids,unsigned int n,int k,int L){

}

/*UPDATES*/
Medoids lloyds(Medoids prevMedoids,unsigned int n){

}

Medoids clarans(Medoids prevMedoids,unsigned int n,int iter,int frac){

}

/*UTILITY FUNCTIONS*/
int medoids_areSame(Medoids a,Medoids b){

}

void medoids_destroy(Medoids m){
	free(m);
}