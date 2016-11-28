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
#include "data.h"
#define BUFFER_SIZE 512
#define PATH_BUFFER_SIZE 100

int main(int argc,char* argv[]) 
{
    int i=1,k=4,L=5,error,token=0,d=0,n,j,tk=1;
	char *dPath=NULL,*qPath=NULL,*oPath=NULL,symbols[65],*line;
	FILE *inputFile,*queryFile,*outputFile;
	List list;
    Data data,*dataP;
	HashDescriptor *g;
	HashTable *H;
	unsigned int **x;
	
	/*Read command line parameters*/
	while(i<=argc-1)
	{
		if(!strcmp(argv[i],"-d"))
		{
			if(argv[i+1]!=NULL)
			{
			    dPath=argv[i+1];
			    i+=2;
			}
			else break;
		}
		else if(!strcmp(argv[i],"-q"))
		{
		    if(argv[i+1]!=NULL)
		    {
			    qPath=argv[i+1];
		        i+=2;
		    }
		    else break;
		}
	    else if(!strcmp(argv[i],"-k"))
		{
			if(argv[i+1]!=NULL)
			{
			    k=atoi(argv[i+1]);
		        i+=2;
		    }
			else break;
		}
		else if(!strcmp(argv[i],"-L"))
		{
			if(argv[i+1]!=NULL)
			{
		        L=atoi(argv[i+1]);
		        i+=2;
		    }
		    else break;
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
                else break;
            }
            else i++;
		}
	}

	/*Ask for file paths if not given from command line*/
	if(dPath==NULL)
	{
	    if((dPath=malloc(PATH_BUFFER_SIZE*sizeof(char)))==NULL)
		{
		    perror("Failed to allocate memory for dataset file path");
			return 1;
		}
		printf("Enter path name of dataset file and press [Enter]: ");
		fgets(dPath,PATH_BUFFER_SIZE,stdin);
	}
	if(qPath==NULL)
	{
	    if((qPath=malloc(PATH_BUFFER_SIZE*sizeof(char)))==NULL)
		{
		    perror("Failed to allocate memory for query file path");
			return 1;
		}
		printf("Enter path name of query file and press [Enter]: ");
		fgets(qPath,PATH_BUFFER_SIZE,stdin);
	}
	if(oPath==NULL)
	{
	    if((oPath=malloc(PATH_BUFFER_SIZE*sizeof(char)))==NULL)
		{
		    perror("Failed to allocate memory for output file path");
			return 1;
		}
		printf("Enter path name of output file and press [Enter]: ");
		fgets(oPath,PATH_BUFFER_SIZE,stdin);
	}

	/*Allocate Memory for hash function descriptors and hash tables*/
	if((g=malloc(L*sizeof(HashDescriptor)))==NULL)
    {
		perror("Failed to allocate memory for hash descriptors in main");
	    return 1;
	}
	if((H=malloc(L*sizeof(HashTable)))==NULL)
    {
    	perror("Failed to allocate memory for hash tables in main");
	    return 1;
	}

	do
	{
	    if((inputFile=fopen(dPath,"r"))==NULL)
	    {
		    perror("Failed to open input file");
		    error=1;
			exit(1);
	    }
	    fscanf(inputFile,"%64s",symbols);
		error=strcmp(symbols,"@metric_space");
		if(error)
	    {
		    printf("Error: Incorrect input file.\n");
		    printf("Enter path name of input file and press [Enter]: ");
			fgets(dPath,PATH_BUFFER_SIZE,stdin);
		}
		else
		{
		    fscanf(inputFile,"%64s",symbols);
	        if(!strcmp(symbols,"vector"))
	        {
			    fscanf(inputFile,"%64s",symbols);
	        	if(!strcmp(symbols,"@metric"))
	        	{
				    fscanf(inputFile,"%64s",symbols);
					if(!strcmp(symbols,"euclidean"))
					{
					    if((list=list_create())==NULL)
		    		    {
		    			    printf("Error: System failure.\n");
		    			    exit(1);
		    		    }
				        while(!feof(inputFile))
			            {
						    line=NULL;
				            i=0;
				            do
				            {
						        fscanf(inputFile,"%64s",symbols);
				                n=strlen(symbols);
				                if((line=realloc(line,(i+n+1)*sizeof(char)))==NULL)
				                {
				                    printf("Error: System failure.\n");
					                exit(1);
				                }
				                if(!token)
				                {
				                	d++;
				                }
				                strcpy(line+i,symbols);
				                i+=n+1;
				                line[i-1]= ;
				            }
				            while(getc(inputFile)!='\n');
				            line[i-1]='\0';
				            data=euclidean_data_create(line);
				            list_pushEnd(list,data);
				            free(line);
				            if(!token)
				            {
				            	d--;
				            	token=1;
				            }
				        }
				        n=list_length(list);
				        if((dataP=realloc(NULL,n*sizeof(Data)))==NULL)
				        {
				    	    printf("Error: System failure.\n");
						    exit(1);
				        }
				        i=0;
				        while(!list_isEmpty(list))
				        {
					        dataP[i]=list_pop(list);
				    	    i++;
				        }
				        list_destroy(list);
						for(i=0;i<=L-1;i++)
						{
							do
							{
								if((g[i]=euclidean_hash_create(d,k,n))==NULL)
								{
									fprintf(stderr,"Could not create euclidean hash descriptor.\n");
		                            return 1;
				                }
							    for(j=0;j<=i-1;j++)
							    {
								    token=euclidean_is_equal(g[i],g[j]);
							        if(token) break;
							    }
					        }
					        while(token);
					        if((H[i]=hashTable_create(n/2,g[i]))==NULL)
					        {
					        	fprintf(stderr,"Could not create hash table.\n");
		                        return 1;
				            }
					    }
					    for(i=0;i<=L-1;i++)
					    {
						    for(j=0;j<=n-1;j++)
						    {
							    hashTable_insert(H[i],dataP[j]);
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
							for(i=0;i<d;i++) fprintf(outputFile,"%f ",true_nearest[i]);
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
				    	if(!strcmp(symbols,"cosine"))
					    {
					    	if((list=list_create())==NULL)
		    		        {
		    			        printf("Error: System failure.\n");
		    			        exit(1);
		    		        }
				            while(!feof(inputFile))
			                {
						        line=NULL;
				                i=0;
				                do
				                {
						            fscanf(inputFile,"%64s",symbols);
				                    n=strlen(symbols);
				                    if((line=realloc(line,(i+n+1)*sizeof(char)))==NULL)
				                    {
				                        printf("Error: System failure.\n");
					                    exit(1);
				                    }
				                    if(!token)
				                    {
				                	    d++;
				                    }
				                    strcpy(line+i,symbols);
				                    i+=n+1;
				                    line[i-1]= ;
				                }
				                while(getc(inputFile)!='\n');
				                line[i-1]='\0';
				                data=cosine_data_create(line);
				                list_pushEnd(list,data);
				                free(line);
				                if(!token)
				                {
				             	    d--;
				            	    token=1;
				                }
				            }
				            n=list_length(list);
				            if((dataP=realloc(NULL,n*sizeof(Data)))==NULL)
				            {
				    	        printf("Error: System failure.\n");
						        exit(1);
				            }
				            i=0;
				            while(!list_isEmpty(list))
				            {
					            dataP[i]=list_pop(list);
				    	        i++;
				            }
				            list_destroy(list);
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
									    fprintf(stderr,"Could not create cosine hash descriptor.\n");
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
					            	fprintf(stderr,"Could not create hash table.\n");
		                            return 1;
				                }
					        }
					        for(i=0;i<=L-1;i++)
					        {
						        for(j=0;j<=n-1;j++)
					            {
					                hashTable_insert(H[i],dataP[j]);
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
								for(i=0;i<d;i++) fprintf(outputFile,"%f ",true_nearest[i]);
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
				        	error=1;
				        	printf("Error: Incorrect input file.\n");
		                    printf("Enter path name of input file and press [Enter]: ");
			                fgets(dPath,PATH_BUFFER_SIZE,stdin);
			            }
				    }
				}
			    else
			    {
			    	if((list=list_create())==NULL)
		    		{
		    			printf("Error: System failure.\n");
		    			exit(1);
		    		}
				    while(!feof(inputFile))
			        {
						line=NULL;
				        i=0;
				        do
				        {
				        	if(!token)
				        	{
				        		token=1;
				        	}
				        	else
				        	{
						        fscanf(inputFile,"%64s",symbols);
						    }
				            n=strlen(symbols);
				            if((line=realloc(line,(i+n+1)*sizeof(char)))==NULL)
				            {
				                printf("Error: System failure.\n");
					            exit(1);
				            }
				            if(token==1)
				            {
				                d++;
				            }
				            strcpy(line+i,symbols);
				            i+=n+1;
				            line[i-1]= ;
				        }
				        while(getc(inputFile)!='\n');
				        line[i-1]='\0';
				        data=euclidean_data_create(line);
				        list_pushEnd(list,data);
				        free(line);
				        if(token==1)
				        {
				            d--;
				            token=2;
				        }
				    }
				    n=list_length(list);
				    if((dataP=realloc(NULL,n*sizeof(Data)))==NULL)
				    {
				    	printf("Error: System failure.\n");
						exit(1);
				    }
				    i=0;
				    while(!list_isEmpty(list))
				    {
					    dataP[i]=list_pop(list);
				    	i++;
				    }
				    list_destroy(list);
					for(i=0;i<=L-1;i++)
					{
						do
						{
							if((g[i]=euclidean_hash_create(d,k,n))==NULL)
							{
								fprintf(stderr,"Could not create euclidean hash descriptor.\n");
		                        return 1;
				            }
							for(j=0;j<=i-1;j++)
							{
								token=euclidean_is_equal(g[i],g[j]);
							    if(token) break;
							}
					    }
					    while(token);
					    if((H[i]=hashTable_create(n/2,g[i]))==NULL)
					    {
						    fprintf(stderr,"Could not create hash table.\n");
		                    return 1;
				        }
					}
					for(i=1;i<=L-1;i++)
					{
						for(j=0;j<=n-1;j++)
						{
						    hashTable_insert(H[i],dataP[j]);
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
							for(i=0;i<d;i++) fprintf(outputFile,"%f ",true_nearest[i]);
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
	        else
	        {
			    if(!strcmp(symbols,"hamming"))
				{
					if((list=list_create())==NULL)
		            {
		    	        printf("Error: System failure.\n");
		    	        exit(1);
		            }
			        while(!feof(inputFile))
			        {
				        line=NULL;
				        i=0;
				        do
				        {
				            fscanf(inputFile,"%64s",symbols);
				            n=strlen(symbols);
				            if((!token)&&i)
				            {
				            	d=n;
				            	token=1;
				            }
				            if((line=realloc(line,(i+n+1)*sizeof(char)))==NULL)
				            {
				                printf("Error: System failure.\n");
					            exit(1);
				            }
				            strcpy(line+i,symbols);
				            i+=n+1;
				            line[i-1]= ;
				        }
				        while(getc(inputFile)!='\n');
				        line[i-1]='\0';
				        data=hamming_data_create(line);
				        list_pushEnd(list,data);
				        free(line);
			        }
			        n=list_length(list);
			        if((dataP=realloc(NULL,n*sizeof(Data)))==NULL)
			        {
				        printf("Error: System failure.\n");
				        exit(1);
			        }
			        i=0;
			        while(!list_isEmpty(list))
			        {
				        dataP[i]=list_pop(list);
				        i++;
			        }
			        list_destroy(list);
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
								fprintf(stderr,"Could not create hamming hash descriptor.\n");
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
						    fprintf(stderr,"Could not create hash table.\n");
		                    return 1;
				        }
					}
					for(i=0;i<=L-1;i++)
					{
						for(j=0;j<=n-1;j++)
						{
						    hashTable_insert(H[i],dataP[j]);
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
						long long int temp2 = true_nearest;
						while(temp2){
							if(temp2 & 1) fputc('1',outputFile);
							else fputc('0',outputFile);
							temp2 >>= 1;
						}
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
			        if(!strcmp(symbols,"matrix"))
		            {
					    fscanf(dataset,"%64s",symbols);
						if(!strcmp(symbols,"@items"))
						{
						    fscanf(dataset,"%*s");
						    if(fgetpos(dataset,&pos))
					        {
						        perror("Failure on fgetpos call");
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
						        perror("Failure on fsetpos call");
		                        return 1;
				            }
				            if((a=malloc(n*sizeof(unsigned int*)))==NULL)
				            {
							    perror("Failed to allocate memory for matrix distances");
			                    return 1;
		                    }
		                    for(i=0;i<=n-1;i++)
		                    {
							    if((a[i]=malloc(n*sizeof(unsigned int)))==NULL)
				                {
							        perror("Failed to allocate memory for matrix distances");
			                        return 1;
		                        }
		                    }
		                    for(i=0;i<=n-1;i++)
		                    	for(j=0;j<=n-1;j++)
		                    		fscanf(dataset,"%u",&a[i][j]);
						    tk=1;
						    for(i=1;i<=k;i++) tk*=2;
						    for(i=0;i<=L-1;i++)
					        {
						        do
						        {
							        if((g[i]=matrix_hash_create(k,a,n))==NULL)
							        {
								        fprintf(stderr,"Could not create matrix hash descriptor.\n");
		                                return 1;
				                    }
							        for(j=0;j<=i-1;j++)
							        {
								        token=matrix_is_equal(g[i],g[j]);
							            if(token) break;
							        }
					            }
					            while(token);
					            if((H[i]=hashTable_create(tk,g[i]))==NULL)
					            {
						            fprintf(stderr,"Could not create hash table.\n");
		                            return 1;
				                }
					        }
					        if((b=malloc(n*sizeof(unsigned int)))==NULL)
					        {
							    perror("Failed to allocate memory for position variables");
			                    return 1;
		                    }
		                    for(i=0;i<=n-1;i++) b[i]=i;
					        for(i=0;i<=L-1;i++)
					        	for(j=0;j<=n-1;j++)
							        hashTable_insert(H[i],&b[j]);

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
						    fprintf(stderr,"Error: Incorrect dataset file.\n");
		                    printf("Enter path name of dataset file and press [Enter]: ");
							fgets(dPath,PATH_BUFFER_SIZE,stdin);
				        }
		            }
		            else
		            {
						error=1;
						fprintf(stderr,"Error: Incorrect dataset file.\n");
		                printf("Enter path name of dataset file and press [Enter]: ");
						fgets(dPath,PATH_BUFFER_SIZE,stdin);
				    }
		            
				}
			}
		}
	}
	while(error);
	return 0;
}
