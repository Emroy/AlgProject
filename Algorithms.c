#include "Algorithms.h"
#include "data.h"
#include <stdlib.h>

struct MedoidData{
	unsigned int k;
	unsigned int* m;
};

typedef struct Assignment{
	unsigned int* nearest;
	unsigned int* secondNearest;
	unsigned int n;
}Assignment;

static Assignment* currentAssignment = NULL;

/*INITIALIZATIONS*/
Medoids k_MedoidsPP(unsigned int k,unsigned int n,char metric)
{
	int counter=1,*map,i,j,token,l,*Dnatural,*natural,start,end,middle;
	float *Dreal,*real,*P,x;
	Medoids medoid;
	
	if((k<=0)||(n<=0))
	{
	    printf("Error: Invalid data.\n");
	    exit(1);
	}
	if((medoid=realloc(NULL,sizeof(struct MedoidData)))==NULL)
	{
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}
	medoid->k=k;
	if((medoid->m=malloc(k*sizeof(unsigned int)))==NULL)
	{
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}
	medoid->m[0]=integerUniform(n);
	if((metric=='e')||(metric=='c'))
	{
	    while(counter<=k-1)
	    {
	    	if((map=realloc(NULL,(n-counter)*sizeof(int)))==NULL)
	    	{
			    printf("Error: System failure.\n");
			    exit(1);
		    }
		    i=0;
		    for(j=0;j<=n-1;j++)
		    {
		    	token=0;
		    	for(l=0;l<=counter-1;l++)
		    	{
		    		if(j==medoid->m[l])
		    		{
		    			token=1;
		    			break;
		    		}
		    	}
		    	if(!token)
		    	{
		    		map[i]=j;
		    		i++;
		    	}
		    }
	    	if((Dreal=realloc(NULL,(n-counter)*sizeof(float)))==NULL)
		    {
			    printf("Error: System failure.\n");
			    exit(1);
		    }
	    	for(i=0;i<=n-counter-1;i++)
	    	{
			    real=data_getIdDistance(map[i],medoid->m[0]);
			    Dreal[i]=*real;
			    for(j=1;j<=counter-1;j++)
			    {
			    	real=data_getIdDistance(map[i],medoid->m[j]);
			    	if(*real<Dreal[i])
			    	{
			    		Dreal[i]=*real;
			    	}
			    }
			}
			if((P=realloc(NULL,(n-counter)*sizeof(float)))==NULL)
	        {
		        printf("Error: k-medoids++ initialization failure.\n");
		        return NULL;
	        }
			for(i=0;i<=n-counter-1;i++)
			{
				P[i]=0.0;
				for(j=0;j<=i;j++)
				{
					P[i]+=Dreal[j]*Dreal[j];
				}
			}
			x=realUniform(0.0,P[n-counter-1],3);
			start=0;
			end=n-counter-1;
			middle=(start+end)/2;
			while(1)
			{
				if(P[middle]==x)
				{
					medoid->m[counter]=map[middle];
					break;
				}
				else
				{
					if(P[middle]<x)
					{
						start=middle+1;
						if(start>end)
						{
							medoid->m[counter]=map[middle+1];
					        break;
					    }
					    middle=(start+end)/2;
					}
					else
					{
						end=middle-1;
						if(start>end)
						{
							medoid->m[counter]=map[middle];
					        break;
					    }
					    middle=(start+end)/2;
					}
				}
			}
			counter++;
			free(map);
			free(Dreal);
			free(P);
		}
	}
	else
	{
	    while(counter<=k-1)
	    {
	    	if((map=realloc(NULL,(n-counter)*sizeof(int)))==NULL)
	    	{
			    printf("Error: System failure.\n");
			    exit(1);
		    }
		    i=0;
		    for(j=0;j<=n-1;j++)
		    {
		    	token=0;
		    	for(l=0;l<=counter-1;l++)
		    	{
		    		if(j==medoid->m[l])
		    		{
		    			token=1;
		    			break;
		    		}
		    	}
		    	if(!token)
		    	{
		    		map[i]=j;
		    		i++;
		    	}
		    }
	    	if((Dnatural=realloc(NULL,(n-counter)*sizeof(int)))==NULL)
		    {
			    printf("Error: System failure.\n");
			    exit(1);
		    }
	    	for(i=0;i<=n-counter-1;i++)
	    	{
			    natural=data_getIdDistance(map[i],medoid->m[0]);
			    Dnatural[i]=*natural;
			    for(j=1;j<=counter-1;j++)
			    {
			    	natural=data_getIdDistance(map[i],medoid->m[j]);
			    	if(*natural<Dnatural[i])
			    	{
			    		Dnatural[i]=*natural;
			    	}
			    }
			}
			if((P=realloc(NULL,(n-counter)*sizeof(float)))==NULL)
	        {
		        printf("Error: k-medoids++ initialization failure.\n");
		        return NULL;
	        }
			for(i=0;i<=n-counter-1;i++)
			{
				P[i]=0.0;
				for(j=0;j<=i;j++)
				{
					P[i]+=(double)Dnatural[j]*Dnatural[j];
				}
			}
			x=realUniform(0.0,P[n-counter-1],3);
			start=0;
			end=n-counter-1;
			middle=(start+end)/2;
			while(1)
			{
				if(P[middle]==x)
				{
					medoid->m[counter]=map[middle];
					break;
				}
				else
				{
					if(P[middle]<x)
					{
						start=middle+1;
						if(start>end)
						{
							medoid->m[counter]=map[middle+1];
					        break;
					    }
					    middle=(start+end)/2;
					}
					else
					{
						end=middle-1;
						if(start>end)
						{
							medoid->m[counter]=map[middle];
					        break;
					    }
					    middle=(start+end)/2;
					}
				}
			}
			counter++;
			free(map);
			free(Dnatural);
			free(P);
		}
	}
	return medoid;
}
	

