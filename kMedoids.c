/* SOFTWARE DEVELOPMENT FOR ALGORITHMIC PROBLEMS
   ASSIGNMENT 2
   LELEGIANNIS IOANNIS: 1115201200090
   POULIDIS NIKOLAOS: 1115200000111 */
   
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

int main(int argc,char* argv[]) 
{
	/*Read command line arguements*/
	int i;
	char* outFilePath;
	for(i = 1;i < argc;i++){
		if(!strcmp(argv[i],"-d")){
			/*call function for evaluating input*/
		}
		else if(!strcmp(argv[i],"-c")){
			/*call function for evaluating configuration*/
		}
		else if(!strcmp(argv[i],"-o")){
			outFilePath = argv[i];
		}
		else{
			fprintf(stderr,"Invalid parameters given. Call should be:\n")
			fprintf(stderr,"./medoids -d <input file> -c <configuration file> -o <output file>\n");
			return 0;
		}
	}
	
	return 0;
}