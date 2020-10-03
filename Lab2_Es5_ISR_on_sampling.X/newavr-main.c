/*
 * File:   newavr-main.c
 * Author: aless
 *
 * Created on January 28, 2020, 12:58 PM
 */

/*
 * SCRIVERE UN FIRMWARE PER CONTARE LA DURATA IN SECONDI DELLA
 * PRESSIONE DEL PULSANTE SUL PC5 UTILIZZANDO UNA INTERRUPT
 * ATTIVATA DALL?OVERFLOW DEL TIMER1.
 * UTILIZZARE LE FUNZIONI FORNITE PER LA COMUNICAZIONE SERIALE
 * PER INVIARE AL PC LA DURATA DELLA PRESSIONE DEL PULSANTE
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 16000000UL
#define USART_BAUDRATE 9600
#define MYUBRR (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#define START 63535

void USART_Init(void);
void USART_Transmit(char data );
void USART_putstring(char* StringPtr);

void set_port(void);
void set_button_ISR(void);
void set_timer(void);

char string[10];
unsigned int time = 0;

int main(void) {
    USART_Init();
    set_port();
    set_button_ISR();
    set_timer();
    sei();
    while (1) {
    }
}

void set_port(void){
    PORTB &= (~(1<<PORTB5));    // TURN OFF LED BY DEFAULT
    DDRB  |= (1<<DDB5);         // SET LED PORT AS OUTPUT
    PORTC |= (1<<PORTC5);       // SET PC5 AS PULL-UP
    DDRC  &= (~(1<<DDC5));      // SET PC5 AS INPUT
}

void set_timer(void){
    TCCR1B |= (1<<CS11);        // CLOCK SOURCE /8
    TIMSK1 |= (1<<TOIE1);       // TIM1 OVERFLOW MODE
    TCNT1   = START;            // SET COUNT START TO 3035
}

void set_button_ISR(void){
    PCICR |= (1<<PCIE1);
    PCMSK1 |= (1<<PCINT13);
}

ISR(PCINT1_vect){
    if(PINC & (1<<PINC5)){          // IF BUTTON RELEASED
        PORTB |= (1<<PORTB5);       // LED OFF
        sprintf(string, "%d ms\r\n", time);
        USART_putstring(string);
    }
    else{                           // IF BUTTON PRESSED
        PORTB &= (~(1<<PORTB5));    // LED ON
        time = 0;                   // RESET TIME COUNT
    }
    
}

ISR(TIMER1_OVF_vect){
    TCNT1 = START;              // SET COUNT START TO 3035
    time++;
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
