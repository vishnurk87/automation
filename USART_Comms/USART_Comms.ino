
#include <avr/io.h>		//contains i/o register macros
#include <util/delay.h> 	//generates blocking delay
#include <avr/interrupt.h>	//contains all the interrupt vectors
#include <math.h>
#include <Arduino.h>


#define F_cpu 16000000 
//defining CPU frequency 
//https://stackoverflow.com/questions/59487494/sending-data-from-atmega324p-to-arduino-by-usart

#define BAUD 9600		//baud rate setting
//#define BAUD_Prescalar ((F_cpu/(8*BAUD))-1)
#define BIT(a) (1 << (a))

#define TX_COMPLETE_INTERUPT_ON BIT(TXCIE0)
#define DATA_REG_EMPTY_INTERUPT BIT(UDRIE0)
#define DOUBLE_TX_SPEED BIT(U2X0)

#define TX_ENABLE BIT(TXEN0)
#define EVEN_PARITY BIT(UPM01)

volatile uint8_t USART_TX_Buffer; // Global Buffer

void setup()
{	
	
	DDRB |= BIT(DDB5);
	uint8_t data[3];
	data[0] = 'a';
	data[1] = 'k';
	data[2] = 'b';
	unsigned int BAUD_Prescalar = 207;
	USART_init(BAUD_Prescalar);
	//for (int i = 0; i<3; i++) 
	//{

	//give delay here to produce error free tx and rx, otherwise tw bytes were sent instread of one
	delay(1000);
		USART_TransmitInterrupt(data[1]);
	//}
	
}


void USART_TransmitInterrupt(uint8_t buffer)
{	
	USART_TX_Buffer = buffer;
	UCSR0A |= UDRE0;
	UCSR0B|= DATA_REG_EMPTY_INTERUPT;

}

void USART_init(unsigned int baud_rate)
{

	cli();
//set baud rate
	
	UBRR0H = (unsigned char)(baud_rate>>8);
	UBRR0L = (unsigned char)baud_rate;

	// double speed to get correct baud rate for F_cpu
	UCSR0A |= DOUBLE_TX_SPEED;
//	Enable transmitter and reciever
	UCSR0B = 0;
 	UCSR0B |= TX_ENABLE | (1<<RXEN0);


 	// 8 bit transmission, even parity and 1 stop bit
 	UCSR0C = 0; // set every bit to 0 first
 	UCSR0C |= BIT(UCSZ01) ;
 	UCSR0C|= BIT(UCSZ00); 
 	UCSR0C|= EVEN_PARITY;

 	//denable interupts
 	sei();
 	
}

ISR(USART_UDRE_vect)
{	
	PORTB &= ~BIT(PORTB5);
	UDR0 = USART_TX_Buffer;
	UCSR0B &= ~DATA_REG_EMPTY_INTERUPT; // Disables the Interrupt, uncomment for one time transmission of data
	//delay(1000);
	
}
void loop()
{

}