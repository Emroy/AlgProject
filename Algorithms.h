#ifndef ALGORITHMS_H
#define ALGORITHMS_H
typedef struct MedoidData* Medoids;

/*CLARA*/
Medoids clara(unsigned int k,unsigned int n);

/*INITIALIZATIONS*/
Medoids k_MedoidsPP(unsigned int k,unsigned int n);

Medoids Park_Jun(unsigned int k,unsigned int n);

/*ASSIGNMENTS*/
void pam(Medoids medoids,unsigned int n);

void lsh_dbh(Medoids medoids,unsigned int n,int k,int L);

/*UPDATES*/
Medoids lloyds(Medoids prevMedoids,unsigned int n);

Medoids clarans(Medoids prevMedoids,unsigned int n,int iter,int frac);

/*UTILITY FUNCTIONS*/
int medoids_areSame(Medoids a,Medoids b);

void medoids_destroy(Medoids m);
#endif
