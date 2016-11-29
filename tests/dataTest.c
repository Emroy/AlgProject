#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>
//#include "HashGen.h"
//#include "HashTable.h"
#include "../data.h"
#include "../List.h"
#define BUFFER_BLOCK 4096  /*Block size*/
#define PATH_BUFFER_SIZE 100

/*user should free the returned pointer after use*/
char* readLine(FILE* file){
	static char* buffer = NULL;
	if(buffer != NULL){
		free(buffer);
		buffer = NULL;
	}

	unsigned int buffer_size = 0;
	unsigned int bytes_read = 0;
	unsigned int i;

	do{
		buffer_size += BUFFER_BLOCK;

		buffer = realloc(buffer,buffer_size);
		if(buffer == NULL) return NULL;
		for(i=buffer_size-BUFFER_BLOCK;i<buffer_size;i++) buffer[i] = 0;
		
		fgets(buffer+bytes_read,buffer_size,file);
		bytes_read+=BUFFER_BLOCK; /*if we read less chars than that we exit the loop*/

	}while(!feof(file) && !strchr(buffer+bytes_read-BUFFER_BLOCK,'\n'));

	return buffer;
}

Data* evalInput(const char* inputFilePath,unsigned int* n){
	FILE* inputFile = fopen(inputFilePath,"r");
	if(inputFile == NULL){
		perror("Failed to open input file");
		exit(-1);
	}

	char* line = readLine(inputFile);
	if(line == NULL){
		perror("Could not allocate memory to read next line of input file");
		fclose(inputFile);
		return NULL;
	}
	char* token = strtok(line," \t\n");
	if(strcmp("@metric_space",token)){
		fprintf(stderr,"Invalid command \"%s\" given on input file\n",token);
		fclose(inputFile);
		free(line);
		line = NULL;
		return NULL;
	}

	token = strtok(NULL," \t\n");
	char metric = 0;
	if(!strcmp(token,"hamming")) metric = 'h';
	else if(!strcmp(token,"euclidean") || !strcmp(token,"vector")){
		line = readLine(inputFile);
		if(line == NULL){
			perror("Could not allocate memory to read next line of input file");
			fclose(inputFile);
			return NULL;
		}
		token = strtok(line," \t\n");
		if(strcmp("@metric",token)){
			fprintf(stderr,"Invalid command \"%s\" given for metric space euclidean on input file.\n",token);
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}

		token = strtok(NULL," \t\n");
		if(!strcmp(token,"euclidean")) metric = 'e';
		else if(!strcmp(token,"cosine")) metric = 'c';
		else{
			fprintf(stderr,"Invalid metric \"%s\" given on input file.\n",token);
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}
	}
	else if(!strcmp(token,"matrix")) metric = 'm';
	else{
		fprintf(stderr,"Invalid metric space \"%s\" given.\n",token);
		fclose(inputFile);
		free(line);
		line = NULL;
		return NULL;
	}

	/*Start reading and creating data*/
	if(metric != 'm'){
		List l = list_create();
		/*must do something if list could not be created*/
		while(!feof(inputFile)){
			line = readLine(inputFile);
			if(line == NULL){
				perror("Could not allocate memory to read next line of input file");
				fclose(inputFile);
				return NULL;
			}
			if(line[0]=='\0') break;

			Data newData;
			if(metric == 'h') newData = hamming_data_create(line);
			else if(metric == 'e') newData = euclidean_data_create(line);
			else if(metric == 'c') newData = cosine_data_create(line);
			else{/*This should never happen*/
				fprintf(stderr,"Unexpected error while reading input file.\n");
				fclose(inputFile);
				free(line);
				line = NULL;
				return NULL;
			}

			if(newData == NULL) return NULL;

			list_pushEnd(l,newData);
		}

		*n = list_length(l);
		Data* dataArray = malloc((*n)*sizeof(Data));
		if(dataArray == NULL){
			perror("Failed to allocate memory for data array");
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}

		unsigned int i;
		for(i=0;i<(*n);i++){
			dataArray[i] = list_pop(l);
			if(dataArray[i] == NULL) break;
		}

		list_destroy(l);
		fclose(inputFile);
		free(line);
		line = NULL;
		return dataArray;
	}
	else if(metric == 'm'){
		line = readLine(inputFile);
		if(line == NULL){
			perror("Could not allocate memory to read next line of input file");
			fclose(inputFile);
			return NULL;
		}

		token = strtok(line," ,\t\n");
		if(strcmp(token,"@items")){
			fprintf(stderr,"Invalid command \"%s\" given on matrix input file\n",token);
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}

		List l = list_create();
		while(token = strtok(NULL," ,\t\n")){
			Data newData = matrix_data_create(token);
			list_pushEnd(l,newData);
		}

		*n = list_length(l);
		Data* dataArray = malloc((*n)*sizeof(Data));
		if(dataArray == NULL){
			perror("Failed to allocate memory for data array");
			fclose(inputFile);
			free(line);
			line = NULL;
			return NULL;
		}

		unsigned int i;
		for(i=0;i<(*n);i++){
			dataArray[i] = list_pop(l);
			if(dataArray[i] == NULL) break;
		}

		list_destroy(l);
		fclose(inputFile);
		free(line);
		line = NULL;
		return dataArray;
	}
	else{/*This should never happen*/
		fprintf(stderr,"Unexpected error while reading input file.\n");
		fclose(inputFile);
		free(line);
		line = NULL;
		return NULL;
	}
}

