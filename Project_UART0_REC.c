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
#define LED PORTB.1
// Declare your global variables here
// Declare subroutines
void GPIO_Init(void);                   //GPIO initialization
void UART_Init(void);                   //UART initialization
void TransmitByte(unsigned char data);  //UART transmit 1 byte
char CharAvailable(void);
char ReceiveChar(void);
void USART_Flush(void);
char ReceiveChar(); 
char rec;

// Standard Input/Output functions
#include <stdio.h>
#include <delay.h>
void main(void)
{
GPIO_Init();
UART_Init();
while (1)
      {
      rec=ReceiveChar();
      if((rec%2)==1) 
      LED=1;
      if((rec%2)!=1)
      LED=0; 
      delay_ms(250);
      }                    
}
// Subroutines define section
//--------------------------------GPIO--------------------------------//
//GPIO initilization
//Define output, input pins
void GPIO_Init(void)
{
// Port B initialization
// Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
DDRB=(1<<DDB7)|(1<<DDB6)|(1<<DDB5)|(1<<DDB4)|(1<<DDB3)|(1<<DDB2)| (1<<DDB1) | (1<<DDB0);
// State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
PORTB=(0<<PORTB7)|(0<<PORTB6)|(0<<PORTB5)|(0<<PORTB4)|(0<<PORTB3)|(0<<PORTB2)|(0<<PORTB1)|(0<<PORTB0);

// Port C initialization
// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=Out Bit3=In Bit2=In Bit1=Out Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (1<<DDD4) | (0<<DDD3) | (0<<DDD2) | (1<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=0 Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);    
};
/*-----------------------------------------------------------------------*/
//UART initilization 
//Define Frame struture, Baud rate,...
void UART_Init(void)
{
// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: On
// USART Transmitter: On
// USART Mode: Sync. Master UCPOL=0
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
UCSRC=(1<<URSEL)|(0<<UMSEL)|(0<<UPM1)|(0<<UPM0)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0)|(0<<UCPOL);
/*
URSEL   -   Register Select         - Must be 1 when write to UCSRC
UMSEL   -   Mode Select     - 0     - Asynchronous   
UPM     -   Parity Mode     - 00    - Disable
USBS    -   Stop Bit Select - 0     - 1 bit
UCSZ    -   Character Size  - 110   - 8 bit
UCPOL   -   Clock Polarity  - 0     - Transmit Rising edge- Receive Falling edge  
*/
UBRRH=0x00; //Set baud rate at 9600
UBRRL=0x33; //UBRR_value=(ClockSpeed/16/Baud_rate)-1 max 2^12-1
};
///////////////////////////////////////////////////////////////////////
/*----------------Transmit data--------------------------*/
//Send 1 byte of data
//Input data to transmit
void TransmitByte(unsigned char data )
{
    while ((UCSRA & (1<<UDRE))==0); /* Wait for empty transmit buffer */
    UDR = data;                     /* Start transmittion */
}
/*----------------Receive data--------------------------*/
//Receive data
//Output data in UDR (Buffer)
char ReceiveChar() 
{
   while ( !(UCSRA & (1 << RXC)) );  // Wait until data is received  
   return UDR;                      // Read the data from the RX buffer
};
/*---------------------------------------------------------------------------------------------------*/
