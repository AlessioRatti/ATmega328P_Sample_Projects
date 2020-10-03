/*
 * File:   newavr-main.c
 * Author: aless
 *
 * Created on January 28, 2020, 11:28 AM
 */

/*
 * UTILIZZARE LE FUNZIONI FORNITE PER SCRIVERE UN FIRMWARE CHE
 * FUNZIONI DA ECO SULLA PORTA SERIALE:
 * ALLA RICEZIONE DI UN CARATTERE, DEVE RITRASMETTERE AL PC LO
 * STESSO CARATTERE E INVERTIRE LO STATO DEL LED «L» SU PB5.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 16000000UL
#define USART_BAUDRATE 9600
#define MYUBRR (((F_CPU/(USART_BAUDRATE*16UL)))-1)

void USART_Init(void);
void USART_Transmit(char data );
char USART_Receive();
void USART_putstring(char* StringPtr);

void set_port(void);

char received;

int main(void) {
    USART_Init();
    set_port();
    sei();
    while (1) {
    }
}

void set_port(void){
    PORTB &= (~(1<<PORTB5));    // TURN OFF LED BY DEFAULT
    DDRB |= (1<<DDB5);          // SET LED PORT AS OUTPUT
}

ISR(USART_RX_vect){
	received = USART_Receive();	// Receive blocking function
	PORTB ^= (1 << PORTB5);
	USART_Transmit(received);
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
char USART_Receive()
{
	while(!(UCSR0A & (1<<RXC0))){};
	return UDR0;
}

void USART_putstring(char* StringPtr)
{
	while(*StringPtr != 0x00)
	{
		USART_Transmit(*StringPtr);
		StringPtr++;
	}
}
