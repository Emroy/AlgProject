/* SOFTWARE DEVELOPMENT FOR ALGORITHMIC PROBLEMS
   ASSIGNMENT 1
   LELEGIANNIS IOANNIS: 1115201200090
   POULIDIS NIKOLAOS: 1115200000111 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char* argv[]) 
{
    int i=1,k=4,L=5,error,token=0,d=0,n=0,j,tk;
	char *dPath=NULL,*qPath=NULL,*oPath=NULL,bits[65],line;
	FILE *dataset,*query,*output;
	double *p;
	HashDescriptor *g;
	HashTable *H;
	long long int x;
	fpos_t pos;
	unsigned int **a,*b;
	
	while(i<=argc-1)
	{
		if(!strcmp(argv[i],"-d"))
		{
			if(argv[i+1]!=NULL)
			{
			    dPath=argv[i+1];
			    i+=2;
			}
			else
			{
				break;
			}
		}
		else
		{
		    if(!strcmp(argv[i],"-q"))
			{
			    if(argv[i+1]!=NULL)
			    {
				    qPath=argv[i+1];
			        i+=2;
			    }
			    else
				{
				    break;
			    }
			}
		    else
			{
			    if(!strcmp(argv[i],"-k"))
				{
					if(argv[i+1]!=NULL)
					{
					    k=atoi(argv[i+1]);
				        i+=2;
				    }
					else
			        {
				        break;
			        }
				}
				else
				{
				    if(!strcmp(argv[i],"-L"))
					{
						if(argv[i+1]!=NULL)
						{
					        L=atoi(argv[i+1]);
					        i+=2;
					    }
					    else
			            {
				            break;
			            }
		            }
		            else
					{
					    if(!strcmp(argv[i],"-o"))
						{
						    if(argv[i+1]!=NULL)
			                {
							    oPath=argv[i+1];
			                    i+=2;
			                }
			                else
			                {
				                break;
			                }
		                }
		                else
		                {
		                	i++;
		                }
					}
				}
			}
		}
	}
	if(dPath==NULL)
	{
	    if((dPath=malloc(100*sizeof(char)))==NULL)
		{
		    printf("Error: Failed to allocate memory.\n");
			printf("Press [Enter] to terminate the program.\n");
			getc(stdin);
			return 1;
		}
		printf("Enter path name of dataset file and press [Enter]: ");
		fgets(dPath,100,stdin);
	}
	if(qPath==NULL)
	{
	    if((qPath=malloc(100*sizeof(char)))==NULL)
		{
		    printf("Error: Failed to allocate memory.\n");
			printf("Press [Enter] to terminate the program.\n");
			getc(stdin);
			return 1;
		}
		printf("Enter path name of query file and press [Enter]: ");
		fgets(qPath,100,stdin);
	}
	if(oPath==NULL)
	{
	    if((oPath=malloc(100*sizeof(char)))==NULL)
		{
		    printf("Error: Failed to allocate memory.\n");
			printf("Press [Enter] to terminate the program.\n");
			getc(stdin);
			return 1;
		}
		printf("Enter path name of output file and press [Enter]: ");
		fgets(oPath,100,stdin);
	}
	if((g=malloc(L*sizeof(HashDescriptor)))==NULL)
    {
		printf("Error: Failed to allocate memory.\n");
	    printf("Press [Enter] to terminate the program.\n");
	    getc(stdin);
	    return 1;
	}
	if((H=malloc(L*sizeof(HashTable)))==NULL)
    {
		printf("Error: Failed to allocate memory.\n");
	    printf("Press [Enter] to terminate the program.\n");
	    getc(stdin);
	    return 1;
	}
	do
	{
	    if((dataset=fopen(dPath,"r"))==NULL)
	    {
		    printf("Error: Failed to open dataset file.\n");
		    printf("Press [Enter] to terminate the program.\n");
		    getc(stdin);
		    return 1;
	    }
	    fscanf(dataset,"%64s",bits);
		error=strcmp(bits,"@metric_space");
		if(error)
	    {
		    printf("Error: Incorrect dataset file.\n");
		    printf("Enter path name of dataset file and press [Enter]: ");
			fgets(dPath,100,stdio);
		}
		else
		{
		    fscanf(dataset,"%64s",bits);
	        if(!strcmp(bits,"vector"))
	        {
			    fscanf(dataset,"%64s",bits);
	        	if(!strcmp(bits,"@metric"))
	        	{
				    fscanf(dataset,"%64s",bits);
					if(fgetpos(dataset,&pos))
					{
						printf("Error: Function failure.\n");
		                printf("Press [Enter] to terminate the program.\n");
		                getc(stdin);
		                return 1;
				    }
					fscanf(dataset,"%*s");
					do
					{
					    fscanf(dataset,"%*f");
					    line=getc(dataset);
					    d++;
					}
					while((line!='\n')&&(line!=EOF));
					n++;
					while(!feof(dataset))
					{
						line=getc(dataset);
					    if((line=='\n')||(line==EOF))
						{
							n++;
						}
					}
					if(!strcmp(bits,"euclidean"))
					{
						for(i=0;i<=L-1;i++)
						{
							do
							{
								if((g[i]=euclidean_hash_create(d,k,n))==NULL)
								{
									printf("Error: Function failure.\n");
									printf("Press [Enter] to terminate the program.\n");
									getc(stdin);
		                            return 1;
				                }
							    for(j=0;j<=i-1;j++)
							    {
								    token=euclidean_is_equal(g[i],g[j]);
							        if(token)
							        {
							            break;
							        }
							    }
					        }
					        while(token);
					        if((H[i]=hashTable_create(n/2,g[i]))==NULL)
					        {
						        printf("Error: Function failure.\n");
		                        printf("Press [Enter] to terminate the program.\n");
		                        getc(stdin);
		                        return 1;
				            }
					    }
					    if(fsetpos(dataset,&pos))
					    {
						    printf("Error: Function failure.\n");
		                    printf("Press [Enter] to terminate the program.\n");
		                    getc(stdin);
		                    return 1;
				        }
				        if((p=malloc(d*sizeof(double)))==NULL)
		                {
		                    printf("Error: Failed to allocate memory.\n");
			                printf("Press [Enter] to terminate the program.\n");
			                getc(stdin);
			                return 1;
		                }
					    while(!feof(dataset))
					    {
						    fscanf(dataset,"%*s");
						    for(i=0;i<=d-1;i++)
					        {
					            fscanf(dataset,"%lf",&p[i]);
					        }
					        for(i=0;i<=L-1;i++)
					        {
					            hashTable_insert(H[i],euclidean_data_create(g[i],p));
					        }
					    }
				    }
				    else
				    {
				    	if(!strcmp(bits,"cosine"))
					    {
						    tk=1;
						    for(i=1;i<=k;i++)
						    {
						    	tk*=2;
						    }
						    for(i=0;i<=L-1;i++)
						    {
							    do
							    {
								    if((g[i]=cosine_hash_create(d,k,n))==NULL)
								    {
									    printf("Error: Function failure.\n");
									    printf("Press [Enter] to terminate the program.\n");
									    getc(stdin);
		                                return 1;
				                    }
							        for(j=0;j<=i-1;j++)
							        {
								        token=cosine_is_equal(g[i],g[j]);
							            if(token)
							            {
							                break;
							            }
							        }
					            }
					            while(token);
					            if((H[i]=hashTable_create(tk,g[i]))==NULL)
					            {
						            printf("Error: Function failure.\n");
		                            printf("Press [Enter] to terminate the program.\n");
		                            getc(stdin);
		                            return 1;
				                }
					        }
					        if(fsetpos(dataset,&pos))
					        {
						        printf("Error: Function failure.\n");
		                        printf("Press [Enter] to terminate the program.\n");
		                        getc(stdin);
		                        return 1;
				            }
				            if((p=malloc(d*sizeof(double)))==NULL)
		                    {
		                        printf("Error: Failed to allocate memory.\n");
			                    printf("Press [Enter] to terminate the program.\n");
			                    getc(stdin);
			                    return 1;
		                    }
					        while(!feof(dataset))
					        {
						        fscanf(dataset,"%*s");
						        for(i=0;i<=d-1;i++)
					            {
					                fscanf(dataset,"%lf",&p[i]);
					            }
					            for(i=0;i<=L-1;i++)
					            {
					                hashTable_insert(H[i],p);
					            }
					        }
				        }
				    }
				}
			    else
			    {
				    if(fgetpos(dataset,&pos))
					{
						printf("Error: Function failure.\n");
		                printf("Press [Enter] to terminate the program.\n");
		                getc(stdin);
		                return 1;
				    }
					do
					{
					    fscanf(dataset,"%*f");
					    line=getc(dataset);
					    d++;
					}
					while((line!='\n')&&(line!=EOF));
					n++;
					while(!feof(dataset))
					{
						line=getc(dataset);
					    if((line=='\n')||(line==EOF))
						{
							n++;
						}
					}
					for(i=0;i<=L-1;i++)
					{
						do
						{
							if((g[i]=euclidean_hash_create(d,k,n))==NULL)
							{
								printf("Error: Function failure.\n");
								printf("Press [Enter] to terminate the program.\n");
								getc(stdin);
		                        return 1;
				            }
							for(j=0;j<=i-1;j++)
							{
								token=euclidean_is_equal(g[i],g[j]);
							    if(token)
							    {
							        break;
							    }
							}
					    }
					    while(token);
					    if((H[i]=hashTable_create(n/2,g[i]))==NULL)
					    {
						    printf("Error: Function failure.\n");
		                    printf("Press [Enter] to terminate the program.\n");
		                    getc(stdin);
		                    return 1;
				        }
					}
					if(fsetpos(dataset,&pos))
					{
						printf("Error: Function failure.\n");
		                printf("Press [Enter] to terminate the program.\n");
		                getc(stdin);
		                return 1;
				    }
				    if((p=malloc(d*sizeof(double)))==NULL)
		            {
		                printf("Error: Failed to allocate memory.\n");
			            printf("Press [Enter] to terminate the program.\n");
			            getc(stdin);
			            return 1;
		            }
		            for(i=0;i<=d-1;i++)
					{
					    fscanf(dataset,"%lf",&p[i]);
					}
					for(i=0;i<=L-1;i++)
					{
					    hashTable_insert(H[i],p);
					}
					while(!feof(dataset))
					{
						fscanf(dataset,"%*s");
						for(i=0;i<=d-1;i++)
					    {
					        fscanf(dataset,"%lf",&p[i]);
					    }
					    for(i=0;i<=L-1;i++)
					    {
					        hashTable_insert(H[i],p);
					    }
					}
			    }
		    }
	        else
	        {
			    if(!strcmp(bits,"hamming"))
				{
				    while(!feof(dataset))
				    {
					    fscanf(dataset,"%*s");
						fscanf(dataset,"%64s",bits);
						if(!d)
						{
						    d=strlen(bits);
						    tk=1;
						    for(i=1;i<=k;i++)
						    {
						    	tk*=2;
						    }
						    for(i=0;i<=L-1;i++)
					        {
						        do
						        {
							        if((g[i]=hamming_hash_create(d,k,n))==NULL)
							        {
								        printf("Error: Function failure.\n");
								        printf("Press [Enter] to terminate the program.\n");
								        getc(stdin);
		                                return 1;
				                    }
							        for(j=0;j<=i-1;j++)
							        {
								        token=hamming_is_equal(g[i],g[j]);
							            if(token)
							            {
							                break;
							            }
							        }
					            }
					            while(token);
					            if((H[i]=hashTable_create(tk,g[i]))==NULL)
					            {
						            printf("Error: Function failure.\n");
		                            printf("Press [Enter] to terminate the program.\n");
		                            getc(stdin);
		                            return 1;
				                }
					        }
						}
						x=strtoll(bits,NULL,2);
						for(i=0;i<=L-1;i++)
					    {
					        hashTable_insert(H[i],&x);
					    }
					}
		        }
		        else
		        {
			        if(!strcmp(bits,"matrix"))
		            {
					    fscanf(dataset,"%64s",bits);
						if(!strcmp(bits,"@items"))
						{
						    fscanf(dataset,"%*s");
						    if(fgetpos(dataset,&pos))
					        {
						        printf("Error: Function failure.\n");
		                        printf("Press [Enter] to terminate the program.\n");
		                        getc(stdin);
		                        return 1;
				            }
							while(!feof(dataset))
				            {
							    do
							    {
							        fscanf(dataset,"%*u");
							        line=getc(dataset);
							        n++;
							    }
							    while((line!='\n')&&(line!=EOF));
							}
							if(fsetpos(dataset,&pos))
					        {
						        printf("Error: Function failure.\n");
		                        printf("Press [Enter] to terminate the program.\n");
		                        getc(stdin);
		                        return 1;
				            }
				            if((a=malloc(n*sizeof(unsigned int*)))==NULL)
				            {
							    printf("Error: Failed to allocate memory.\n");
			                    printf("Press [Enter] to terminate the program.\n");
			                    getc(stdin);
			                    return 1;
		                    }
		                    for(i=0;i<=n-1;i++)
		                    {
							    if((a[i]=malloc(n*sizeof(unsigned int)))==NULL)
				                {
							        printf("Error: Failed to allocate memory.\n");
			                        printf("Press [Enter] to terminate the program.\n");
			                        getc(stdin);
			                        return 1;
		                        }
		                    }
		                    for(i=0;i<=n-1;i++)
		                    {
		                    	for(j=0;j<=n-1;j++)
		                    	{
		                    		fscanf(dataset,"%u",&a[i][j]);
		                    	}
		                    }
						    tk=1;
						    for(i=1;i<=k;i++)
						    {
						        tk*=2;
						    }
						    for(i=0;i<=L-1;i++)
					        {
						        do
						        {
							        if((g[i]=matrix_hash_create(k,a,n))==NULL)
							        {
								        printf("Error: Function failure.\n");
								        printf("Press [Enter] to terminate the program.\n");
								        getc(stdin);
		                                return 1;
				                    }
							        for(j=0;j<=i-1;j++)
							        {
								        token=matrix_is_equal(g[i],g[j]);
							            if(token)
							            {
							                break;
							            }
							        }
					            }
					            while(token);
					            if((H[i]=hashTable_create(tk,g[i]))==NULL)
					            {
						            printf("Error: Function failure.\n");
		                            printf("Press [Enter] to terminate the program.\n");
		                            getc(stdin);
		                            return 1;
				                }
					        }
					        if((b=malloc(n*sizeof(unsigned int)))==NULL)
					        {
							    printf("Error: Failed to allocate memory.\n");
			                    printf("Press [Enter] to terminate the program.\n");
			                    getc(stdin);
			                    return 1;
		                    }
		                    for(i=0;i<=n-1;i++)
		                    {
		                    	b[i]=i;
		                    }
					        for(i=0;i<=L-1;i++)
					        {
					    	    for(j=0;j<=n-1;j++)
					    	    {
							        hashTable_insert(H[i],&b[j]);
							    }
					        }
					    }
						else
				        {
						    error=1;
						    printf("Error: Incorrect dataset file.\n");
		                    printf("Enter path name of dataset file and press [Enter]: ");
							fgets(dPath,100,stdin);
				        }
		            }
		            else
		            {
				        if(!strcmp(bits,"function"))
				        {
				        	;
				        }
				        else
				        {
				        	error=1;
						    printf("Error: Incorrect dataset file.\n");
		                    printf("Enter path name of dataset file and press [Enter]: ");
							fgets(dPath,100,stdin);
				        }
				    }
				}
			}
		}
	}
	while(error);
	return 0;
}