Medoids Park_Jun(unsigned int k,unsigned int n)
{
	Medoids medoid;
	
	if((medoid=realloc(NULL,sizeof(struct MedoidData)))==NULL)
	{
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}

	medoid->k=k;
	if((medoid->m=realloc(NULL,k*sizeof(unsigned int)))== NULL)
	{
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}
}

/*ASSIGNMENTS*/
void pam(Medoids medoids,unsigned int n,char metric){
	if(currentAssignment == NULL){
		currentAssignment = malloc(sizeof(Assignment));
		if(currentAssignment == NULL){
			perror("Failed to allocate memory for internal Assignment struct");
			exit(1);
		}

		currentAssignment->nearest = malloc(n*sizeof(unsigned int));
		if(currentAssignment->nearest == NULL){
			perror("Failed to allocate memory for internal Assignment struct nearest");
			exit(2);
		}

		currentAssignment->secondNearest = malloc(n*sizeof(unsigned int));
		if(currentAssignment->nearest == NULL){
			perror("Failed to allocate memory for internal Assignment struct secondNearest");
			exit(3);
		}

		currentAssignment->n = n;
	}

	unsigned int* uDist;
	double* dDist;
	unsigned int min_uDist,min_uDist2;
	double min_dDist,min_dDist2;
	unsigned int i,j;
	for(i=0;i<n;i++){
		/*Initialize min_Dist and minDist2*/
		if(metric == 'h'||metric == 'm'){
			uDist = data_getIdDistance(i,medoids->medoids[0]);
			min_uDist = *uDist;
			currentAssignment->nearest[i] = medoids->medoids[0];

			uDist = data_getIdDistance(i,medoids->medoids[1]);
			if(min_uDist > *uDist){
				min_uDist2 = min_uDist;
				currentAssignment->secondNearest[i]=currentAssignment->nearest[i];
				min_uDist = *uDist;
				currentAssignment->nearest[i] = medoids->medoids[1];
			}
		}
		else{
			dDist = data_getIdDistance(i,medoids->medoids[0]);
			min_dDist = *dDist;
			currentAssignment->nearest[i] = medoids->medoids[0];

			dDist = data_getIdDistance(i,medoids->medoids[1]);
			if(min_dDist > *dDist){
				min_dDist2 = min_dDist;
				currentAssignment->secondNearest[i]=currentAssignment->nearest[i];
				min_dDist = *dDist;
				currentAssignment->nearest[i] = medoids->medoids[1];
			}
		}

		for(j=2;j<medoids->k;j++){
			if(metric == 'h'||metric == 'm'){
				uDist = data_getIdDistance(i,medoids->medoids[j]);
				if(min_uDist > *uDist){
					min_uDist2 = min_uDist;
					currentAssignment->secondNearest[i]=currentAssignment->nearest[i];
					min_uDist = *uDist;
					currentAssignment->nearest[i] = medoids->medoids[j];
				}
			}
			else{
				dDist = data_getIdDistance(i,medoids->medoids[j]);
				if(min_dDist > *dDist){
					min_dDist2 = min_dDist;
					currentAssignment->secondNearest[i]=currentAssignment->nearest[i];
					min_dDist = *dDist;
					currentAssignment->nearest[i] = medoids->medoids[j];
				}
			}
		}
	}
}

