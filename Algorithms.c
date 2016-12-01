#include "Algorithms.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>

struct MedoidData{
	unsigned int k;
	unsigned int* m;
};

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
void pam(Medoids medoids,unsigned int n){

}

void lsh_dbh(Medoids medoids,unsigned int n,int k,int L){

}

/*UPDATES*/
Medoids lloyds(Medoids prevMedoids,unsigned int n){
	Medoids retVal = malloc(sizeof(struct MedoidData));
	if(retVal == NULL){
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}

	retVal->k = prevMedoids->k;
	retVal->medoids = malloc(k*sizeof(unsigned int));
	if(retVal->medoids == NULL){
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}
}

Medoids clarans(Medoids prevMedoids,unsigned int n,int iter,int frac){
	Medoids retVal = malloc(sizeof(struct MedoidData));
	if(retVal == NULL){
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}

	retVal->k = prevMedoids->k;
	retVal->medoids = malloc(k*sizeof(unsigned int));
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
