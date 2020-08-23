/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
� Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : Project_UART
Version : v.1.0
Date    : 8/2/2018
Author  : Tran Minh Thuan
Company : VMMachine
Comments: 
Receiver and Transmittor
UCPOL=0 --  Data change at rising edge and sampling at falling edge
Chip type               : ATmega8
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega8.h>
#include <stdio.h>  // Standard Input/Output functions
#include <delay.h>
#define LED PORTB.0
// Declare your global variables here
// Declare subroutines
void GPIO_Init(void);                   //GPIO initialization
void UART_Init(void);                   //UART initialization
void TransmitByte(unsigned char data);  //UART transmit 1 byte
char ReceiveChar(void);                 //Receive 1 byte from UDR
int  UART_Available(void);              //Check the FIFO to see if empty or not
long rec=0;
void main(void)
{
GPIO_Init();
UART_Init();
#asm("sei")
while (1)
{
    if(UART_Available()){  /*Check if data in RX buffer not empty*/
    rec=ReceiveChar();
    if (rec<50) LED=1;
    else        LED=0;
    TransmitByte(rec);
    }                    
}}
// Subroutines define section
//--------------------------------GPIO--------------------------------//
//GPIO initilization
//Define output, input pins
void GPIO_Init(void)
{
// PB0 LED to test system
DDRB=0x01; 
// Port D initialization
// PD1 output for transmitter pin
DDRD|=0x02;  
};
/*-----------------------------------------------------------------------*/
//UART initilization 
//Define Frame struture, Baud rate,...
void UART_Init(void){
// USART initialization
// Communication Parameters: 7 Data, 1 Stop, Even parity
// USART Mode: ASync
// USART Baud Rate: 9600
UCSRA=(0<<RXC) | (0<<TXC) | (0<<UDRE) | (0<<FE) | (0<<DOR) | (0<<UPE) | (0<<U2X) | (0<<MPCM);
/* UCSRA - UART Control and Status Register A
RXC -  Receive complete flag
       +0 - Receive buffer empty
       +1 - There are unread data in receive buffer
TXC -  Transmit complete flag 
       +0 - Transmit ISR complete or write 1 the its location
       +1 - There are no data in transmit buffer
UDRE-  USART data register empty
FE  -  Frame error
DOR -  Data OveRun
PE  -  Parity error
U2X -  Double the USART transmission speed
MPCM-  Multi-processor Communication mod  */
UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (1<<RXEN) | (1<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
/* USART Control and status register B
RXCIE   -   RX Complete Interrupt Enable            - 0 Diasble interrupt       
TXCIE   -   Transmit Complete Interrupt Enable      - 0 Disable interrupt
UDRIE   -   Data Register Empty Interrupt Enable    - 0 Disable interrupt
RXEN    -   Receiver Enable                         - 1 Enable USART receiver
TXEN    -   Transmitter Enable                      - 1 Enable USART transmitter
USCZ    -   Character size                          - 011 8 bits data
RXB8    -   Receive Data Bit 8                      
TXB8    -   Transmit Data Bit 8
*/
UCSRC=(1<<URSEL)|(0<<UMSEL)|(1 <<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(0<<UCSZ0)|(0<<UCPOL);
/*
URSEL   -   Register Select         - Must be 1 when write to UCSRC                 
UMSEL   -   Mode Select     - 0     - Asynchronous   
UPM     -   Parity Mode     - 10    - Even mode
USBS    -   Stop Bit Select - 0     - 1 bit
UCSZ    -   Character Size  - 010   - 7 bit
UCPOL   -   Clock Polarity  - 0     - Transmit Rising edge- Receive Falling edge  
*/
UBRRH=0x00; //Set baud rate at 9600
UBRRL=0x33; //UBRR_value=(ClockSpeed/16/Baud_rate)-1 max 2^12-1
};
/*----------------Transmit data--------------------------*/
//Send 1 byte of data
//Input data to transmit
void TransmitByte(unsigned char data){
    while ((UCSRA & (1<<UDRE))==0); /* Wait for empty transmit buffer */
    UDR = data;                     /* Start transmittion */
}
/*----------------Receive data--------------------------*/
//Receive data
//Output data in UDR (Buffer)
char ReceiveChar(void) {
   return UDR;                      /* Read the data from the RX buffer*/
}
/*----------------Check UART available--------------------------*/
//Check UART available
int UART_Available(void){
   if ((UCSRA & 0x80)==0)       return 0; //Receiver buffer empty
   else                         return 1; //Not empty
}
/*---------------------------------------------------------------------------------------------------*/
