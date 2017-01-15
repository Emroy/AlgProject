#include "clusteringRecommendation.h"
#include <stdlib.h>
#include <stdio.h>
#include "Ratings.h"

void deletion(char token,void *structure)
{
	Medoids *medoids;
	Assignment *assignment;
	
	switch(token)
	{
		case 'M':
			medoids=(Medoids*)structure;
			free(medoids->m);
			free(medoids);
			break;
		case 'A':
			assignment=(Assignment*)structure;
			free(assignment->first);
			free(assignment->second);
			free(assignment->swap);
			free(assignment);
			break;
		default:
			printf("Error: Unknown structure.\n");
	}
}

Assignment* PAM(Medoids *medoids,unsigned int n,char metric)
{
	unsigned int i,j,natural,*firstNatural=NULL,*secondNatural=NULL,fNatural=0,k;
	double real,*firstDouble=NULL,*secondDouble=NULL,fReal=0.0;
	Assignment *assignment;
	
	if((assignment=realloc(NULL,sizeof(Assignment)))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	if((assignment->first=realloc(NULL,n*sizeof(unsigned int)))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	if((assignment->second=realloc(NULL,n*sizeof(unsigned int)))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	if((assignment->swap=realloc(NULL,n*sizeof(unsigned int)))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	assignment->n=n;
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
			            if(firstNatural==NULL)
			            {
						    if((firstNatural=realloc(NULL,sizeof(unsigned int)))==NULL)
			                {
			                    printf("Error: System failure.\n");
			                    exit(1);
			                }
				            *firstNatural=natural;
				            assignment->first[i]=medoids->m[j];
			            }
			            else
			            {
				            if(natural<*firstNatural)
				            {
				        	    if(secondNatural==NULL)
			                    {
			                        if((secondNatural=realloc(NULL,sizeof(unsigned int)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                    }
					            *secondNatural=*firstNatural;
					            *firstNatural=natural;
					            assignment->second[i]=assignment->first[i];
			                    assignment->first[i]=medoids->m[j];
			                }
			                else
			                {
			            	    if(secondNatural==NULL)
			                    {
			                        if((secondNatural=realloc(NULL,sizeof(unsigned int)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                        *secondNatural=natural;
				                    assignment->second[i]=medoids->m[j];
			                    }
			                    else
			                    {
			                        if(natural<*secondNatural)
					                {
						                *secondNatural=natural;
						                assignment->second[i]=medoids->m[j];
			                        }
			                    }
			                }
			            }
		            }
		            free(firstNatural);
		            free(secondNatural);
		            firstNatural=NULL;
		            secondNatural=NULL;
		        }
		        if(!fNatural)
		        {
				    for(i=0;i<=n-1;i++)
		            {
		    	        fNatural+=user_hammingDistance(i+1,assignment->first[i]);
		            }
		        }
		        for(i=0;i<=medoids->k-1;i++)
		        {
		    	    for(j=0;j<=n-1;j++)
	                {
	                	if(j+1!=assignment->first[j])
	                	{
	                		for(k=0;k<=n-1;k++)
	                		{
	                			if(assignment->first[k]==medoids->m[i])
	                			{
	                				if(user_hammingDistance(k+1,j+1)<=user_hammingDistance(k+1,assignment->second[k]))
	                				{
	                					assignment->swap[k]=j+1;
	                				}
	                				else
	                				{
	                					assignment->swap[k]=assignment->second[k];
	                				}
	                			}
	                			else
	                			{
	                				if(user_hammingDistance(k+1,j+1)>=user_hammingDistance(k+1,assignment->first[k]))
	                				{
	                					assignment->swap[k]=assignment->first[k];
	                				}
	                				else
	                				{
	                					assignment->swap[k]=j+1;
	                				}
	                			}
	                		}
	                		natural=0;
	                		for(i=0;i<=n-1;i++)
		                    {
		    	                natural+=user_hammingDistance(i+1,assignment->swap[i]);
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
			            if(firstDouble==NULL)
			            {
						    if((firstDouble=realloc(NULL,sizeof(double)))==NULL)
			                {
			                    printf("Error: System failure.\n");
			                    exit(1);
			                }
				            *firstDouble=real;
				            assignment->first[i]=medoids->m[j];
			            }
			            else
			            {
				            if(real<*firstDouble)
				            {
				        	    if(secondDouble==NULL)
			                    {
			                        if((secondDouble=realloc(NULL,sizeof(double)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                    }
					            *secondDouble=*firstDouble;
					            *firstDouble=real;
					            assignment->second[i]=assignment->first[i];
			                    assignment->first[i]=medoids->m[j];
			                }
			                else
			                {
			            	    if(secondDouble==NULL)
			                    {
			                        if((secondDouble=realloc(NULL,sizeof(double)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                        *secondDouble=real;
				                    assignment->second[i]=medoids->m[j];
			                    }
			                    else
			                    {
			                        if(real<*secondDouble)
					                {
						                *secondDouble=real;
						                assignment->second[i]=medoids->m[j];
			                        }
			                    }
			                }
			            }
		            }
		            free(firstDouble);
		            free(secondDouble);
		            firstDouble=NULL;
		            secondDouble=NULL;
		        }
		        if(fReal==0.0)
		        {
				    for(i=0;i<=n-1;i++)
		            {
		    	        fReal+=user_euclideanDistance(i+1,assignment->first[i]);
		            }
		        }
		        for(i=0;i<=medoids->k-1;i++)
		        {
		    	    for(j=0;j<=n-1;j++)
	                {
	                	if(j+1!=assignment->first[j])
	                	{
	                		for(k=0;k<=n-1;k++)
	                		{
	                			if(assignment->first[k]==medoids->m[i])
	                			{
	                				if(user_euclideanDistance(k+1,j+1)<=user_euclideanDistance(k+1,assignment->second[k]))
	                				{
	                					assignment->swap[k]=j+1;
	                				}
	                				else
	                				{
	                					assignment->swap[k]=assignment->second[k];
	                				}
	                			}
	                			else
	                			{
	                				if(user_euclideanDistance(k+1,j+1)>=user_euclideanDistance(k+1,assignment->first[k]))
	                				{
	                					assignment->swap[k]=assignment->first[k];
	                				}
	                				else
	                				{
	                					assignment->swap[k]=j+1;
	                				}
	                			}
	                		}
	                		real=0.0;
	                		for(i=0;i<=n-1;i++)
		                    {
		    	                real+=user_euclideanDistance(i+1,assignment->swap[i]);
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
			            if(firstDouble==NULL)
			            {
						    if((firstDouble=realloc(NULL,sizeof(double)))==NULL)
			                {
			                    printf("Error: System failure.\n");
			                    exit(1);
			                }
				            *firstDouble=real;
				            assignment->first[i]=medoids->m[j];
			            }
			            else
			            {
				            if(real<*firstDouble)
				            {
				        	    if(secondDouble==NULL)
			                    {
			                        if((secondDouble=realloc(NULL,sizeof(double)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                    }
					            *secondDouble=*firstDouble;
					            *firstDouble=real;
					            assignment->second[i]=assignment->first[i];
			                    assignment->first[i]=medoids->m[j];
			                }
			                else
			                {
			            	    if(secondDouble==NULL)
			                    {
			                        if((secondDouble=realloc(NULL,sizeof(double)))==NULL)
			                        {
			                            printf("Error: System failure.\n");
			                            exit(1);
			                        }
			                        *secondDouble=real;
				                    assignment->second[i]=medoids->m[j];
			                    }
			                    else
			                    {
			                        if(real<*secondDouble)
					                {
						                *secondDouble=real;
						                assignment->second[i]=medoids->m[j];
			                        }
			                    }
			                }
			            }
		            }
		            free(firstDouble);
		            free(secondDouble);
		            firstDouble=NULL;
		            secondDouble=NULL;
		        }
		        if(fReal==0.0)
		        {
				    for(i=0;i<=n-1;i++)
		            {
		    	        fReal+=user_cosineDistance(i+1,assignment->first[i]);
		            }
		        }
		        for(i=0;i<=medoids->k-1;i++)
		        {
		    	    for(j=0;j<=n-1;j++)
	                {
	                	if(j+1!=assignment->first[j])
	                	{
	                		for(k=0;k<=n-1;k++)
	                		{
	                			if(assignment->first[k]==medoids->m[i])
	                			{
	                				if(user_cosineDistance(k+1,j+1)<=user_cosineDistance(k+1,assignment->second[k]))
	                				{
	                					assignment->swap[k]=j+1;
	                				}
	                				else
	                				{
	                					assignment->swap[k]=assignment->second[k];
	                				}
	                			}
	                			else
	                			{
	                				if(user_cosineDistance(k+1,j+1)>=user_cosineDistance(k+1,assignment->first[k]))
	                				{
	                					assignment->swap[k]=assignment->first[k];
	                				}
	                				else
	                				{
	                					assignment->swap[k]=j+1;
	                				}
	                			}
	                		}
	                		real=0.0;
	                		for(i=0;i<=n-1;i++)
		                    {
		    	                real+=user_cosineDistance(i+1,assignment->swap[i]);
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
	return assignment;
}

double silhouette(char metric,unsigned int n,Assignment *assignment)
{
	double *a,*b,*s,sum=0.0;
	unsigned int *aCounter,*bCounter,i,j;
	
	if((a=realloc(NULL,n*sizeof(double)))==NULL)
	{
	    printf("Error: System failure.\n");
	    exit(1);
	}
	if((b=realloc(NULL,n*sizeof(double)))==NULL)
	{
	    printf("Error: System failure.\n");
	    exit(1);
	}
	if((s=realloc(NULL,n*sizeof(double)))==NULL)
	{
	    printf("Error: System failure.\n");
	    exit(1);
	}
	if((aCounter=realloc(NULL,n*sizeof(unsigned int)))==NULL)
	{
	    printf("Error: System failure.\n");
	    exit(1);
	}
	if((bCounter=realloc(NULL,n*sizeof(unsigned int)))==NULL)
	{
	    printf("Error: System failure.\n");
	    exit(1);
	}
	for(i=0;i<=n-1;i++)
	{
	    a[i]=0.0;
	}
	for(i=0;i<=n-1;i++)
	{
	    b[i]=0.0;
	}
	for(i=0;i<=n-1;i++)
	{
	    aCounter[i]=0;
	}
	for(i=0;i<=n-1;i++)
	{
	    bCounter[i]=0;
	}
	switch(metric)
	{
		case 'h':
	        for(i=0;i<=n-1;i++)
	        {
		        for(j=0;j<=n-1;j++)
		        {
			        if(j!=i)
			        {
				        if(assignment->first[i]==assignment->first[j])
				        {
				        	a[i]+=(double)user_hammingDistance(i+1,j+1);
				        	aCounter[i]++;
				        }
				        else
				        {
				        	if(assignment->second[i]==assignment->first[j])
				            {
				        	    b[i]+=(double)user_hammingDistance(i+1,j+1);
				        	    bCounter[i]++;
				            }
				        }
				    }
				}
			}
			break;
		case 'e':
			for(i=0;i<=n-1;i++)
	        {
		        for(j=0;j<=n-1;j++)
		        {
			        if(j!=i)
			        {
				        if(assignment->first[i]==assignment->first[j])
				        {
				        	a[i]+=user_euclideanDistance(i+1,j+1);
				        	aCounter[i]++;
				        }
				        else
				        {
				        	if(assignment->second[i]==assignment->first[j])
				            {
				        	    b[i]+=user_euclideanDistance(i+1,j+1);
				        	    bCounter[i]++;
				            }
				        }
				    }
				}
			}
			break;
		case 'c':
			for(i=0;i<=n-1;i++)
	        {
		        for(j=0;j<=n-1;j++)
		        {
			        if(j!=i)
			        {
				        if(assignment->first[i]==assignment->first[j])
				        {
				        	a[i]+=user_cosineDistance(i+1,j+1);
				        	aCounter[i]++;
				        }
				        else
				        {
				        	if(assignment->second[i]==assignment->first[j])
				            {
				        	    b[i]+=user_cosineDistance(i+1,j+1);
				        	    bCounter[i]++;
				            }
				        }
				    }
				}
			}
			break;
		default:
			printf("Error: Unknown metric.\n");
			exit(1);
	}
	for(i=0;i<=n-1;i++)
	{
		a[i]/=aCounter[i];
		b[i]/=bCounter[i];
	}
	for(i=0;i<=n-1;i++)
	{
		if(a[i]>b[i])
		{
			s[i]=(b[i]-a[i])/a[i];
		}
		else
		{
			s[i]=(b[i]-a[i])/a[i];
		}
	}
	for(i=0;i<=n-1;i++)
	{
	    sum+=s[i];
	}
	free(a);
	free(b);
	free(s);
	free(aCounter);
	free(bCounter);
	return sum/n;
}
