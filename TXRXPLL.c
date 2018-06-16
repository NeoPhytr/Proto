
#include "global.h"
#include <math.h>

int FreqSel(unsigned int FLO)
{unsigned int x, y, NINT, NFRAC, FrqSel, i = 0;
    float N,  temp, FRange, FREF = 30.72, FRange_l[16] = {0.2325, 0.285625, 0.336875, 0.405, 0.465, 0.57125, 0.67375, 0.81, 0.93, 1.1425, 1.3475, 1.62, 1.86, 2.285, 2.695, 3.24,};
    float FRange_H[16] = {0.285625, 0.336875, 0.405, 0.465, 0.57125, 0.67375, 0.81, 0.93, 1.1425, 1.3475, 1.62, 1.86, 2.285, 2.695, 3.24, 3.72,};
    unsigned int val[16] = {0x27, 0x2F, 0x37, 0x3F, 0x26, 0x2E, 0x36, 0x3E, 0x25, 0x2D, 0x35, 0x3D, 0x24, 0x2C, 0x34, 0x3C,};
    
	printf("Enter Freq to be generated\n");
	scanf("%d", &FLO);
	printf("FLO = %d\n", FLO);
	FRange=(float)FLO/1000;
	printf("FRange = %f\n", FRange);
		
	while(!(FRange >= FRange_l[i] && FRange < FRange_H[i]))
    {
        i++;
    }
    FrqSel = val[i];
	
	printf("i = %d\n", i);
	printf("FreqSel = %x\n", FrqSel);
    
	x = pow(2, ((FrqSel & 0x07)-3));
    printf("x = %d\n", x);
	N = ((x * FLO)/FREF);
	printf("N = %f\n", N);
    NINT = (int)N; 
	printf("NINT = %d\n", NINT);
    temp = pow(2, 23) * (N - (float)NINT);
	NFRAC = (int)temp;
	printf("NFRAC = %d\n", NFRAC);

}



   