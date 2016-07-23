# include <stdio.h>


unsigned char * transmit(unsigned char * num)
{

	
	int i, n = 0;
	unsigned char * p;
	
	for (i = 0; num[i] != '\0'; i++)
	{
		if (num[i] >= '0'&&num[i] <= '9')
			n = n * 16 + num[i] - '0';
		if (num[i] >= 'a'&&num[i] <= 'f')
			n = n * 16 + num[i] - 'a' + 10;
		if (num[i] >= 'A'&&num[i] <= 'F')
			*p = n * 16 + num[i] - 'A' + 10;
		return p;
	}

}