#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc,char* argv[]) 
{
    int i=1,k=4,L=5;
	char *d=NULL,*q=NULL,*o=NULL,check[20],x[64];
	FILE *dataset,*query,*output;
	
	while(i<=argc-1) /* Read command line arguements */
	{
		if(!strcmp(argv[i],"-d"))
		{
			if(argv[i+1]!=NULL)
			{
			    if((d=malloc((strlen(argv[i+1])+1)*sizeof(char)))==NULL)
			    {
				    printf("Error: Can not allocate memory.\n");
				    printf("Press [Enter] to terminate the program.\n");
				    getc(stdin);
				    return 1;
			    }
			    strcpy(d,argv[i+1]);
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
			        if((q=malloc((strlen(argv[i+1])+1)*sizeof(char)))==NULL)
			        {
				        printf("Error: Can not allocate memory.\n");
				        printf("Press [Enter] to terminate the program.\n");
				        getc(stdin);
				        return 1;
			        }
			        strcpy(q,argv[i+1]);
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
			                    if((o=malloc((strlen(argv[i+1])+1)*sizeof(char)))==NULL)
			                    {
				                    printf("Error: Can not allocate memory.\n");
				                    printf("Press [Enter] to terminate the program.\n");
				                    getc(stdin);
				                    return 1;
			                    }
			                    strcpy(o,argv[i+1]);
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
		    printf("Error: Can not allocate memory.\n");
			printf("Press [Enter] to terminate the program.\n");
			getc(stdin);
			return 1;
		}
		printf("Enter path name of dataset file and press [Enter]: ");
		fgets(d,100,stdin);
		while(getc(stdin)!='\n')
		{
			;
		}
	}
	if(q==NULL)
	{
	    if((q=malloc(100*sizeof(char)))==NULL)
		{
		    printf("Error: Can not allocate memory.\n");
			printf("Press [Enter] to terminate the program.\n");
			getc(stdin);
			return 1;
		}
		printf("Enter path name of query file and press [Enter]: ");
		fgets(q,100,stdin);
		while(getc(stdin)!='\n')
		{
			;
		}
	}
	if(o==NULL)
	{
	    if((o=malloc(100*sizeof(char)))==NULL)
		{
		    printf("Error: Can not allocate memory.\n");
			printf("Press [Enter] to terminate the program.\n");
			getc(stdin);
			return 1;
		}
		printf("Enter path name of output file and press [Enter]: ");
		fgets(o,100,stdin);
		while(getc(stdin)!='\n')
		{
			;
		}
	}
	do
	{
	    if((dataset=fopen(d,"r"))==NULL)
	    {
		    printf("Error: Dataset file can not be opened.\n");
		    printf("Press [Enter] to terminate the program.\n");
		    getc(stdin);
		    return 1;
	    }
	    fscanf(dataset,"%s",check);
	    i=strcmp(check,"@metric_space");
		if(i)
	    {
		    printf("Error: Incorrect dataset file.\n");
		    printf("Enter path name of dataset file and press [Enter]: ");
		    fgets(d,100,stdin);
		    while(getc(stdin)!='\n')
		    {
			    ;
		    }
		}
		else
		{
		    fscanf(dataset,"%s",check);
	        if(!strcmp(check,"vector"))
	        {
	    	
	        }
	        else
	        {
			    if(!strcmp(check,"hamming"))
				{
					while(!feof(dataset))
					{
						fscanf(dataset,"%*s");
						i=0;
						do
						{
						    fscanf(dataset,"%c",&x[i]);
						    if(x[i]==' ')
						    {
						    	break;
						    }
						    i++;
						}
						while(i<=63);
					}
		        }
		        else
		        {
			        if(!strcmp(check,"matrix"))
		            {
		    	
		            }
		            else
		            {
				        if(!strcmp(check,"function"))
				        {
				        	;
				        }
				        else
				        {
						    printf("Error: Incorrect dataset file.\n");
		                    printf("Enter path name of dataset file and press [Enter]: ");
		                    fgets(d,100,stdin);
		                    while(getc(stdin)!='\n')
		                    {
			                    ;
		                    }
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
