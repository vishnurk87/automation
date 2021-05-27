
#include <avr/io.h>		//contains i/o register macros
#include <util/delay.h> 	//generates blocking delay
#include <avr/interrupt.h>	//contains all the interrupt vectors
#include <math.h>
#include <Arduino.h>


#define F_cpu 16000000UL //defining CPU frequency
#define BAUD 9600		//baud rate setting
#define BAUD_Prescalar ((F_cpu/(16*BAUD))-1)
#define BIT(a) (1 << (a))

#define TX_COMPLETE_INTERUPT_ON BIT(TXCIE0)
#define DATA_REG_EMPTY_INTERUPT BIT(UDRIE0)

#define TX_ENABLE BIT(TXEN0)
#define EVEN_PARITY BIT(UPM01)

volatile uint8_t USART_TX_Buffer; // Global Buffer

void setup()
{
	DDRB |= BIT(PB5);
	char data = 'a';
	USART_init();
	USART_TransmitInterrupt(data);

	while (1) {
		PORTB |= 1<<5; // Writing HIGH to glow LED
		_delay_ms(500);
		PORTB &= ~(1<<5); // Writing LOW
		_delay_ms(500);	
	}
}


void USART_TransmitInterrupt(uint8_t buffer)
{
	USART_TX_Buffer = buffer;
	UCSR0B|= DATA_REG_EMPTY_INTERUPT;
}

void USART_init()
{
//set baud rate
	UBRR0H = (unsigned char)(BAUD_Prescalar>>8);
	UBRR0L = (unsigned char)BAUD_Prescalar;

	cli();
//	Enable transmitter and reciever
	UCSR0B = 0;
 	UCSR0B |= TX_ENABLE ;

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
	UDR0 = USART_TX_Buffer;
	//UCSR0B &= ~DATA_REGISTER_EMPTY_INTERRUPT; // Disables the Interrupt, uncomment for one time transmission of data
}
void loop()
{

}