/* SOFTWARE DEVELOPMENT FOR ALGORITHMIC PROBLEMS
   ASSIGNMENT 3
   LELEGIANNIS IOANNIS: 1115201200090
   POULIDIS NIKOLAOS: 1115200000111 */

#include "RNG.h"
#include <stdlib.h>
#include <math.h>

int integerUniform(int n) /* Function generating pseudorandom integers with uniform distribution. */
{
    if(n<0)
	{
		return (int)(-(1.0/2.0)*RAND_MAX+rand()); /* Create pseudorandom integers in space [-(1.0/2.0)*RAND_MAX,(1.0/2.0)*RAND_MAX]. */
	}
	else
	{
		if(!n) /* Create pseudorandom integers in space [0,RAND_MAX]. */
		{
			return rand();
		}
		else
		{
		    return (rand()%n); /* Create pseudorandom integers in space [0,n-1]. */
		}
	}
}

float realUniform(float a,float b,int token) /* Function generating pseudorandom real numbers with uniform distribution in space with edges a,b. */
{
	float r;
	
	if(!token) /* Create pseudorandom real numbers in space (a,b). */
	{
		do
		{
			r=a+(rand()/(RAND_MAX+1.0))*(b-a);
		}
		while(r==a);
		return r;
	}
	else
	{
		if(token==1) /* Create pseudorandom real numbers in space (á,b]. */
		{
		    do
		    {
			    r=a+(rand()/(RAND_MAX+1.0))*(b-a+1);
		    }
		    while(r==a);
		    return r;
		}
		else
		{
		    if(token==2) /* Create pseudorandom real numbers in space [á,b). */
		    {
		    	return (a+(rand()/(RAND_MAX+1.0))*(b-a));
		    }
		    else
		    {
		    	if(token==3) /* Create pseudorandom real numbers in space [á,b]. */
		    	{
		    		return (a+(rand()/(RAND_MAX+1.0))*(b-a+1));
		    	}
		    }
		}
	}
}

float Gaussian01(void) /* Function generating pseudorandom real numbers with Gaussian distribution in space (0,1). */
{
	float x1,x2,s;
	
	do
	{
		x1=realUniform(-1.0,1.0,0);
		x2=realUniform(-1.0,1.0,0);
		s=x1*x1+x2*x2;
	}
	while(s>=1);
	return (x1*sqrt(-2*log(s)/s));
}
