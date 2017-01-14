#include "Algorithms.h"
#include "data.h"
#include "RNG.h"
#include <stdlib.h>

typedef struct MedoidData
{
	unsigned int k,*m;
}MedoidData;
typedef struct Assignment
{
	unsigned int n,*first,*second,*swap;
}Assignment;
static Assignment *currentAssignment=NULL;
typedef struct Values
{
	double v;
	int index;
}Values;
int compF(const void* a,const void* b)
{
	const Values* A=(const Values*)a;
	const Values* B=(const Values*)b;
	if((A->v)>(B->v))
	{
		return 1;
	}
	if((A->v)==(B->v))
	{
		return 0;
	}
	if((A->v)<(B->v))
	{
		return -1;
	}
}
/*INITIALIZATIONS*/
Medoids k_MedoidsPP(unsigned int k,unsigned int n,char metric)
{
	unsigned int counter=1,*map,i,j,token,l,*Dnatural,*natural,start,end,middle;
	float *Dreal,*real,*P,x;
	Medoids medoids;
	
	if((k<=0)||(n<=0))
	{
	    printf("Error: Invalid data.\n");
	    exit(1);
	}
	if((medoids=realloc(NULL,sizeof(struct MedoidData)))==NULL)
	{
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}
	medoids->k=k;
	if((medoids->m=malloc(k*sizeof(unsigned int)))==NULL)
	{
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}
	medoids->m[0]=integerUniform(n);
	if((metric=='e')||(metric=='c'))
	{
	    while(counter<=k-1)
	    {
	    	if((map=realloc(NULL,(n-counter)*sizeof(unsigned int)))==NULL)
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
		    		if(j==medoids->m[l])
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
			    real=data_getIdDistance(map[i],medoids->m[0]);
			    Dreal[i]=*real;
			    for(j=1;j<=counter-1;j++)
			    {
			    	real=data_getIdDistance(map[i],medoids->m[j]);
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
					medoids->m[counter]=map[middle];
					break;
				}
				else
				{
					if(P[middle]<x)
					{
						start=middle+1;
						if(start>end)
						{
							medoids->m[counter]=map[middle+1];
					        break;
					    }
					    middle=(start+end)/2;
					}
					else
					{
						end=middle-1;
						if(start>end)
						{
							medoids->m[counter]=map[middle];
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
	    	if((map=realloc(NULL,(n-counter)*sizeof(unsigned int)))==NULL)
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
		    		if(j==medoids->m[l])
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
	    	if((Dnatural=realloc(NULL,(n-counter)*sizeof(unsigned int)))==NULL)
		    {
			    printf("Error: System failure.\n");
			    exit(1);
		    }
	    	for(i=0;i<=n-counter-1;i++)
	    	{
			    natural=data_getIdDistance(map[i],medoids->m[0]);
			    Dnatural[i]=*natural;
			    for(j=1;j<=counter-1;j++)
			    {
			    	natural=data_getIdDistance(map[i],medoids->m[j]);
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
					medoids->m[counter]=map[middle];
					break;
				}
				else
				{
					if(P[middle]<x)
					{
						start=middle+1;
						if(start>end)
						{
							medoids->m[counter]=map[middle+1];
					        break;
					    }
					    middle=(start+end)/2;
					}
					else
					{
						end=middle-1;
						if(start>end)
						{
							medoids->m[counter]=map[middle];
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
	return medoids;
}
	

Medoids Park_Jun(unsigned int k,unsigned int n,char metric)
{
	Medoids medoids;
	Values *values;
	unsigned int i,j,t,**dNatural,*natural,sumNatural;
	double **dReal,*real,sumReal;
	
	if((medoids=realloc(NULL,sizeof(struct MedoidData)))==NULL)
	{
		perror("Failed to allocate memory for medoids on Park-Jun initialization");
		return NULL;
	}
	medoids->k=k;
	if((medoids->m=realloc(NULL,k*sizeof(unsigned int)))==NULL)
	{
		perror("Failed to allocate memory for medoids on Park-Jun initialization");
		return NULL;
	}
	if((values=realloc(NULL,n*sizeof(Values)))==NULL)
	{
		perror("Failed to allocate memory for medoids on Park-Jun initialization");
		return NULL;
	}
	if((metric=='e')||(metric=='c'))
	{
		if((dReal=realloc(NULL,sizeof(double*)))==NULL)
		{
		    perror("Failed to allocate memory for medoids on Park-Jun initialization");
		    return NULL;
	    }
	    for(i=0;i<=n-1;i++)
	    {
	    	if((dReal[i]=realloc(NULL,sizeof(double)))==NULL)
		    {
		        perror("Failed to allocate memory for medoids on Park-Jun initialization");
		        return NULL;
	        }
	    }
	    for(i=0;i<=n-1;i++)
	    {
	    	for(j=0;j<=n-1;j++)
	    	{
	    		real=data_getIdDistance(i,j);
	    		dReal[i][j]=*real;
	    	}
	    }
	    for(i=0;i<=n-1;i++)
	    {
	    	values[i].v=0.0;
	    	values[i].index=i;
	    	for(j=0;j<=n-1;j++)
	    	{
			    sumReal=0.0;
	    		for(t=0;t<=n-1;t++)
	    		{
	    			sumReal+=dReal[j][t];
	    		}
	    		values[i].v+=dReal[i][j]/sumReal;
	    	}
	    }
	    qsort(values,n,sizeof(Values),compF);
	    for(i=0;i<=k-1;i++)
	    {
	    	medoids->m[i]=values[i].index;
	    }
	}
	else
	{
		if((dNatural=realloc(NULL,sizeof(unsigned int*)))==NULL)
		{
		    perror("Failed to allocate memory for medoids on Park-Jun initialization");
		    return NULL;
	    }
	    for(i=0;i<=n-1;i++)
	    {
	    	if((dNatural[i]=realloc(NULL,sizeof(unsigned int)))==NULL)
		    {
		        perror("Failed to allocate memory for medoids on Park-Jun initialization");
		        return NULL;
	        }
	    }
	    for(i=0;i<=n-1;i++)
	    {
	    	for(j=0;j<=n-1;j++)
	    	{
	    		natural=data_getIdDistance(i,j);
	    		dNatural[i][j]=*natural;
	    	}
	    }
	    for(i=0;i<=n-1;i++)
	    {
	    	values[i].v=0.0;
	    	values[i].index=i;
	    	for(j=0;j<=n-1;j++)
	    	{
			    sumNatural=0;
	    		for(t=0;t<=n-1;t++)
	    		{
	    			sumNatural+=dNatural[j][t];
	    		}
	    		values[i].v+=(double)dNatural[i][j]/sumNatural;
	    	}
	    }
	    qsort(values,n,sizeof(Values),compF);
	    for(i=0;i<=k-1;i++)
	    {
	    	medoids->m[i]=values[i].index;
	    }
	}
	return medoids;
}

/*ASSIGNMENTS*/
void PAM(Medoids medoids,unsigned int n,char metric)
{
	void *first=NULL,*second=NULL;
	unsigned int i,j,k,natural,fNatural=0;
	double real,fReal=0.0;
	
	if(currentAssignment==NULL)
	{
	    if((currentAssignment=realloc(NULL,sizeof(Assignment)))==NULL)
		{
			printf("Error: System failure.\n");
			exit(1);
		}
		if((currentAssignment->first=realloc(NULL,n*sizeof(unsigned int)))==NULL)
		{
			printf("Error: System failure.\n");
			exit(1);
		}
		if((currentAssignment->second=realloc(NULL,n*sizeof(unsigned int)))==NULL)
		{
			printf("Error: System failure.\n");
			exit(1);
		}
		if((currentAssignment->swap=realloc(NULL,n*sizeof(unsigned int)))==NULL)
		{
			printf("Error: System failure.\n");
			exit(1);
		}
		currentAssignment->n=n;
	}
	switch(metric)
	{
		case 'h':
			do
			{
			    for(i=0;i<=n-1;i++)
	            {
				    for(j=0;j<=medoids->k-1;j++)
		            {
		        	    natural=user_hammingDistance(i+1,medoids->m[j]);
			            if(first==NULL)
			            {
						    if((first=realloc(NULL,sizeof(unsigned int)))==NULL)
			                {
			                    printf("Error: System failure.\n");
			                    exit(1);
			                }
				            *first=natural;
				            currentAssignment->first[i]=medoids->m[j];
			            }
			            else
			            {
				            if(natural<*first)
				            {
				        	    if(second==NULL)
			                    {
			                        if((second=realloc(NULL,sizeof(unsigned int)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                    }
					            *second=*first;
					            *first=natural;
					            currentAssignment->second[i]=currentAssignment->first[i];
			                    currentAssignment->first[i]=medoids->m[j];
			                }
			                else
			                {
			            	    if(second==NULL)
			                    {
			                        if((second=realloc(NULL,sizeof(unsigned int)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                        *second=natural;
				                    currentAssignment->second[i]=medoids->m[j];
			                    }
			                    else
			                    {
			                        if(natural<*second)
					                {
						                *second=natural;
						                currentAssignment->second[i]=medoids->m[j];
			                        }
			                    }
			                }
			            }
		            }
		        }
		        if(!fNatural)
		        {
				    for(i=0;i<=n-1;i++)
		            {
		    	        fNatural+=user_hammingDistance(i+1,currentAssignment->first[i]);
		            }
		        }
		        for(i=0;i<=medoids->k-1;i++)
		        {
		    	    for(j=0;j<=n-1;j++)
	                {
	                	if(j+1!=currentAssignment->first[j])
	                	{
	                		for(k=0;k<=n-1;k++)
	                		{
	                			if(currentAssignment->first[k]==medoids->m[i])
	                			{
	                				if(user_hammingDistance(k+1,j+1)<=user_hammingDistance(k+1,currentAssignment->second[k]))
	                				{
	                					currentAssignment->swap[k]=j+1;
	                				}
	                				else
	                				{
	                					currentAssignment->swap[k]=currentAssignment->second[k];
	                				}
	                			}
	                			else
	                			{
	                				if(user_hammingDistance(k+1,j+1)>=user_hammingDistance(k+1,currentAssignment->first[k]))
	                				{
	                					currentAssignment->swap[k]=currentAssignment->first[k];
	                				}
	                				else
	                				{
	                					currentAssignment->swap[k]=j+1;
	                				}
	                			}
	                		}
	                		natural=0;
	                		for(i=0;i<=n-1;i++)
		                    {
		    	                natural+=user_hammingDistance(i+1,currentAssignment->swap[i]);
		                    }
		                    if(natural<fNatural)
		                    {
		                    	fNatural=natural;
		                    	medoids->m[i]=j+1;
		                    	i=medoids->k+1;
		                    	break;
		                    }
		                }
		            }
		        }
		    }
		    while(i>=medoids->k+1);
		    break;
		case 'e':
			do
			{
			    for(i=0;i<=n-1;i++)
	            {
				    for(j=0;j<=medoids->k-1;j++)
		            {
		        	    real=user_euclideanDistance(i+1,medoids->m[j]);
			            if(first==NULL)
			            {
						    if((first=realloc(NULL,sizeof(double)))==NULL)
			                {
			                    printf("Error: System failure.\n");
			                    exit(1);
			                }
				            *first=real;
				            currentAssignment->first[i]=medoids->m[j];
			            }
			            else
			            {
				            if(real<*first)
				            {
				        	    if(second==NULL)
			                    {
			                        if((second=realloc(NULL,sizeof(double)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                    }
					            *second=*first;
					            *first=real;
					            currentAssignment->second[i]=currentAssignment->first[i];
			                    currentAssignment->first[i]=medoids->m[j];
			                }
			                else
			                {
			            	    if(second==NULL)
			                    {
			                        if((second=realloc(NULL,sizeof(double)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                        *second=real;
				                    currentAssignment->second[i]=medoids->m[j];
			                    }
			                    else
			                    {
			                        if(real<*second)
					                {
						                *second=real;
						                currentAssignment->second[i]=medoids->m[j];
			                        }
			                    }
			                }
			            }
		            }
		        }
		        if(fReal==0.0)
		        {
				    for(i=0;i<=n-1;i++)
		            {
		    	        fReal+=user_euclideanDistance(i+1,currentAssignment->first[i]);
		            }
		        }
		        for(i=0;i<=medoids->k-1;i++)
		        {
		    	    for(j=0;j<=n-1;j++)
	                {
	                	if(j+1!=currentAssignment->first[j])
	                	{
	                		for(k=0;k<=n-1;k++)
	                		{
	                			if(currentAssignment->first[k]==medoids->m[i])
	                			{
	                				if(user_euclideanDistance(k+1,j+1)<=user_euclideanDistance(k+1,currentAssignment->second[k]))
	                				{
	                					currentAssignment->swap[k]=j+1;
	                				}
	                				else
	                				{
	                					currentAssignment->swap[k]=currentAssignment->second[k];
	                				}
	                			}
	                			else
	                			{
	                				if(user_euclideanDistance(k+1,j+1)>=user_euclideanDistance(k+1,currentAssignment->first[k]))
	                				{
	                					currentAssignment->swap[k]=currentAssignment->first[k];
	                				}
	                				else
	                				{
	                					currentAssignment->swap[k]=j+1;
	                				}
	                			}
	                		}
	                		real=0.0;
	                		for(i=0;i<=n-1;i++)
		                    {
		    	                real+=user_euclideanDistance(i+1,currentAssignment->swap[i]);
		                    }
		                    if(real<fReal)
		                    {
		                    	fReal=real;
		                    	medoids->m[i]=j+1;
		                    	i=medoids->k+1;
		                    	break;
		                    }
		                }
		            }
		        }
		    }
		    while(i>=medoids->k+1);
		    break;
		case 'c':
			do
			{
			    for(i=0;i<=n-1;i++)
	            {
				    for(j=0;j<=medoids->k-1;j++)
		            {
		        	    real=user_cosineDistance(i+1,medoids->m[j]);
			            if(first==NULL)
			            {
						    if((first=realloc(NULL,sizeof(double)))==NULL)
			                {
			                    printf("Error: System failure.\n");
			                    exit(1);
			                }
				            *first=real;
				            currentAssignment->first[i]=medoids->m[j];
			            }
			            else
			            {
				            if(real<*first)
				            {
				        	    if(second==NULL)
			                    {
			                        if((second=realloc(NULL,sizeof(double)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                    }
					            *second=*first;
					            *first=real;
					            currentAssignment->second[i]=currentAssignment->first[i];
			                    currentAssignment->first[i]=medoids->m[j];
			                }
			                else
			                {
			            	    if(second==NULL)
			                    {
			                        if((second=realloc(NULL,sizeof(double)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                        *second=real;
				                    currentAssignment->second[i]=medoids->m[j];
			                    }
			                    else
			                    {
			                        if(real<*second)
					                {
						                *second=real;
						                currentAssignment->second[i]=medoids->m[j];
			                        }
			                    }
			                }
			            }
		            }
		        }
		        if(fReal==0.0)
		        {
				    for(i=0;i<=n-1;i++)
		            {
		    	        fReal+=user_cosineDistance(i+1,currentAssignment->first[i]);
		            }
		        }
		        for(i=0;i<=medoids->k-1;i++)
		        {
		    	    for(j=0;j<=n-1;j++)
	                {
	                	if(j+1!=currentAssignment->first[j])
	                	{
	                		for(k=0;k<=n-1;k++)
	                		{
	                			if(currentAssignment->first[k]==medoids->m[i])
	                			{
	                				if(user_cosineDistance(k+1,j+1)<=user_cosineDistance(k+1,currentAssignment->second[k]))
	                				{
	                					currentAssignment->swap[k]=j+1;
	                				}
	                				else
	                				{
	                					currentAssignment->swap[k]=currentAssignment->second[k];
	                				}
	                			}
	                			else
	                			{
	                				if(user_cosineDistance(k+1,j+1)>=user_cosineDistance(k+1,currentAssignment->first[k]))
	                				{
	                					currentAssignment->swap[k]=currentAssignment->first[k];
	                				}
	                				else
	                				{
	                					currentAssignment->swap[k]=j+1;
	                				}
	                			}
	                		}
	                		real=0.0;
	                		for(i=0;i<=n-1;i++)
		                    {
		    	                real+=user_cosineDistance(i+1,currentAssignment->swap[i]);
		                    }
		                    if(real<fReal)
		                    {
		                    	fReal=real;
		                    	medoids->m[i]=j+1;
		                    	i=medoids->k+1;
		                    	break;
		                    }
		                }
		            }
		        }
		    }
		    while(i>=medoids->k+1);
		    break;
		default:
			printf("Error: Unknown metric.\n");
			exit(1);
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
	retVal->m = malloc(retVal->k*sizeof(unsigned int));
	if(retVal->m == NULL){
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
	retVal->m = malloc(retVal->k*sizeof(unsigned int));
	if(retVal->m == NULL){
		perror("Failed to allocate memory for medoids on k_MedoidsPP initialization");
		return NULL;
	}
}

/*UTILITY FUNCTIONS*/
int medoids_areSame(Medoids a,Medoids b){
	unsigned int i;
	for(i=0;i<a->k;i++){
		if(a->m[i] != b->m[i]) return 0;
	}

	return 1;
}

void medoids_destroy(Medoids m){
	free(m->m);
	free(m);
}

void medoids_printCluster(Medoids m,FILE* file,int complete){
	unsigned int i,j,size = 0;
	for(i=0;i<m->k;i++){
		fprintf(file,"CLUSTER-%u {",i);
		for(j=0;j<currentAssignment->n;j++){
			if(currentAssignment->nearest[j] == m->m[i]){
				if(complete) fprintf(file,"item_id%u,",j);
				else size++;
			}
		}
		if(complete) fprintf(file,"}\n");
		else fprintf(file,"size: %u, medoid: %u}\n",size,m->m[i]);
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


			if(currentAssignment->nearest[j] == m->m[i]){
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

Medoids clara(unsigned int k,unsigned int n){
	return NULL;
}