void lsh_dbh(Medoids medoids,unsigned int n,int k,int L){
	if(currentAssignment == NULL){
		currentAssignment = malloc(sizeof(Assignment));
		if(currentAssignment == NULL){
			perror("Failed to allocate memory for internal Assignment struct");
			exit(1);
		}

		currentAssignment->nearest = malloc(n*sizeof(unsigned int));
		if(currentAssignment->nearest == NULL){
			perror("Failed to allocate memory for internal Assignment struct nearest");
			exit(2);
		}

		currentAssignment->secondNearest = malloc(n*sizeof(unsigned int));
		if(currentAssignment->nearest == NULL){
			perror("Failed to allocate memory for internal Assignment struct secondNearest");
			exit(3);
		}

		currentAssignment->n = n;
	}
}

/*UPDATES*/
Medoids lloyds(Medoids prevMedoids,unsigned int n){
	Medoids retVal = malloc(sizeof(struct MedoidData));
	if(retVal == NULL){
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}

	retVal->k = prevMedoids->k;
	retVal->medoids = malloc(retVal->k*sizeof(unsigned int));
	if(retVal->medoids == NULL){
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}

	unsigned int i;
	for(i=0;i<prevMedoids->k;i++){

	}
}

Medoids clarans(Medoids prevMedoids,unsigned int n,int iter,int frac){
	Medoids retVal = malloc(sizeof(struct MedoidData));
	if(retVal == NULL){
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}

	retVal->k = prevMedoids->k;
	retVal->medoids = malloc(retVal->k*sizeof(unsigned int));
	if(retVal->medoids == NULL){
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}
}

/*UTILITY FUNCTIONS*/
int medoids_areSame(Medoids a,Medoids b){
	unsigned int i;
	for(i=0;i<a->k;i++){
		if(a->medoids[i] != b->medoids[i]) return 0;
	}

	return 1;
}

void medoids_destroy(Medoids m){
	free(m->medoids);
	free(m);
}

void medoids_printCluster(Medoids m,FILE* file,int complete){
	unsigned int i,j,size = 0;
	for(i=0;i<m->k;i++){
		fprintf(file,"CLUSTER-%u {",i);
		for(j=0;j<currentAssignment->n;j++){
			if(currentAssignment->nearest[j] == m->medoids[i]){
				if(complete) fprintf(file,"item_id%u,",j);
				else size++;
			}
		}
		if(complete) fprintf(file,"}\n");
		else fprintf(file,"size: %u, medoid: %u}\n",size,m->medoids[i]);
	}
}

void medoids_printSilhouette(Medoids m,FILE* file,char metric){
	unsigned int i,j,k,counta,countb,counts;
	unsigned int* uDist = NULL;
	double* dDist = NULL;
	double ai,bi,si,stotal = 0.0;

	fprintf(file,"Silhouette: [");

	for(i=0;i<m->k;i++){
		si = 0.0;
		counts = 0;
		for(j=0;j<currentAssignment->n;j++){
			ai = 0.0;
			bi = 0.0;
			counta = 0;
			countb = 0;
			counts++;

			for(k=0;k<currentAssignment->n;k++){
				if(currentAssignment->nearest[k] == currentAssignment->nearest[j]){
					if(metric == 'h'||metric == 'm'){
						uDist = data_getIdDistance(j,k);
						ai += (double)*uDist;
					}
					else{
						dDist = data_getIdDistance(j,k);
						ai += *dDist;
					}
					counta++;
				}
				else if(currentAssignment->secondNearest[j] == currentAssignment->nearest[k]){
					if(metric == 'h'||metric == 'm'){
						uDist = data_getIdDistance(j,k);
						bi += (double)*uDist;
					}
					else{
						dDist = data_getIdDistance(j,k);
						bi += *dDist;
					}
					countb++;
				}
			}

			ai /= counta;
			bi /= countb;


			if(currentAssignment->nearest[j] == m->medoids[i]){
				if(ai < bi) si += 1-ai/bi;
				else if(ai > bi) si += bi/ai-1;
			}
			if(i==0){
				if(ai < bi) stotal += 1-ai/bi;
				else if(ai > bi) stotal += bi/ai-1;
			}
		}

		si /= counts;
		fprintf(file,"s%u=%f,",i,si);
	}

	stotal /= currentAssignment->n;
	fprintf(file,"stotal=%f]\n",stotal);

	free(currentAssignment->nearest);
	free(currentAssignment->secondNearest);
	free(currentAssignment);
}