Data* evalQuery(const char* queryFilePath,double *r,char metric)
{
	FILE *queryFile;
	char *line,symbols[65];
	List list;
	Data data,*dataP;
	unsigned int size,i;
	
	if((queryFile=fopen(queryFilePath,"r"))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	if((line=readLine(queryFile))==NULL)
	{
	    printf("Error: Failure while reading query file.\n");
	    return NULL;
	}
	sscanf(line,"%64s",symbols);
	if(!strcmp(symbols,"Radius:"))
	{
		sscanf(line,"%lf",r);
		list=list_create();
		switch(metric)
		{
			case 'e':
				while(!feof(queryFile))
				{
					if((line=readLine(queryFile))==NULL)
	                {
	                    printf("Error: Failure while reading query file.\n");
	                    return NULL;
	                }
	                data=euclidean_data_create(line);
	                list_pushEnd(list,data);
	            }
	            break;
	        case 'c':
	        	while(!feof(queryFile))
				{
					if((line=readLine(queryFile))==NULL)
	                {
	                    printf("Error: Failure while reading query file.\n");
	                    return NULL;
	                }
	                data=cosine_data_create(line);
	                list_pushEnd(list,data);
	            }
	            break;
	        case 'h':
	        	while(!feof(queryFile))
				{
					if((line=readLine(queryFile))==NULL)
	                {
	                    printf("Error: Failure while reading query file.\n");
	                    return NULL;
	                }
	                data=hamming_data_create(line);
	                list_pushEnd(list,data);
	            }
	            break;
	        case 'm':
	        	while(!feof(queryFile))
				{
					if((line=readLine(queryFile))==NULL)
	                {
	                    printf("Error: Failure while reading query file.\n");
	                    return NULL;
	                }
	                data=hamming_data_create(line);
	                list_pushEnd(list,data);
	            }
	            break;
	        default:
	        	printf("Error: Incorrect metric space.\n");
	        	return NULL;
	    }
	    if((dataP=realloc(NULL,list_length(list)*sizeof(Data)))==NULL)
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
	    return dataP;
	}
	else
	{
		printf("Error: Incorrect query file.\n");
		return NULL;
	}
}

void evalOutput(char* outputFilePath,char metric,int L,int k,int n,Data* data,int q,Data* queries,double r)
{
	FILE *outputFile;
    int d,i,j,token=0,counter=1,threshold=3*L;
	HashDescriptor *g;
	HashTable *H;
	Data current,next;
	double *distance,*dLSH=NULL,*dTrue=NULL;
	uint64_t idLSH,idQ,id,idTrue;
	time_t startLSH,endLSH,startTrue,endTrue;
	
	if((outputFile=fopen(outputFilePath,"w"))==NULL)
	{
		printf("Error: System failure.\n");
		exit(1);
	}
	switch(metric)
	{
		case 'e':
		    d=euclidean_data_get_dimention();
	        for(i=0;i<=L-1;i++)
	        {
		        do
		        {
			        if((g[i]=euclidean_hash_create(d,k,n))==NULL)
			        {
				        fprintf(stderr,"Could not create euclidean hash descriptor.\n");
		                exit(1);
			        }
			        for(j=0;j<=i-1;j++)
			        {
				        if(token=euclidean_is_equal(g[i],g[j]))
				        {
				            break;
				        }
			        }
		        }
				while(token);
		        if((H[i]=hashTable_create(n/2,g[i]))==NULL)
        	    {
			        fprintf(stderr,"Could not create hash table.\n");
		            return 1;
		        }
	        }
	        break;
	    case 'c':
	    	d=cosine_data_get_dimention();
	    	for(i=1;i<=k;i++)
	    	{
	    		counter*=2;
	    	}
	        for(i=0;i<=L-1;i++)
	        {
		        do
		        {
			        if((g[i]=cosine_hash_create(d,k))==NULL)
			        {
				        fprintf(stderr,"Could not create cosine hash descriptor.\n");
		                exit(1);
			        }
			        for(j=0;j<=i-1;j++)
			        {
				        if(token=cosine_is_equal(g[i],g[j]))
				        {
				            break;
				        }
			        }
		        }
				while(token);
		        if((H[i]=hashTable_create(counter,g[i]))==NULL)
        	    {
			        fprintf(stderr,"Could not create hash table.\n");
		            return 1;
		        }
	        }
	        break;
	    case 'h':
	    	d=hamming_data_get_dimention();
	    	for(i=1;i<=k;i++)
	    	{
	    		counter*=2;
	    	}
	        for(i=0;i<=L-1;i++)
	        {
		        do
		        {
			        if((g[i]=hamming_hash_create(d,k))==NULL)
			        {
				        fprintf(stderr,"Could not create hamming hash descriptor.\n");
		                exit(1);
			        }
			        for(j=0;j<=i-1;j++)
			        {
				        if(token=hamming_is_equal(g[i],g[j]))
				        {
				            break;
				        }
			        }
		        }
				while(token);
		        if((H[i]=hashTable_create(counter,g[i]))==NULL)
        	    {
			        fprintf(stderr,"Could not create hash table.\n");
		            return 1;
		        }
	        }
	        break;
	    case 'm':
	    	for(i=1;i<=k;i++)
	    	{
	    		counter*=2;
	    	}
	    	for(i=0;i<=L-1;i++)
	        {
		        do
		        {
			        if((g[i]=matrix_hash_create(k,n))==NULL)
			        {
				        fprintf(stderr,"Could not create matrix hash descriptor.\n");
		                exit(1);
			        }
			        for(j=0;j<=i-1;j++)
			        {
				        if(token=matrix_is_equal(g[i],g[j]))
				        {
				            break;
				        }
			        }
		        }
				while(token);
		        if((H[i]=hashTable_create(counter,g[i]))==NULL)
        	    {
			        fprintf(stderr,"Could not create hash table.\n");
		            return;
		        }
	        }
	        break;
	    default:
	    	printf("Error: Incorrect metric space.\n");
	    	return;
	}
	for(i=0;i<=n-1;i++)
	{
	    for(j=0;j<=L-1;j++)
		{
			hashTable_insert(H[j],data[i]);
		}
	}
	for(i=0;i<=q-1;i++)
	{
		fprintf(outputFile,"\nQuery: item%d\n",i+1);
		fprintf(outputFile,"R-near neighbors:\n");
		counter=0;
		startLSH=time(NULL);
		for(j=0;j<=L-1;j++)
		{
		    hashTable_insert(H[j],queries[i]);
		    current=queries[i];
		    while((next=hashTable_getNext(H[j],current))!=NULL)
		    {
		    	counter++;
		    	if((distance=data_distance(current,next))==NULL)
		    	{
		    		printf("Error: Failure while writing in output file.\n");
		    		return;
		    	}
		    	if(*distance<r)
		    	{
		    		fprintf(outputFile,"item%d\n",data_getID(next));
		    	}
		    	if(dLSH==NULL)
		    	{
		    		if((dLSH=realloc(NULL,sizeof(double)))==NULL)
		    		{
		    			printf("Error: Failure while writing in output file.\n");
		    		    return;
		    	    }
		    		*dLSH=*distance;
		    		idLSH=data_getID(next);
		    	}
		    	else
		    	{
				    if(counter>threshold)
		    		{
		    			break;
		    		}
		    		else
		    		{
		    		    if(*distance<*dLSH)
		    		    {
		    		    	*dLSH=*distance;
		    		    	idLSH=data_getID(next);
		    		    }
		    		}
		    	}
		    }
		}
		endLSH=time(NULL);
		fprintf(outputFile,"Nearest neighbor(LSH): item%d\n",idLSH);
		fprintf(outputFile,"distanceLSH: %f\n",*dLSH);
		idQ=data_getID(queries[i]);
		startTrue=time(NULL);
		for(j=0;j<=n-1;j++)
		{
			if((id=data_getID(data[j]))!=idQ)
			{
				if((distance=data_distance(queries[i],data[j]))==NULL)
		    	{
		    		printf("Error: Failure while writing in output file.\n");
		    		return;
		    	}
				if(dTrue==NULL)
				{
					if((dTrue=realloc(NULL,sizeof(double)))==NULL)
		    		{
		    			printf("Error: Failure while writing in output file.\n");
		    		    return;
		    	    }
		    		*dTrue=*distance;
		    		idTrue=id;
		    	}
		    	else
		    	{
		    		if(*distance<*dTrue)
		    		{
		    			*dTrue=*distance;
		    			idTrue=id;
		    		}
		    	}
		    }
		}
		endTrue=time(NULL);
		fprintf(outputFile,"Nearest neighbor(True): item%d\n",idTrue);
		fprintf(outputFile,"distanceTrue: %f\n",*dTrue);
		fprintf(outputFile,"tLSH: %f second(s)\n",difftime(endLSH,startLSH));
		fprintf(outputFile,"tTrue: %f second(s)\n",difftime(endTrue,startTrue));
	}
}

int main(int argc,char* argv[])
{
	if(argc != 2){
		printf("Wrong number of arguements\n");
		return 1;
	}
	Data* temp;
	unsigned int n;
	double r;
	char metric;
	
	printf("Enter the value of r and press [Enter]: ");
	scanf("%lf",&r);
	while(getc(stdin)!='\n')
	{
		;
	}
	printf("Enter the first letter of the metric space and press [Enter]: ");
	scanf("%c",&metric);
	while(getc(stdin)!='\n')
	{
		;
	}
    if((temp=evalQuery(argv[2],&r,metric))!=NULL)
    {
    	printf("Success.\n");
    }
    else
    {
	    printf("Failure.\n");
	}
	return 0;
}
