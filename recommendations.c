#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[]){
	int i = 0;
	Ratings ratings;
	char* outputFilePath;
	short validate = 0;

	while(i < argc){
		if(!strcmp(argv[i],"-d")){
			ratings = readRatings(argv[i+1]);
			i+=2;
		}
		else if(!strcmp(argv[i],"-o")){
			outputFilePath = argv[i+1];
			i+=2;
		}
		else if(!strcmp(argv[i],"-validate")){
			validate = 1;
			i++;
		}
		else{
			fprintf(stderr,"\"%s\" is not a valid command line command\n",argv[i]);
			return -1;
		}
	}

	return 0;
}