

#include <avr/io.h>   //contains i/o register macros
#include <util/delay.h>   //generates blocking delay
#include <avr/interrupt.h>  //contains all the interrupt vectors
#include <math.h>
#include <Arduino.h>

#define BIT(a) (1 << (a))

void setup()
{ 
  cli();

  DDRB |= BIT(1) | BIT(2); // pin 9 set to output //OC1A
  //int ms = 1.5;
  //int pulse_width = ms*2000;	// 20 millisec(50hz of a motor) = 16e6/(50(hz)*8(prescalar)) so x ms =  x*2000 counts
  
  // clear timer1 control register (ie use default values)
  TCCR1A = 0;
  TCCR1B = 0;
  
  TCCR1B |= BIT(4) | BIT(1); // phase and fequency correct PWM with prescaler 8
  //19999 cout till this no. and then downcount - top value
  // depended on 50hz frequency of our motor
  //ICR1 = F_CPU/(2*50*8) - 1; 
  ICR1 = 19999;

  
  TCCR1A |= BIT(7) | BIT(6)| BIT(5) | BIT(4);	// Inverting mode
	//OCR1A = ICR1 - (pulse_width/2);
  OCR1A = ICR1 - 1000;


	
	//TIMSK1 |= BIT(OCIE1A) | BIT(OCIE1B);
	TCNT1 = 0;	// setting timer to zero // initializing
	
	sei();

}

void loop()
{
	}
