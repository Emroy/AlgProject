#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <stdio.h>
typedef struct MedoidData* Medoids;

/*CLARA*/
Medoids clara(unsigned int k,unsigned int n);

/*INITIALIZATIONS*/
Medoids k_MedoidsPP(unsigned int k,unsigned int n,char metric);

Medoids Park_Jun(unsigned int k,unsigned int n,char metric);

/*ASSIGNMENTS*/
void PAM(Medoids medoids,unsigned int n,char metric);

void lsh_dbh(Medoids medoids,unsigned int n,int k,int L);

/*UPDATES*/
Medoids lloyds(Medoids prevMedoids,unsigned int n);

Medoids clarans(Medoids prevMedoids,unsigned int n,int iter,int frac);

/*UTILITY FUNCTIONS*/
int medoids_areSame(Medoids a,Medoids b);

void medoids_destroy(Medoids m);

void medoids_printCluster(Medoids m,FILE* file,int complete);

void medoids_printSilhouette(Medoids m,FILE* file,char metric);
#endif
