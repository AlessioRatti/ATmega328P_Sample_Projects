/*
 * File:   newavr-main.c
 * Author: aless
 *
 * Created on January 27, 2020, 5:59 PM
 */

/*
 * AGGIUNGERE UN LED SUL PIEDINO PB0 E FARLO
 * LAMPEGGIARE IN CONTROFASE RISPETTO A PB5
 */

#include <avr/io.h>

void set_port(void);

int main(void) {
    unsigned int i,j;
    set_port();
    while (1) {
        for(i=0;i<30;i++){
            for(j=0;j<60000;j++);
        }
        PORTB ^= (1<<PORTB0)^(1<<PORTB5);       // TOGGLE BOTH PB0 AND PB5
    }
}

void set_port(void){
    PORTB &= (~(1<<PORTB0))&(~(1<<PORTB5)); // SET PB0 & PB5 AS LOW
    DDRB |= (1<<DDB0)|(1<<DDB5);            // SET PB0 & PB5 AS OUTPUT
    PORTB |= (1<<PORTB0);                   // SET PB0 AS HIGH
}