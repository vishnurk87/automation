
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
#define RX_READY_INTERUPT BIT(RXCIE0)
#define TX_COMPLETE_INTERUPT_ON BIT(TXCIE0)
#define DATA_REG_EMPTY_INTERUPT BIT(UDRIE0)
#define DOUBLE_TX_SPEED BIT(U2X0)

#define TX_ENABLE BIT(TXEN0)
#define EVEN_PARITY BIT(UPM01)

volatile uint8_t USART_TX_Buffer; // Global Buffer
volatile uint8_t USART_RX_DATA; // Global RX

volatile int TX_shuffle = 1; // 1 for transmit and 0 for recieve

void setup()
{	
	
	DDRB |= BIT(DDB5);
	
	unsigned int BAUD_Prescalar = 207;
	USART_init(BAUD_Prescalar);

	//give delay here to produce error free tx and rx, otherwise tw bytes were sent instread of one
	delay(1000);

	//calling usart start function 
	USART();

}

// function that sends and recieves one byte of data at a time
void USART(){

	uint8_t data[3];
	data[0] = 'a';
	data[1] = 'k';
	data[2] = 'b';
	DR_Empty_Interrupt(data[0]);
	
}


void USART_Recieve()
{	
	//set recieve complete RXCn bit to 1 if necessary to initiate the interupt
	UCSR0B|= RX_READY_INTERUPT;
}

//fucntion to turn on tx complete interupt 
void USART_Transmit()
{	

	//this creates an interupt when tx is complete
	UCSR0B |= TX_COMPLETE_INTERUPT_ON;


}
void DR_Empty_Interrupt(uint8_t buffer)
{	

	//shufle changed to TX mode (TX_shuffle =1)
	TX_shuffle = 1;
	USART_TX_Buffer = buffer;

	UCSR0B|= DATA_REG_EMPTY_INTERUPT;

}

void USART_init(unsigned int baud_rate)
{

	cli();
//set baud rate
	
	UBRR0H = (unsigned char)(baud_rate>>8);
	UBRR0L = (unsigned char)baud_rate;

	//clearing flags
	UCSR0A &= ~BIT(RXC0);
	UCSR0A &= ~BIT(TXC0);

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


//interupts


//recieve complete interrupt
ISR(USART_RX_vect)
{	

	// Disables data empty interupt enabled in TX interupt and in the next lines reads the data

	USART_RX_DATA = UDR0;
	USART_TX_Buffer = USART_RX_DATA;

	//disabls rx ready interupt.
	UCSR0B &= ~RX_READY_INTERUPT;
	
	//USART_Transmit();

}


ISR(USART_TX_vect)
{	

	UDR0 = USART_TX_Buffer;


	//UCSR0B &= ~TX_COMPLETE_INTERUPT_ON;

	USART_Recieve();

}

ISR(USART_UDRE_vect)
{	

	UDR0 = USART_TX_Buffer;

	UCSR0B &= ~DATA_REG_EMPTY_INTERUPT; 

	USART_Transmit();
}



void loop()
{

}