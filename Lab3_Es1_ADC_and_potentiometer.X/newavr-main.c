/*
 * File:   newavr-main.c
 * Author: aless
 *
 * Created on January 28, 2020, 3:14 PM
 */

 /*
  * INVIARE TRAMITE SERIALE IL VALORE DEL POTENZIOMETRO
  * CAMPIONATO DALL?ADC ALLA FREQUENZA ESATTA DI 10 Hz
  * FAR LAMPEGGIARE IL LED SU PB5 ALLA STESSA FREQUENZA.
  */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 16000000UL
#define USART_BAUDRATE 9600
#define MYUBRR (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#define START 59285

void USART_Init(void);
void USART_Transmit(char data );
void USART_putstring(char* StringPtr);

void set_port(void);
void set_timer(void);
void set_ADC (void);

unsigned long int ADC_value;              // 0-1023 STRAIGHT OUT FROM THE ADC
unsigned long int voltage;              // CONVERSION TO VOLTS
char string[20];            // STRING TO BE SENT VIA USART

int main(void) {
    USART_Init();
    set_port();
    set_timer();
    set_ADC();
    sei();
    while (1) {
    }
}

void set_port(void){
    PORTB &= (~(1<<PORTB5));    // TURN OFF LED BY DEFAULT
    DDRB  |= (1<<DDB5);         // SET LED PORT AS OUTPUT
    DDRC  &= (~(1<<DDC0));      // SET PC0 AS ADC INPUT
}

void set_timer(void){
    TCCR1B |= (1<<CS12);        // CLOCK SOURCE /256
    TIMSK1 |= (1<<TOIE1);       // TIM1 OVERFLOW MODE
    TCNT1   = START;            // SET COUNT START
}

void set_ADC(void){
    ADMUX |= (1<<REFS0);        // SELECT REFERENCE VOLTAGE
                                // A0/ADC0 AS INPUT SELECTED BY DEFAULT
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // PRESCALER = 128
    ADCSRA |= (1<<ADEN);        // ENABLE ADC
    ADCSRA |= (1<<ADSC);        // START SINGLE CONVERSION
}

ISR(TIMER1_OVF_vect){
    TCNT1 = START;                  // SET TIM COUNT START
    ADCSRA |= (1<<ADSC);            // START SINGLE CONVERSION
    ADC_value = (ADCL)|(ADCH<<8);
    voltage = (ADC_value*5000)/1023;
    sprintf(string, "Voltage = %1.0ld.%0.3ld [V] \t(%ld)\r\n", voltage/1000, voltage%1000, ADC_value);
    USART_putstring(string);
}

// ############################## USART FUNCTIONS ##############################

void USART_Init(void)
{
	UBRR0H=(unsigned char)(MYUBRR>>8);	// baud rate msb
	UBRR0L=(unsigned char)(MYUBRR);		// baud rate lsb
	
	UCSR0B |= (1<<RXEN0);				// RX enabled
	UCSR0B |=(1<<TXEN0);				// TX enabled
	UCSR0C &= !(1<<UMSEL00);			// asynchronous USART
	UCSR0C &= !(1<<UMSEL01);			// asynchronous USART
	UCSR0C &= !(1<<UPM01);				// no parity
	UCSR0C &= !(1<<UPM00);				// no parity
	UCSR0C &= !(1<<USBS0);				// 1 stop bit
	UCSR0C |=(1<<UCSZ01);				// 8 bit
	UCSR0C |=(1<<UCSZ00);				// 8 bit
	UCSR0B |= (1<<RXCIE0);				// RX enable interrupt flag
}

void USART_Transmit(char data )
{
	while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){}
	UDR0= data;
}

void USART_putstring(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		USART_Transmit(*StringPtr);
		StringPtr++;
	}
}
