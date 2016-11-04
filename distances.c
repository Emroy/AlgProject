/* SOFTWARE DEVELOPMENT FOR ALGORITHMIC PROBLEMS
   ASSIGNMENT 1
   LELEGIANNIS IOANNIS: 1115201200090
   POULIDIS NIKOLAOS: 1115200000111 */

#include <math.h>
   
int hamming(long long int x,long long int y)
{
	int distance = 0;
   	while(x | y){
   		if((x & 0x1)!=(y & 0x1)) distance+=1;
   		x >>= 1;
   		y >>= 1;
   	}

   	return distance;
}

double euclidean(double *x,double *y,int d)
{
	int i;
	double sum=0.0;
	
	for(i=0;i<=d-1;i++)
	{
		sum+=(x[i]-y[i])*(x[i]-y[i]);
	}
	return sum;
}

double cosine(double *x,double *y,int d)
{
	int i;
	double xy=0.0,xx=0.0,yy=0.0;
	
	for(i=0;i<=d-1;i++)
	{
		xy+=x[i]*y[i];
	}
	for(i=0;i<=d-1;i++)
	{
		xx+=x[i]*x[i];
	}
	xx=sqrt(xx);
	for(i=0;i<=d-1;i++)
	{
	    yy+=y[i]*y[i];
	}
	yy=sqrt(yy);
	return (xy/(xx*yy));
}
	    
   	    
