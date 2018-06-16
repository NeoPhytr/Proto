/* 
 * File:   Lime_Tx_PLL.c
 * Author: Naveen Kurumaddali
 * Description : LMS6002D Transceiver programming in transmitter mode
 * Created on April 6, 2018, 12:18 PM
 */

// DSPIC30F5011 Configuration Bit Settings

// 'C' source line config statements

// FOSC
#pragma config FPR = ECIO_PLL16         // Primary Oscillator Mode (ECIO w/ PLL 16x)
#pragma config FOS = PRI                // Oscillator Source (Primary Oscillator)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_4           // POR Timer Value (4ms)
#pragma config BODENV = BORV_20         // Brown Out Voltage (2.0V)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FBS
#pragma config BWRP = WR_PROTECT_BOOT_OFF// Boot Segment Program Memory Write Protect (Boot Segment Program Memory may be written)
#pragma config BSS = NO_BOOT_CODE       // Boot Segment Program Flash Memory Code Protection (No Boot Segment)
#pragma config EBS = NO_BOOT_EEPROM     // Boot Segment Data EEPROM Protection (No Boot EEPROM)
#pragma config RBS = NO_BOOT_RAM        // Boot Segment Data RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WR_PROT_SEC_OFF   // Secure Segment Program Write Protect (Disabled)
#pragma config SSS = NO_SEC_CODE        // Secure Segment Program Flash Memory Code Protection (No Secure Segment)
#pragma config ESS = NO_SEC_EEPROM      // Secure Segment Data EEPROM Protection (No Segment Data EEPROM)
#pragma config RSS = NO_SEC_RAM         // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = GSS_OFF            // General Segment Code Protection (Disabled)

// FICD
//#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
/********************************************************************************************************************************/

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <p30f5011.h>
#include <stdint.h>
#include <stdbool.h>
#include "global.h"

/*SPI1 SEN =RB2(14), SPI1 CLK =RF6(35), SPI1 MOSI =RF3(33), SPI1 MISO =RF2(34) 
  SPI2 SEN =RG9(8), SPI2 CLK =RG6(4), SPI2 MOSI =RG8(6), SPI2 MISO = RG7(5)*/

/*char array1[10] = "{0}",array2[10] = "{0}",array3[10] = "{0}",array4[10] = "{0}",array5[10] = "{0}",array6[10] = "{0}",array7[10] = "{0}";
char *p = array1,*q = array2,*r = array3,*s = array4,*t = array5,*w = array6,*x = array7;*/

unsigned int ReadByte;

void delay(unsigned int p)
	{
		unsigned int k;
		for(k=0;k<p;k++);
 	}

void Delayms ( void )
{
    int16_t temp;
    for( temp = 0; temp < 255; temp++ );
}

void BitBang_Init()
{
    ADPCFG=0XFFFF;
    ADCON1=0x0000;
    
    BitBangIO_Tris   = 0;
    BitBangCLK_Tris  = 0;                       //Defining Bit banging port pins as outputs
    BitBangSS_Tris   = 0;
    comp_Tris = 0;
        
    BitBangIO  = 0;
    BitBangSS  = 0;                             //Slave select is active high
    BitBangCLK = 0;                             //Making clock polarity (CKP) High to emulate SPIMODE 3 Operation.    
    comp = 0;                                   //Comparison Pin. When Read Write registers are equal it is made high 
}

void BitBang_Read(unsigned int SPIBYTE)
{
	unsigned char spicount;
	
	BitBangSS = 0;
	for(spicount=16;spicount>0;spicount--)
	{
		BitBangIO=((SPIBYTE & 0x80)>>7);
		delay(50);
		BitBangCLK =1;
		delay(50);
		BitBangCLK=0;
		delay(50);
		SPIBYTE = SPIBYTE << 1;
	}
}

