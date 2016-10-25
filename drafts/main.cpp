#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

int main(int argc,char* argv[]){

	string d,q,o;
	int k = 4;
	int l = 5;

	//Read comand line arguements
	int i = 1;
	while(i < argc){
		if(!strcmp(argv[i],"-d")){
			d = argv[i+1];
		}
		else if(!strcmp(argv[i],"-q")){
			q = argv[i+1];
		}
		else if(!strcmp(argv[i],"-k")){
			k = atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-L")){
			l = atoi(argv[i+1]);
		}
		else if(!strcmp(argv[i],"-o")){
			o = argv[i+1];
		}
		else{
			i++;
			continue;
		}
		i+=2;
	}

	return 0;
}