#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#define LSH_L 5
#define LSH_K 4

/*calculates 2^num*/
unsigned int two_pow(unsigned int num){
	unsigned int i;
	unsigned int retVal = 1;
	for(i=0;i<num;i++) retVal*=2;
	return retVal;
}

static unsigned short range;

struct NearestNeighbors{
	User* users;
};

/*NN-LSH*/

void lsh_init(Ratings ratings,char metric)
{
	int i,j;
	int8_t mask;
	User currentUser;

	switch(metric)
	{
		case 'h':
			while(currentUser = getNextUser(ratings))
			{
				int8_t* vector = getUserVector(currentUser);
				int8_t* hammingData;
				unsigned int numOfItems = getNumberOfItems(ratings);
				unsigned int hammingDataSize = numOfItems/8;
				if(numOfItems % 8) hammingDataSize++;
				hammingData = malloc(sizeof(int8_t)*hammingDataSize);
				if(hammingData == NULL)
				{
					perror("Failed to allocate memory for hammingData");
					exit(-2);
				}
				for(i=0;i<=hammingDataSize-1;i++)
				{
					hammingData[i]=0;
				}
				j=0;
				mask=1;
				for(i=0;i<=numOfItems-1;i++)
				{
				    if(vector[i])
				    {
				    	hammingData[j]|=mask;
				    }
				    if(!(mask<<=1))
				    {
				    	mask=1;
				    	j++;
				    }
				}
			}
			
}

Neighbors lsh_getNeighbors(User user){

}

void lsh_terminate(){

}

void clustering_init(Ratings ratings,char metric)
{
	unsigned int i,j,n=ratings_getNumberOfUsers(ratings),start=1,end=n;
	unsigned short k[2],token=0;
	double s[2];
	Medoids *medoids[2];
	Assignment *assignment[2];
	
	if((medoids[0]=realloc(NULL,sizeof(Medoids)))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	k[0]=n/ratings_getNumberOfNeighbors(ratings);
	k[1]=(start+k[0])/2;
	if((medoids[0]->m=realloc(NULL,k[0]*sizeof(unsigned int)))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	medoids[0]->k=k[0];
	for(i=0;i<=k[0]-1;i++)
	{
		do
		{
		    medoids[0]->m[i]=integerUniform(n)+1;
		    for(j=0;j<=i-1;j++)
		    {
		    	if(medoids[0]->m[i]==medoids[0]->m[j])
		    	{
		    		j=i+1;
		    		break;
		    	}
		    }
		}
		while(j==i+1);
	}
	assignment[0]=PAM(medoids[0],n,metric);
	s[0]=silhouette(metric,n,assignment[0]);
	do
	{
	    if((medoids[1]=realloc(NULL,sizeof(Medoids)))==NULL)
	    {
		    printf("Error: System failure.\n");
		    exit(1);
	    }
	    if((medoids[1]->m=realloc(NULL,k[1]*sizeof(unsigned int)))==NULL)
	    {
		    printf("Error: System failure.\n");
		    exit(1);
	    }
	    medoids[1]->k=k[1];
	    for(i=0;i<=k[1]-1;i++)
	    {
		    do
		    {
		        medoids[1]->m[i]=integerUniform(n)+1;
		        for(j=0;j<=i-1;j++)
		        {
		    	    if(medoids[1]->m[i]==medoids[1]->m[j])
		    	    {
		    		    j=i+1;
		    		    break;
		    	    }
		        }
		    }
		    while(j==i+1);
	    }
	    assignment[1]=PAM(medoids[1],n,metric);
	    s[1]=silhouette(metric,n,assignment[1]);
	    if(s[1]>s[0])
	    {
	    	if(!token)
	    	{
			    end=k[0];
			}
			else
			{
				start=k[0];
				token=0;
			}
	    	k[0]=k[1];
	    	k[1]=(start+k[0])/2;
	    	free(medoids[0]->m);
	    	free(medoids[0]);
	    	free(assignment[0]);
	    	medoids[0]=medoids[1];
	    	assignment[0]=assignment[1];
	    }
	    else
	    {
		    free(medoids[1]->m);
	    	free(medoids[1]);
	    	free(assignment[1]);
	    	if(!token)
	    	{
	    	    k[1]=(k[0]+end)/2;
	    	    token=1;
	    	}
	    	else
	    	{
	    		break;
	    	}
	    }
	}
	while((k[1]!=start)&&(k[1]!=end));
}

Neighbors clustering_getNeighbors(User user){

}

void clustering_terminate(){

}
