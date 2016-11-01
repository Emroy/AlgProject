/* SOFTWARE DEVELOPMENT FOR ALGORITHMIC PROBLEMS
   ASSIGNMENT 1
   LELEGIANNIS IOANNIS: 1115201200090
   POULIDIS NIKOLAOS: 1115200000111 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char* argv[]) 
{
    int i=1,k=4,L=5,error,token=0,d=0,n=1,j;
	char *dPath=NULL,*qPath=NULL,*oPath=NULL,bits[65],line;
	FILE *dataset,*query,*output;
	double *p;
	HashDescriptor *g;
	long long int x;
	fpos_t pos;
	
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
		gets(dPath);
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
		gets(qPath);
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
		gets(oPath);
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
			gets(dPath);
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
					    fscanf(dataset,"%*lf");
					    d++;
					}
					while((line=getc(dataset))!='\n');
					while(!feof(dataset))
					{
					    if((line=getc(dataset))=='\n')
						{
							n++;
						}
					}
					if(fgetpos(dataset,&pos))
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
					}
				}
			    else
			    {
				    error=1;
				    printf("Error: Incorrect dataset file.\n");
		            printf("Enter path name of dataset file and press [Enter]: ");
				    gets(dPath);
			    }
		    }
	        else
	        {
			    if(!strcmp(bits,"hamming"))
				{
				    {
					    fscanf(dataset,"%*s");
						fscanf(dataset,"%64s",bits);
						if(!d)
						{
						    d=strlen(bits);
						    if((g=mallloc(L*sizeof(HashDescriptor)))==NULL)
						    {
							    printf("Error: Failed to allocate memory.\n");
			                    printf("Press [Enter] to terminate the program.\n");
			                    getc(stdin);
			                    return 1;
		                    }
						    for(i=0;i<=L-1;i++)
						    {
						    	do
						    	{
								    g[i]=hamming_hash_create(k,d);
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
							}
						}
						x=strtoll(bits,NULL,2);
						for(i=0;i<=L-1;i++)
						{
							
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
							do
							{
							    fscanf(dataset,"%64s",bits);
							}
							while(strcmp(bits,"0"));
							do
							{
							    fscanf(dataset,"%64s",bits);
							}
							while(!strcmp(bits,"0"));
						}
						else
				        {
						    error=1;
						    printf("Error: Incorrect dataset file.\n");
		                    printf("Enter path name of dataset file and press [Enter]: ");
							gets(dPath);
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
							gets(dPath);
				        }
				    }
				}
			}
		}
	}
	while(error);
	return 0;
}
