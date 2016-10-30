#include <RNG.h>
#include <stdlib.h>
#include <math.h>

int integerUniform(int n) /* ��������� ����������� ������������ �������� �� ���������� ��������. */
{
    if(n<=0)
	{
		return (int)(-(1.0/2.0)*RAND_MAX+rand()); /* ���������� ������������ �������� ��� �������� [-(1.0/2.0)*RAND_MAX,(1.0/2.0)*RAND_MAX]. */
	}
	else
	{
	    return (rand()%n); /* ���������� ������������ ������� ��� �������� [0,n-1]. */
	}
}

float realUniform(float a,float b,int token) /* ��������� ����������� ������������ ����������� �� ���������� �������� ��� �������� �� ���� �� a,b. */
{
	float r;
	
	if(!token) /* ���������� ������������ ����������� ��� �������� (�,b). */
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
		if(token==1) /* ���������� ������������ ����������� ��� �������� (�,b]. */
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
		    if(token==2) /* ���������� ������������ ����������� ��� �������� [�,b). */
		    {
		    	return (a+(rand()/(RAND_MAX+1.0))*(b-a));
		    }
		    else
		    {
		    	if(token==3) /* ���������� ������������ ����������� ��� �������� [�,b]. */
		    	{
		    		return (a+(rand()/(RAND_MAX+1.0))*(b-a+1));
		    	}
		    }
		}
	}
}

float Gaussian01(void) /* ��������� ����������� ������������ ����������� �� �������� �������� ��� �������� (0,1). */
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
