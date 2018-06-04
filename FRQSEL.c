
#include <stdio.h>

void main()
{
	int i=0;
	float  CarFrq, FreqSel;
	float uplt[16] = {0.2325, 0.285625, 0.336875, 0.405, 0.465, 0.57125, 0.67375, 0.81, 0.93, 1.1425, 1.3475, 1.62, 1.86, 2.285, 2.695, 3.24,};
	float lowlt[16] = {0.285625, 0.336875, 0.405, 0.465, 0.57125, 0.67375, 0.81, 0.93, 1.1425, 1.3475, 1.62, 1.86, 2.285, 2.695, 3.24, };
	int val[16] = {0x47, 0x57, 0x67, 0x77, 0x46, 0x56, 0x66, 0x76, 0x45, 0x55, 0x65, 0x75, 0x44, 0x54, 0x64, 0x74,};

	printf("Enter Frequency to be generated" );
	scanf("%f", CarFrq);

	while(!(CarFrq >= uplt[i] && CarFrq < lowlt[i]))
	{
		i++;
	}

	FreqSel = val[i];
	printf("FreqSel value = %x", FreqSel);
}
