/* SOFTWARE DEVELOPMENT FOR ALGORITHMIC PROBLEMS
   ASSIGNMENT 1
   LELEGIANNIS IOANNIS: 1115201200090
   POULIDIS NIKOLAOS: 1115200000111 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "HashGen.h"
#include "HashTable.h"
#define BUFFER_SIZE 512

int main(int argc,char* argv[]) 
{
    int i=1,k=4,L=5,error,token=0,d=0,n=0,j,tk=1,counter=0;
	char *dPath=NULL,*qPath=NULL,*oPath=NULL,bits[65],line;
	FILE *dataset,*queryFile,*outputFile;
	double *p;
	HashDescriptor *g;
	HashTable *H;
	long long int *x;
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
			fgets(dPath,100,stdin);
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

					    queryFile = fopen(qPath,"r");
						if(queryFile == NULL){
							perror("Failed to open query file");
							return -2;
						}

						outputFile = fopen(oPath,"w");
						if(outputFile == NULL){
							perror("Failed to open/create output file");
							return -3;
						}

						char buffer[BUFFER_SIZE];

						fgets(buffer,BUFFER_SIZE,queryFile);
						unsigned short radius = atoi(strtok(buffer,"Radius: \t\n"));
						double* q = malloc(sizeof(double)*d);
						if(q == NULL){
							perror("Failed to allocate memory for query vector");
							return -4;
						}

						while(fgets(buffer,BUFFER_SIZE,queryFile)){
							char* item_name=strtok(buffer," \t\n");
							for(i=0;i<d;i++) q[i] = atof(strtok(NULL," \t\n"));
							
							fputs("Query: ",outputFile);
							fputs(item_name,outputFile);
							fputc('\n',outputFile);
							fputs("R-near neighbors:\n",outputFile);

							/*calculate and time lsh*/
							clock_t begin = clock();
							EuclideanData p_eu,q_eu;
							double* b;
							double db=-1.0,temp;
							int i,j=0;
							for(i=0;i<L;i++){
								q_eu = euclidean_data_create(g[i],q);
								while(p_eu=hashTable_getNext(H[i],q_eu)){
									if(j > 3*L) break;
									if(euclidean_data_getID(q_eu)!=euclidean_data_getID(p_eu)){
										j++;
										continue;
									}
									temp = euclidean(q,euclidean_data_getVector(p_eu),d);
									if(temp < radius){
										int iter;
										for(iter=0;iter<d;iter++) fprintf(outputFile,"%f ",q[iter]);
										fputc('\n',outputFile);
									}
									if(temp < db || db < 0.0){
										b = q;
										db = temp;
									}
									j++;
								}
							}
							clock_t end = clock();
							double lsh_time = (double)(end - begin)/CLOCKS_PER_SEC;

							/*calculate and time true nearest*/
							begin = clock();
							double* true_nearest;
							double true_dist = -1.0;
							for(i=0;i<n;i++){
								temp = euclidean(p[i],q,d);
								if(temp < true_dist || true_dist < 0.0){
									true_nearest = p[i];
									true_dist = temp;
								}
							}
							end = clock();
							double true_time = (double)(end - begin)/CLOCKS_PER_SEC;

							/*output search data*/
							fputs("Nearest neighbor: ",outputFile);
							/*output nearest neighbor*/
							fputc('\n',outputFile);
							fputs("distanceLSH: ",outputFile);
							fprintf(outputFile,"%f\n",db);
							fputs("distanceTrue: ",outputFile);
							fprintf(outputFile,"%f\n",true_dist);
							fputs("tLSH: ",outputFile);
							fprintf(outputFile,"%f\n",lsh_time);
							fputs("tTrue: ",outputFile);
							fprintf(outputFile,"%f\n",true_time);
						}
				    }
				    else
				    {
				    	if(!strcmp(bits,"cosine"))
					    {
						    for(i=1;i<=k;i++)
						    {
						    	tk*=2;
						    }
						    for(i=0;i<=L-1;i++)
						    {
							    do
							    {
								    if((g[i]=cosine_hash_create(d,k))==NULL)
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

					        queryFile = fopen(qPath,"r");
							if(queryFile == NULL){
								perror("Failed to open query file");
								return -2;
							}

							outputFile = fopen(oPath,"w");
							if(outputFile == NULL){
								perror("Failed to open/create output file");
								return -3;
							}

							char buffer[BUFFER_SIZE];

							fgets(buffer,BUFFER_SIZE,queryFile);
							unsigned short radius = atoi(strtok(buffer,"Radius: \t\n"));
							double* q = malloc(sizeof(double)*d);
							if(q == NULL){
								perror("Failed to allocate memory for query vector");
								return -4;
							}

							while(fgets(buffer,BUFFER_SIZE,queryFile)){
								char* item_name=strtok(buffer," \t\n");
								for(i=0;i<d;i++) q[i] = atof(strtok(NULL," \t\n"));
								
								fputs("Query: ",outputFile);
								fputs(item_name,outputFile);
								fputc('\n',outputFile);
								fputs("R-near neighbors:\n",outputFile);

								/*calculate and time lsh*/
								clock_t begin = clock();
								double* p_current;
								double* b;
								double db=-1.0,temp;
								int i,j=0;
								for(i=0;i<L;i++){
									while(p_current=hashTable_getNext(H[i],q)){
										if(j > 3*L) break;
										temp = cosine(p_current,q,d);
										if(temp < radius){
											int iter;
											for(iter=0;iter<d;iter++) fprintf(outputFile,"%f ",q[iter]);
											fputc('\n',outputFile);
										}
										if(temp < db || db < 0.0){
											b = q;
											db = temp;
										}
										j++;
									}
								}
								clock_t end = clock();
								double lsh_time = (double)(end - begin)/CLOCKS_PER_SEC;

								/*calculate and time true nearest*/
								begin = clock();
								double* true_nearest;
								double true_dist = -1.0;
								for(i=0;i<n;i++){
									temp = cosine(p[i],q,d);
									if(temp < true_dist || true_dist < 0.0){
										true_nearest = p[i];
										true_dist = temp;
									}
								}
								end = clock();
								double true_time = (double)(end - begin)/CLOCKS_PER_SEC;

								/*output search data*/
								fputs("Nearest neighbor: ",outputFile);
								/*output nearest neighbor*/
								fputc('\n',outputFile);
								fputs("distanceLSH: ",outputFile);
								fprintf(outputFile,"%f\n",db);
								fputs("distanceTrue: ",outputFile);
								fprintf(outputFile,"%f\n",true_dist);
								fputs("tLSH: ",outputFile);
								fprintf(outputFile,"%f\n",lsh_time);
								fputs("tTrue: ",outputFile);
								fprintf(outputFile,"%f\n",true_time);
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
					if(fgetpos(dataset,&pos))
					{
						printf("Error: Function failure.\n");
		                printf("Press [Enter] to terminate the program.\n");
		                getc(stdin);
		                return 1;
				    }
				    while(!feof(dataset))
				    {
				    	fscanf(dataset,"%*s");
				    	fscanf(dataset,"%*s");
				    	line=getc(dataset);
					    if((line=='\n')||(line==EOF))
						{
							n++;
						}
					}
					if((x=malloc(n*sizeof(unsigned int)))==NULL)
				    {
		                printf("Error: Failed to allocate memory.\n");
			            printf("Press [Enter] to terminate the program.\n");
			            getc(stdin);
			            return 1;
		            }
					if(fsetpos(dataset,&pos))
					{
						printf("Error: Function failure.\n");
		                printf("Press [Enter] to terminate the program.\n");
		                getc(stdin);
		                return 1;
				    }
				    while(!feof(dataset))
				    {
					    fscanf(dataset,"%*s");
						fscanf(dataset,"%64s",bits);
						if(!d)
						{
						    d=strlen(bits);
						    for(i=1;i<=k;i++)
						    {
						    	tk*=2;
						    }
						    for(i=0;i<=L-1;i++)
					        {
						        do
						        {
							        if((g[i]=hamming_hash_create(d,k))==NULL)
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
						x[counter]=strtoll(bits,NULL,2);
						for(i=0;i<=L-1;i++)
					    {
					        hashTable_insert(H[i],&x[counter]);
					    }
					    counter++;
					}

					queryFile = fopen(qPath,"r");
					if(queryFile == NULL){
						perror("Failed to open query file");
						return -2;
					}

					outputFile = fopen(oPath,"w");
					if(outputFile == NULL){
						perror("Failed to open/create output file");
						return -3;
					}

					char buffer[BUFFER_SIZE];

					fgets(buffer,BUFFER_SIZE,queryFile);
					unsigned short hamming_radius = atoi(strtok(buffer,"Radius: \t\n"));
					long long int q;

					while(fgets(buffer,BUFFER_SIZE,queryFile)){
						char* item_name=strtok(buffer," \t\n");
						q = strtoll(strtok(NULL," \t\n"),NULL,2);
						
						fputs("Query: ",outputFile);
						fputs(item_name,outputFile);
						fputc('\n',outputFile);
						fputs("R-near neighbors:\n",outputFile);

						/*calculate and time lsh*/
						clock_t begin = clock();
						long long int* p;
						long long int b;
						int i,j=0,db=-1,temp;
						for(i=0;i<L;i++){
							while(p=hashTable_getNext(H[i],&q)){
								if(j > 3*L) break;
								temp = hamming(*p,q);
								if(temp < hamming_radius){
									long long int temp2 = *p;
									while(temp2){
										if(temp2 & 1) fputc('1',outputFile);
										else fputc('0',outputFile);
										temp2 >>= 1;
									}
									fputc('\n',outputFile);
								}
								if(temp < db || db == -1){
									b = *p;
									db = temp;
								}
								j++;
							}
						}
						clock_t end = clock();
						double lsh_time = (double)(end - begin)/CLOCKS_PER_SEC;

						/*calculate and time true nearest*/
						begin = clock();
						long long int true_nearest;
						int true_dist = -1;
						for(i=0;i<n;i++){
							temp = hamming(x[i],q);
							if(temp < true_dist || true_dist == -1){
								true_nearest = x[i];
								true_dist = temp;
							}
						}
						end = clock();
						double true_time = (double)(end - begin)/CLOCKS_PER_SEC;

						/*output search data*/
						fputs("Nearest neighbor: ",outputFile);
						/*output nearest neighbor*/
						fputc('\n',outputFile);
						fputs("distanceLSH: ",outputFile);
						fprintf(outputFile,"%d\n",db);
						fputs("distanceTrue: ",outputFile);
						fprintf(outputFile,"%d\n",true_dist);
						fputs("tLSH: ",outputFile);
						fprintf(outputFile,"%f\n",lsh_time);
						fputs("tTrue: ",outputFile);
						fprintf(outputFile,"%f\n",true_time);
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

					        queryFile = fopen(qPath,"r");
							if(queryFile == NULL){
								perror("Failed to open query file");
								return -2;
							}

							outputFile = fopen(oPath,"w");
							if(outputFile == NULL){
								perror("Failed to open/create output file");
								return -3;
							}

							char buffer[BUFFER_SIZE];

							fgets(buffer,BUFFER_SIZE,queryFile);
							unsigned short radius = atoi(strtok(buffer,"Radius: \t\n"));
							unsigned int* q = malloc(sizeof(double)*n);
							if(q == NULL){
								perror("Failed to allocate memory for query vector");
								return -4;
							}

							while(fgets(buffer,BUFFER_SIZE,queryFile)){
								char* item_name=strtok(buffer," \t\n");
								for(i=0;i<d;i++) q[i] = (unsigned int)(atoi(strtok(NULL," \t\n")));
								
								fputs("Query: ",outputFile);
								fputs(item_name,outputFile);
								fputc('\n',outputFile);
								fputs("R-near neighbors:\n",outputFile);

								/*calculate and time lsh*/
								clock_t begin = clock();
								unsigned int* current;
								unsigned int temp,b,db;
								int i,j=0;
								for(i=0;i<L;i++){
									while(current=hashTable_getNext(H[i],q)){
										if(j > 3*L) break;
										temp = q[*current];
										if(temp < radius){
											int iter;
											for(iter=0;iter<d;iter++) fprintf(outputFile,"%u ",a[iter][*current]);
											fputc('\n',outputFile);
										}
										if(temp < db || db < 0.0){
											b = *current;
											db = temp;
										}
										j++;
									}
								}
								clock_t end = clock();
								double lsh_time = (double)(end - begin)/CLOCKS_PER_SEC;

								/*calculate and time true nearest*/
								begin = clock();
								unsigned int* true_nearest;
								unsigned int true_dist = -1;
								for(i=0;i<n;i++){
									temp = q[i];
									if(temp < true_dist || true_dist < 0){
										true_nearest = a[i];
										true_dist = temp;
									}
								}
								end = clock();
								double true_time = (double)(end - begin)/CLOCKS_PER_SEC;

								/*output search data*/
								fputs("Nearest neighbor: ",outputFile);
								for(i=0;i<n;i++) fprintf(outputFile,"%u ",true_nearest[i]);
								fputc('\n',outputFile);
								fputs("distanceLSH: ",outputFile);
								fprintf(outputFile,"%u\n",db);
								fputs("distanceTrue: ",outputFile);
								fprintf(outputFile,"%u\n",true_dist);
								fputs("tLSH: ",outputFile);
								fprintf(outputFile,"%f\n",lsh_time);
								fputs("tTrue: ",outputFile);
								fprintf(outputFile,"%f\n",true_time);
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
		            /*else
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
				    }*/
				}
			}
		}
	}
	while(error);
	return 0;
}
