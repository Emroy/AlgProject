/* SOFTWARE DEVELOPMENT FOR ALGORITHMIC PROBLEMS
   ASSIGNMENT 1
   LELEGIANNIS IOANNIS: 1115201200090
   POULIDIS NIKOLAOS: 1115200000111 */

#include <math.h>
   
int hamming(long long int x,long long int y)
{
    int xBits[64],yBits[64],counter=0,size,i,dH=0;
    
	do
   	{
	    xBits[counter]=x%2;
		x/=2;
		counter++;
	}
	while(x);
	counter=0;
	do
   	{
		yBits[counter]=y%2;
		y/=2;
		counter++;
	}
	while(y);
	size=counter;
	for(i=0;i<=size-1;i++)
	{
	    if(xBits[i]!=yBits[i])
	    {
	    	dH++;
	    }
	}
	return dH;
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
	return (xy/xx*yy);
}
	    
   	    
