/* амаптунг коцислийоу циа акцояихлийа пяобкглата
   кекециаммгс иыаммгс: 1115201200090  
   поукидгс мийокаос: 1115200000111 
   тсейоуяа йымстамтима: 1115201300228 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int k,L,s;
	char check[20];
	FILE *data;
	
	if((argc!=7)&&(argc!=9)&&(argc!=11))
	{
		printf("Error: Missing data.\n");
		printf("Press [Enter] to terminate the program.\n");
		getc(stdin);
		return 1;
	}
	if(strcmp(argv[1],"-d")||strcmp(argv[3],"-q"))
	{
	    printf("Error: Incorrect data.\n");
		printf("Press [Enter] to terminate the program.\n");
		getc(stdin);
		return 1;
	}
	if(argc==7)
	{
		if(strcmp(argv[5],"-o"))
		{
		    printf("Error: Incorrect data.\n");
		    printf("Press [Enter] to terminate the program.\n");
		    getc(stdin);
		    return 1;
	    }
	    k=4;
	    L=5;
	}
	else
	{
		if(argc==9)
		{
		    if((strcmp(argv[5],"-k")&&strcmp(argv[5],"-L"))||strcmp(argv[7],"-o"))
		    {
			    printf("Error: Incorrect data.\n");
		        printf("Press [Enter] to terminate the program.\n");
		        getc(stdin);
		        return 1;
	        }
	        if(!strcmp(argv[5],"-k"))
	        {
	        	k=atoi(argv[6]);
	        	L=5;
	        }
	        else
	        {
	        	k=4;
	        	L=atoi(argv[6]);
	        }
	    }
	    else
	    {
		    if(strcmp(argv[5],"-k")||strcmp(argv[7],"-L")||strcmp(argv[9],"-o"))
			{
			    printf("Error: Incorrect data.\n");
		        printf("Press [Enter] to terminate the program.\n");
		        getc(stdin);
		        return 1;
	        }
	        k=atoi(argv[6]);
	        L=atoi(argv[8]);
	    }
	}
	do
	{
	    if((data=fopen(argv[2],"r"))==NULL)
	    {
		    printf("Error: File can not be opened.\n");
		    printf("Press [Enter] to terminate the program.\n");
		    getc(stdin);
			return 1;
		}
	    fscanf(data,"%s",check);
	    if(strcmp(check,"@metric_space"))
	    {
		    printf("Error: Incorrect data.\n");
		    printf("Press [Enter] to terminate the program.\n");
		    getc(stdin);
		    return 1;
	    }
	    fscanf(data,"%s",check);
	    if(!strcmp(check,"vector"))
	    {
	    	
	    }
	    else
	    {
		    if(!strcmp(check,"hamming"))
		    {
		    	
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
				    	
				    }
				    else
				    {
					    printf("Error: Incorrect data.\n");
		                printf("Press [Enter] to terminate the program.\n");
		                getc(stdin);
						return 1;
	                }
	            }
	        }
	    }
	    fclose(data);
	    do
	    {
		    printf("Menu options:\n");
		    printf("(1) New search.\n");
	        printf("(2) Terminate program.\n");
	        printf("Enter the number that corresponds to your selection and press [Enter]: ");
	        scanf("%d",&s);
	        while(getc(stdin)!='\n')
		    {
			    ;
		    }
		    if((s<=0)||(s>=3))
		    {
			    printf("Error: The value you entered is not valid.\n");
		    }
	    }
 	    while((s<=0)||(s>=3));
    }
 	while(s==1);
	return 0;
}
