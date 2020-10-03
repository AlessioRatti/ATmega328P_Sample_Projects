/*
 * File:   newavr-main.c
 * Author: aless
 *
 * Created on January 28, 2020, 12:34 PM
 */

/*
 * 1) MONTARE L?ENCODER OTTICO SULLA BREADBOARD IN MODO DA PORTARNE
 * L?USCITA DIGITALE (DO) SUL PIN C5 DEL MICROCONTROLLORE.
 * 
 * 2) CONFIGURARE I REGISTRI IN MODO TALE DA ATTIVARE UNA INTERRUPT
 * A OGNI CAMBIO DI VALORE LOGICO DELL?INGRESSO AL PIN C5, A CUI
 * L?ENCODER E? COLLEGATO.
 * 
 * 3) SFRUTTARE LE FUNZIONI FORNITE PER IL CONTROLLO DELLA
 * COMUNICAZIONE SERIALE PER INVIARE UN CARATTERE AL PC CHE
 * NOTIFICHI OGNI CAMBIO DI STATO DELL?ENCODER.
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
void set_button_ISR(void);

char status[8];

int main(void) {
    USART_Init();
    set_port();
    set_button_ISR();
    sei();
    while (1) {
    }
}

void set_port(void){
    PORTB &= (~(1<<PORTB5));    // TURN OFF LED BY DEFAULT
    DDRB |= (1<<DDB5);          // SET LED PORT AS OUTPUT
    PORTC |= (1<<PORTC5);       // SET PC5 AS PULL-UP
    DDRC &= (~(1<<DDC5));       // SET PC5 AS INPUT
}

void set_button_ISR(void){
    PCICR |= (1<<PCIE1);
    PCMSK1 |= (1<<PCINT13);
}

ISR(PCINT1_vect){
    if(PINC & (1<<PINC5))
        sprintf(status, "%s\r\n", "UP");
    else
        sprintf(status, "%s\r\n", "DOWN");
    USART_putstring(status);
    PORTB ^= (1<<PORTB5);
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
