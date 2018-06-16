/* 
 * File:   global.h
 * Author: nEO
 *
 * Created on April 28, 2018, 11:55 AM
 */

#define BitBangIO          PORTBbits.RB12
#define BitBangCLK         PORTBbits.RB13
#define BitBangSS          PORTBbits.RB14
#define comp               PORTCbits.RC14

#define BitBangIO_Tris      TRISBbits.TRISB12
#define BitBangCLK_Tris     TRISBbits.TRISB13
#define BitBangSS_Tris      TRISBbits.TRISB14
#define comp_Tris           TRISCbits.TRISC14



void UART2_Init(void);
void BitBang_Init();
void BitBang_Write();
void Delay ( void );
void delay(unsigned int p);
void SPI2_Write_Init();
void SPI2_Write();
void SPI1_Read_Init();
void SPI1_Read();
int FreqSel(unsigned int FLO);
void __attribute__((__interrupt__, auto_psv)) _SPI2Interrupt(void);

//extern int FrqSel;