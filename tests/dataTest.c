#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../HashGen.h"
#include "../HashTable.h"
#include "../data.h"
#include "../List.h"
#define BUFFER_BLOCK 4096  /*Block size*/
#define PATH_BUFFER_SIZE 100

/*User should free the returned pointer after use.
  Call with parameter NULL to clear allocated memory*/
char* readLine(FILE* file){
	static char* buffer = NULL;

	if(file == NULL){
		free(buffer);
		buffer = NULL;
		return NULL;
	}

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

		fgets(buffer+bytes_read,BUFFER_BLOCK,file);
		bytes_read+=BUFFER_BLOCK-1; /*if we read less chars than that we exit the loop*/
	}while((feof(file) == 0) && (strchr(buffer,'\n') == NULL));

	return buffer;
}

Data* evalInput(const char* inputFilePath,unsigned int* n,char* metric){
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
		return NULL;
	}

	token = strtok(NULL," \t\n");
	*metric = 0;
	if(!strcmp(token,"hamming")) *metric = 'h';
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
			return NULL;
		}

		token = strtok(NULL," \t\n");
		if(!strcmp(token,"euclidean")) *metric = 'e';
		else if(!strcmp(token,"cosine")) *metric = 'c';
		else{
			fprintf(stderr,"Invalid metric \"%s\" given on input file.\n",token);
			fclose(inputFile);
			return NULL;
		}
	}
	else if(!strcmp(token,"matrix")) *metric = 'm';
	else{
		fprintf(stderr,"Invalid metric space \"%s\" given.\n",token);
		fclose(inputFile);
		return NULL;
	}

	/*Start reading and creating data*/
	if(*metric != 'm'){
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
			if(*metric == 'h') newData = hamming_data_create(line);
			else if(*metric == 'e') newData = euclidean_data_create(line);
			else if(*metric == 'c') newData = cosine_data_create(line);
			else{/*This should never happen*/
				fprintf(stderr,"Unexpected error while reading input file.\n");
				fclose(inputFile);
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
			return NULL;
		}

		unsigned int i;
		for(i=0;i<(*n);i++){
			dataArray[i] = list_pop(l);
			if(dataArray[i] == NULL) break;
		}

		list_destroy(l);

		data_create_distance_matrix(dataArray,*n);

		fclose(inputFile);
		return dataArray;
	}
	else if(*metric == 'm'){
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
			return NULL;
		}

		unsigned int i;
		for(i=0;i<(*n);i++){
			dataArray[i] = list_pop(l);
			if(dataArray[i] == NULL) break;
		}

		unsigned int** distance_matrix = malloc((*n)*sizeof(unsigned int*));
		if(distance_matrix == NULL){
			perror("Failed to allocate memory for matrix distance matrix");
			fclose(inputFile);
			free(line);
			line=NULL;
			return NULL;
		}
		for(i=0;i<(*n);i++){
			distance_matrix[i]=malloc((*n)*sizeof(unsigned int));
			if(distance_matrix[i]==NULL){
				perror("Failed to allocate memory for matrix distance matrix");
				fclose(inputFile);
				free(line);
				line=NULL;
				unsigned int j;
				for(j=i-1;j>=0;j--) free(distance_matrix[j]);
				free(distance_matrix);
				return NULL;
			}
		}

		i=0;
		while(!feof(inputFile)){
			line = readLine(inputFile);
			if(line == NULL){
				perror("Could not allocate memory to read next line of input file");
				fclose(inputFile);
				return NULL;
			}
			if(line[0]=='\0') break;

			token = strtok(line," \n\t");
			unsigned int j = 0;
			while(token){
				distance_matrix[i][j] = atoi(token);
				token = strtok(NULL," \t\n");
				j++;
			}
			i++;
		}

		data_set_distance_matrix(distance_matrix,(*n));

		list_destroy(l);
		fclose(inputFile);
		return dataArray;
	}
	else{/*This should never happen*/
		fprintf(stderr,"Unexpected error while reading input file.\n");
		fclose(inputFile);
		return NULL;
	}
}

