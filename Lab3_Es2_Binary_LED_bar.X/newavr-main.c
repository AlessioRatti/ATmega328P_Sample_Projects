/*
 * File:   newavr-main.c
 * Author: aless
 *
 * Created on January 28, 2020, 4:30 PM
 */

/*
 * VISUALIZZARE SU UNA BARRA LED (4 LED) IL VALORE
 * BINARIO DEL POTENZIOMETRO CAMPIONATO CON L?ADC
 * ALLA FREQUENZA DI 15 Hz
 */

// NOTE: USART functionality still available

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 16000000UL
#define USART_BAUDRATE 9600
#define MYUBRR (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#define START 4868

void USART_Init(void);
void USART_Transmit(char data );
void USART_putstring(char* StringPtr);

void set_port(void);
void set_timer(void);
void set_ADC (void);

int ADC_value;                  // 0-256 STRAIGHT OUT FROM THE ADC
char string[20];                    // STRING TO BE SENT VIA USART

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
    PORTD &= 0x00;                  // TURN OFF PORTD BY DEFAULT
    // SET PD4-PD7 AS OUTPUT
    DDRD  |= (1<<DDD4)|(1<<DDD5)|(1<<DDD6)|(1<<DDD7);             
    DDRC  &= (~(1<<DDC0));          // SET PC0 AS ADC INPUT
}

void set_timer(void){
    TCCR1B |= (1<<CS11)|(1<<CS10);  // CLOCK SOURCE /256
    TIMSK1 |= (1<<TOIE1);           // TIM1 OVERFLOW MODE
    TCNT1   = START;                // SET COUNT START
}

void set_ADC(void){
    ADMUX |= (1<<REFS0);            // SELECT REFERENCE VOLTAGE
                                    // A0/ADC0 AS INPUT SELECTED BY DEFAULT
    ADMUX |= (1<<ADLAR);            // LEFT ALIGNMENT
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // PRESCALER = 128
    ADCSRA |= (1<<ADEN);            // ENABLE ADC
    ADCSRA |= (1<<ADSC);            // START SINGLE CONVERSION
}

ISR(TIMER1_OVF_vect){
    TCNT1 = START;                  // SET TIM COUNT START
    PORTD = ADCH&(0b11110000);
    ADC_value = ADCH;
    sprintf(string, "%d\r\n", ADC_value);
    USART_putstring(string);
    ADCSRA |= (1<<ADSC);            // START SINGLE CONVERSION
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