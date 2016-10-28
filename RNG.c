#include <RNG.h>
#include <stdlib.h>
#include <math.h>

int naturalUniform(int n)
{
    if(n<=0)
	{
		return (int)(-(1.0/2.0)*RAND_MAX+rand());
	}
	else
	{
	    return (rand()%n);
	}
}

float realUniform(float a,float b,int token)
{
	float r;
	
	if(!token)
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
		if(token==1)
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
		    if(token==2)
		    {
		    	return (a+(rand()/(RAND_MAX+1.0))*(b-a));
		    }
		    else
		    {
		    	if(token==3)
		    	{
		    		return (a+(rand()/(RAND_MAX+1.0))*(b-a+1));
		    	}
		    }
		}
	}
}

float Gaussian01(void)
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