Data* evalQuery(const char* queryFilePath,double *r,char metric,unsigned int *q)
{
	FILE *queryFile;
	char *line,*symbols;
	List list;
	Data data,*dataP;
	unsigned int size,i;
	
	if((queryFile=fopen(queryFilePath,"r"))==NULL)
	{
		perror("Error: System failure.");
		exit(1);
	}

	if((line=readLine(queryFile))==NULL)
	{
	    printf("Error: Failure while reading query file.\n");
	    fclose(queryFile);
	    return NULL;
	}
	symbols = strtok(line," \t\n");
	if(!strcmp(symbols,"Radius:"))
	{
		symbols = strtok(NULL," \t\n");
		*r = atof(symbols);
		list=list_create();
		switch(metric)
		{
			case 'e':
				while(!feof(queryFile))
				{
					if((line=readLine(queryFile))==NULL)
	                {
	                    printf("Error: Failure while reading query file.\n");
	                    fclose(queryFile);
	                    return NULL;
	                }
	                if(line[0]=='\0') break;
	                data=euclidean_query_create(line);
	                list_pushEnd(list,data);
	            }
	            break;
	        case 'c':
	        	while(!feof(queryFile))
				{
					if((line=readLine(queryFile))==NULL)
	                {
	                    printf("Error: Failure while reading query file.\n");
	                    fclose(queryFile);
	                    return NULL;
	                }
	                if(line[0]=='\0') break;
	                data=cosine_query_create(line);
	                list_pushEnd(list,data);
	            }
	            break;
	        case 'h':
	        	while(!feof(queryFile))
				{
					if((line=readLine(queryFile))==NULL)
	                {
	                    printf("Error: Failure while reading query file.\n");
	                    fclose(queryFile);
	                    return NULL;
	                }
	                if(line[0]=='\0') break;
	                data=hamming_query_create(line);
	                list_pushEnd(list,data);
	            }
	            break;
	        case 'm':
	        	while(!feof(queryFile))
				{
					if((line=readLine(queryFile))==NULL)
	                {
	                    printf("Error: Failure while reading query file.\n");
	                    fclose(queryFile);
	                    return NULL;
	                }
	                if(line[0]=='\0') break;
	                data=matrix_query_create(line);
	                list_pushEnd(list,data);
	            }
	            break;
	        default:
	        	printf("Error: Incorrect metric space.\n");
	        	fclose(queryFile);
	        	return NULL;
	    }
	    *q=list_length(list);
	    if((dataP=realloc(NULL,(*q)*sizeof(Data)))==NULL)
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
	    fclose(queryFile);
	    return dataP;
	}
	else
	{
		printf("Error: Incorrect query file.\n");
		fclose(queryFile);
		return NULL;
	}
}

void evalOutput(char* outputFilePath,char metric,int L,int k,int n,Data* input,int q,Data* queries,double r)
{
	FILE *outputFile;
    int d,i,j,token=0,counter=1,threshold=3*L;
	HashDescriptor *g;
	HashTable *H;
	Data current,next;
	double *distance,*dLSH=NULL,*dTrue=NULL;
	uint64_t idLSH,idQ,id,idTrue;
	time_t startLSH,endLSH,startTrue,endTrue;

	g = malloc(L*sizeof(HashDescriptor));
	if(g == NULL){
		perror("Failed to allocate memory for hash function descriptos array");
		exit(2);
	}

	H = malloc(L*sizeof(HashTable));
	if(H == NULL){
		perror("Failed to allocate memory for hash table array");
		exit(3);
	}
	
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
		            return;
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
		            return;
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
		            return;
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
			//if(matrix_data_get_id(input[i])>=n) fprintf(stderr,"Problem\n");
			hashTable_insert(H[j],input[i]);
		}
	}
	for(i=0;i<=q-1;i++)
	{
		fprintf(outputFile,"\nQuery: item%d\n",i+1);
		fprintf(outputFile,"R-near neighbors:\n");
		startLSH=time(NULL);
		for(j=0;j<=L-1;j++)
		{
		    hashTable_insert(H[j],queries[i]);
		    current=queries[i];
		    while((next=hashTable_getNext(H[j],current))!=NULL)
		    {
			    if((distance=general_distance(current,next))==NULL)
		    	{
		    		printf("Error: Failure while writing in output file.\n");
		    		return;
		    	}
		    	if(*distance<r)
		    	{
		    		fprintf(outputFile,"item%lu\n",data_getID(next));
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
		    		if(*distance<*dLSH)
		    		{
		    		    *dLSH=*distance;
		    		    idLSH=data_getID(next);
		    		}
		    	}
		    }
		}
		endLSH=time(NULL);
		fprintf(outputFile,"Nearest neighbor(LSH): item%lu\n",idLSH);
		fprintf(outputFile,"distanceLSH: %f\n",*dLSH);
		idQ=data_getID(queries[i]);
		startTrue=time(NULL);
		for(j=0;j<=n-1;j++)
		{
			if((id=data_getID(input[j]))!=idQ)
			{
				if((distance=general_distance(queries[i],input[j]))==NULL)
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
		fprintf(outputFile,"Nearest neighbor(True): item%lu\n",idTrue);
		fprintf(outputFile,"distanceTrue: %f\n",*dTrue);
		fprintf(outputFile,"tLSH: %f second(s)\n",difftime(endLSH,startLSH));
		fprintf(outputFile,"tTrue: %f second(s)\n",difftime(endTrue,startTrue));
	}
}

int main(int argc,char* argv[])
{
	if(argc != 4){
		printf("Wrong number of arguements\n");
		return 1;
	}
	Data* input;
	Data* query;
	unsigned int n,L=5,k=4,q;
	double r;
	char metric;
	
    if((input=evalInput(argv[1],&n,&metric))!=NULL)
    {
    	printf("Input success.\n");
    }
    else
    {
	    printf("Input failure.\n");
	}
    if((query=evalQuery(argv[2],&r,metric,&q))!=NULL)
    {
    	printf("Query success.\n");
    }
    else
    {
	    printf("Query failure.\n");
	}
	evalOutput(argv[3],metric,L,k,n,input,q,query,r);
	readLine(NULL);
	return 0;
}
