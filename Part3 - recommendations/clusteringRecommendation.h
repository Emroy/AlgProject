#ifndef CLUSTERING_RECOMMENDATION_H
#define CLUSTERING_RECOMMENDATION_H
typedef struct Medoids
{
	unsigned int k,*m;
}Medoids;

typedef struct Assignment
{
	unsigned int n,*first,*second,*swap;
}Assignment;

void deletion(char token,void *structure);

Assignment* PAM(Medoids *medoids,unsigned int n,char metric);

double silhouette(char metric,unsigned int n,Assignment *assignment);
#endif