/* амаптунг коцислийоу циа акцояихлийа пяобкглата
   еяцасиа 1
   кекециаммгс иыаммгс: 1115201200090
   поукидгс мийокаос: 1115200000111 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char* argv[]) 
{
    int i=1,k=4,L=5,s=0,j,k=0;
	char *d=NULL,*q=NULL,*o=NULL,bits[65];
	FILE *dataset,*query,*output;
	HashDescriptor *g;
	long long int x;
	
	while(i<=argc-1)
	{
		if(!strcmp(argv[i],"-d"))
		{
			if(argv[i+1]!=NULL)
			{
			    d=argv[i+1];
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
				    q=argv[i+1];
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
							    o=argv[i+1];
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
	if(d==NULL)
	{
	    if((d=malloc(100*sizeof(char)))==NULL)
		{
		    printf("Error: Failed to allocate memory.\n");
			printf("Press [Enter] to terminate the program.\n");
			getc(stdin);
			return 1;
		}
		printf("Enter path name of dataset file and press [Enter]: ");
		gets(d);
	}
	if(q==NULL)
	{
	    if((q=malloc(100*sizeof(char)))==NULL)
		{
		    printf("Error: Failed to allocate memory.\n");
			printf("Press [Enter] to terminate the program.\n");
			getc(stdin);
			return 1;
		}
		printf("Enter path name of query file and press [Enter]: ");
		gets(q);
	}
	if(o==NULL)
	{
	    if((o=malloc(100*sizeof(char)))==NULL)
		{
		    printf("Error: Failed to allocate memory.\n");
			printf("Press [Enter] to terminate the program.\n");
			getc(stdin);
			return 1;
		}
		printf("Enter path name of output file and press [Enter]: ");
		gets(o);
	}
	do
	{
	    if((dataset=fopen(d,"r"))==NULL)
	    {
		    printf("Error: Failed to open dataset file.\n");
		    printf("Press [Enter] to terminate the program.\n");
		    getc(stdin);
		    return 1;
	    }
	    fscanf(dataset,"%64s",bits);
	    i=strcmp(bits,"@metric_space");
		if(i)
	    {
		    printf("Error: Incorrect dataset file.\n");
		    printf("Enter path name of dataset file and press [Enter]: ");
			gets(d);
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
				    if(!strcmp(bits,"manhattan"))
					{
						;
					}
					else
					{
					    if(!strcmp(bits,"cosine"))
					    {
						    ;	
					    }
					    else
					    {
					    	;
					    }
					}
				}
				else
				{
				    printf("Error: Incorrect dataset file.\n");
		            printf("Enter path name of dataset file and press [Enter]: ");
					gets(d);
		            i=1;
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
						if(!s)
						{
						    s=strlen(bits);
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
								    g[i]=hamming_hash_create(k,s);
							        for(j=0;j<=i-1;j++)
							        {
							        	k=hamming_is_equal(g[i],g[j]);
							        	if(k)
							        	{
							        		break;
							        	}
							        }
							    }
							    while(k);
							}
						}
						x=strtoll(bits,NULL,2);
						for(i=0;i<=L-1;i++)
						{
						    hash_apply(g[i],&x);
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
						    printf("Error: Incorrect dataset file.\n");
		                    printf("Enter path name of dataset file and press [Enter]: ");
							gets(d);
				        	i=1;
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
						    printf("Error: Incorrect dataset file.\n");
		                    printf("Enter path name of dataset file and press [Enter]: ");
							gets(d);
				        	i=1;
				        }
				    }
				}
			}
		}
	}
	while(i);
	return 0;
}
