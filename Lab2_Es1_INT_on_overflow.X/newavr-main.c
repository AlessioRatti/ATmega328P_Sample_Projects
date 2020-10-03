/*
 * File:   newavr-main.c
 * Author: aless
 *
 * Created on January 28, 2020, 10:58 AM
 */

/*
  * SCRIVERE UN FIRMWARE CHE SFRUTTI L?OVERFLOW DEL TIMER1 PER
  * ATTIVARE UN?INTERRUPT E COMMUTARE LO STATO DEL LED INTERNO
  * AL MICROCONTROLLORE (SU PB5) AD UNA FREQUENZA DI CIRCA 1Hz
*/

// f = 16M/(256*2^16)

#include <avr/io.h>
#include <avr/interrupt.h>

void set_port(void);
void set_timer(void);

int main(void) {
    set_port();
    set_timer();
    sei();
    while (1) {
    }
}

void set_port(void){
    PORTB &= (~(1<<PORTB5));    // TURN OFF LED BY DEFAULT
    DDRB |= (1<<DDB5);          // SET LED PORT AS OUTPUT
}

void set_timer(void){
    TCCR1B |= (1<<CS12);        // CLOCK SOURCE /256
    TIMSK1 |= (1<<TOIE1);       // TIM1 OVERFLOW MODE
}

ISR(TIMER1_OVF_vect){
    PORTB ^= (1<<PORTB5);         // TOGGLE LED
}