void BitBang_Write(unsigned int SPIBYTE)
{
    unsigned char spicount;
         
    BitBangSS = 1;
        for(spicount=8;spicount>0;spicount--)
    {
        BitBangIO=((SPIBYTE & 0X80)>>7);
        delay(100);
        BitBangCLK=1;                       
        delay(200);                              //Syncing clock rising edge to the data to operate in SPIMODE 3. 
        BitBangCLK=0;
        delay(100);
        SPIBYTE=SPIBYTE <<1 ;
    }
}

void SPI1_Read_Init()
{
    IFS0bits.SPI1IF = 0;
    IEC0bits.SPI1IE = 0;
        
    SPI1CONbits.FRMEN  = 0;        //Framed SPI support disabled.
    SPI1CONbits.DISSDO = 0;        //SDO1 pin is controlled by the module.
    SPI1CONbits.MODE16 = 1;        //Communication is byte-wide (8 bits).
    SPI1CONbits.SMP    = 0;        //Input Data is sampled at the middle of data output time
    SPI1CONbits.CKE    = 0;        //Serial output data changes on transition from Idle clock state to active clock state
    SPI1CONbits.CKP    = 0;        //Idle state for clock is a low level; active state is a high level
    SPI1CONbits.MSTEN  = 0;        //Master Mode Disabled
    
    //SPI1BUF = 0;                   //Clearing SPIBUF register
    
    SPI1STATbits.SPIROV = 0;    
    SPI1STATbits.SPIEN  = 1;        //Enable SPI Module
    
    
    IFS0bits.SPI1IF = 0;
    IEC0bits.SPI1IE = 1;
}

/*void SPI1_Read(unsigned int SPIBYTE)
{
   //while( !SPI1STATbits.SPIRBF );               //Wait for the data to be received
   SPIBYTE = SPI1BUF;
}*/

void SPI2_Write_Init()
{
    IFS1bits.SPI2IF = 0;                            
    IEC1bits.SPI2IE = 1;
    
    SPI2CONbits.FRMEN  = 0;           //Framed SPI support disabled.
    SPI2CONbits.DISSDO = 0;           //SDO1 pin is controlled by the module.
    SPI2CONbits.MODE16 = 1;           //Communication is byte-wide (8 bits).
    SPI2CONbits.SMP    = 0;           //Input Data is sampled at the middle of data output time
    SPI2CONbits.CKE    = 0;           //Serial output data changes on transition from Idle clock state to active clock state
    SPI2CONbits.CKP    = 0;           //Idle state for clock is a low level; active state is a high level
    SPI2CONbits.MSTEN  = 1;           //Master Mode Enabled
    
    SPI2STATbits.SPIROV = 0; 
    SPI2STATbits.SPIEN  = 1;         //Enable SPI Module
    
}

void SPI2_Write(unsigned int SPIBYTE)
{
    //while( !SPI2STATbits.SPITBF );
    SPI2BUF = SPIBYTE;                              // write the data out to the SPI peripheral
    /*IFS1bits.SPI2IF = 0;                            
    IEC1bits.SPI2IE = 0;                    */
}

/*void __attribute__((__interrupt__, auto_psv)) _SPI1Interrupt(void)
{
    ReadByte = SPI1BUF;
    IFS0bits.SPI1IF = 0;
}*/

void __attribute__((__interrupt__, auto_psv)) _SPI2Interrupt(void)
{
    IFS1bits.SPI2IF = 0;
    comp = 1;
}

void main() 
{
     unsigned int i,datax[6] = {0x53,0x50,0x49,0x4C,0x4F,0x4F,};
        
    SPI2_Write_Init();
    //SPI1_Read_Init();
    
    for(i=0;i<6;i++)
    {
        SPI2_Write(datax[i]);
    }
    /*ReadByte = SPI1BUF;
    if(ReadByte == 0xCCCC)
    {
        comp == 1;
    }
    else
    {
       BitBangIO = 1;
    }*/
    //while(1);
}



