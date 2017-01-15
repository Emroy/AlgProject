#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#include "Clustering.h"
#include "data.h"
#define LSH_L 5
#define LSH_K 4

/*calculates 2^num*/
unsigned int two_pow(unsigned int num){
	unsigned int i;
	unsigned int retVal = 1;
	for(i=0;i<num;i++) retVal*=2;
	return retVal;
}

static unsigned short neighborsNum;

/*NN-LSH*/
static HashTable* lsh_tables = NULL;
static char m; //metric

void lsh_init(Ratings ratings,char metric)
{
	if(lsh_tables) lsh_terminate();
	lsh_tables = malloc(LSH_L*sizeof(HashTable));
	if(lsh_tables == NULL){
		perror("Failed to allocate memory for lsh tables array");
		return;
	}

	normalizeRatings(ratings,metric);

	if(metric == 'h'){
		unsigned int d = ratings_calculateHammingDim(ratings);
		unsigned int i;
		for(i=0;i<LSH_L;i++){
			HashDescriptor g = hamming_hash_create(d,LSH_K);
			lsh_tables[i] = hashTable_create(two_pow(LSH_K),g);
		}

		User currentUser;
		while(currentUser = getNextUser(ratings)){
			Data data = hammingData_create(currentUser);
			if(data == NULL){
				fprintf(stderr,"lsh initialization cannot continue\n");
				exit(-1);
			}

			for(i=0;i<LSH_L;i++)
				hashTable_insert(lsh_tables[i],data);
		}
	}
	else if(metric == 'e'){
		unsigned int d = ratings_calculateEuclideanDim(ratings);
		unsigned int n = ratings_getNumberOfUsers(ratings);
		unsigned int i;
		for(i=0;i<LSH_L;i++){
			HashDescriptor g = euclidean_hash_create(d,LSH_K,n);
			lsh_tables[i] = hashTable_create(n/2,g);
		}

		User currentUser;
		while(currentUser = getNextUser(ratings)){
			Data data = euclideanData_create(currentUser);
			if(data == NULL){
				fprintf(stderr,"lsh initialization connot continue\n");
				exit(-1);
			}

			for(i=0;i<LSH_L;i++)
				hashTable_insert(lsh_tables[i],data);
		}
	}
	else if(metric == 'c'){
		//euclidean and cosine dims are the same
		unsigned int d = ratings_calculateEuclideanDim(ratings);
		unsigned int i;
		for(i=0;i<LSH_L;i++){
			HashDescriptor g = cosine_hash_create(d,LSH_K);
			lsh_tables[i] = hashTable_create(two_pow(LSH_K),g);
		}

		User currentUser;
		while(currentUser = getNextUser(ratings)){
			Data data = cosineData_create(currentUser);
			if(data == NULL){
				fprintf(stderr,"lsh initialization connot continue\n");
				exit(-1);
			}

			for(i=0;i<LSH_L;i++)
				hashTable_insert(lsh_tables[i],data);
		}
	}
	else{
		fprintf(stderr,"Metric %c is not supported\n",metric);
		exit(-1);
	}

	neighborsNum = ratings_getNeighborsNum(ratings);
	m = metric;
}

unsigned int* lsh_getRecommendedItems(User user){
	unsigned int* retVal = malloc(sizeof(unsigned int)*5);
	if(retVal == NULL){
		perror("Failed to allocate memory for top 5 ratings");
		return NULL;
	}

	Data current;
	if(m == 'h')
		current = hammingData_create(user);
	else if(m == 'e')
		current = euclideanData_create(user);
	else if(m == 'c')
		current = cosineData_create(user);

	
}

void lsh_terminate(){
	unsigned int i;
	Data data;
	for(i=0;i<LSH_L;i++){
		while(data = hashTable_getAll(lsh_tables[i])){
			data_destroy(data);
		}
		hashTable_destroy(lsh_tables[i]);
	}
	free(lsh_tables);
}

typedef struct Unit
{
    unsigned int rating,index;
}Unit;

int f(const void *x_1,const void *x_2)
{
	const Unit *u_1=(const Unit*)x_1;
	const Unit *u_2=(const Unit*)x_2;
	
	if(u_1->rating<u_2->rating)
	{
		return -1;
	}
	if(u_1->rating==u_2->rating)
	{
		return 0;
	}
	if(u_1->rating>u_2->rating)
	{
		return 1;
	}
}

int clustering_init(Ratings ratings,char metric)
{
	unsigned int n,m,i,j,start=2,end=n/2,counter;
	unsigned short k[2],token=0;
	double s[2],sim,sum,z;
	clock_t t_1,t_2;
	Medoids *medoids[2];
	Assignment *assignment[2];
	Unit *unit;
	User user;
	int8_t *vector,*flags;
	FILE *output;
	
	t_1=clock();
	normalizeRatings(ratings,metric);
	if((medoids[0]=realloc(NULL,sizeof(Medoids)))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	n=ratings_getNumberOfUsers(ratings);
	m=ratings_getNumberOfItems(ratings);
	k[0]=n/ratings_getNumberOfNeighbors(ratings);
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
	k[1]=(start+k[0])/2;
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
	    	deletion('M',medoids[0]);
	    	deletion('A',assignment[0]);
	    	medoids[0]=medoids[1];
	    	assignment[0]=assignment[1];
	    }
	    else
	    {
		    deletion('M',medoids[1]);
	    	deletion('A',assignment[1]);
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
	while(start<=end+1);
	if((output=fopen("clustering.txt","w"))==NULL)
	{
		printf("Error: Failed to create the file \"clustering.txt\".\n");
		return 1;
	}
	switch(metric)
	{
		case 'h':
		    fprintf(output,"Hamming Clustering\n");
		    break;
		case 'e':
			fprintf(output,"Euclidean Clustering\n");
		    break;
		case 'c':
			fprintf(output,"Cosine Clustering\n");
		    break;
		default:
			printf("Error: Unknown metric.\n");
			return 1;
	}
	for(i=0;i<=n-1;i++)
	{
	    user=ratings_getUser(ratings,i+1);
		vector=user_getRatingsVector(user);
		flags=user_getRatingFlags(user);
		counter=0;
		unit=NULL;
		for(j=0;j<=m-1;j++)
		{
			if(flags[j])
			{
				counter++;
				sum=z=0.0;
				for(start=0;start<=n-1;start++)
				{
					if(i!=start)
					{
					    if(assignment[0]->first[i]==assignment[0]->first[start])
					    {
						    sim=1-user_cosineDistance(i+1,start+1);
						    sum+=sim*vector[j];
						    if(sim>=0)
						    {
							    z+=sim;
						    }
						    else
						    {
							    z+=-sim;
						    }
					    }
					}
				}
			    z=1/z;
			    vector[j]=z*sum;
			    if((unit=realloc(unit,counter*sizeof(Unit)))==NULL)
	            {
		            printf("Error: System failure.\n");
		            exit(1);
	            }
	            unit[counter-1].rating=vector[j];
	            unit[counter-1].index=j+1;
	        }
		}
		qsort(unit,counter,sizeof(Unit),f);
		fprintf(output,"%u",i+1);
		for(j=0;j<=4;j++)
		{
			fprintf(output,"	%u",unit[j].index);
		}
		fprintf(output,"\n");
		free(unit);
	}
	t_2=clock();
	fprintf(output,"Execution Time: %f milliseconds.\n",1000*(double)(t_2-t_1)/CLOCKS_PER_SEC);
	deletion('M',medoids[0]);
	deletion('A',assignment[0]);
	fclose(output);
	return 0;
}

void clustering_terminate(void)
{
	return;
}
