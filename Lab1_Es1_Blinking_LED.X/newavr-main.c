/*
 * File:   newavr-main.c
 * Author: aless
 *
 * Created on January 27, 2020, 5:26 PM
 */

/*
 * MODIFICARE IL FIRMWARE PER FAR LAMPEGGIARE IL LED
 * DI ARDUINO NANO, COLLEGATO AL PIN PB5 DI ATMEGA328P
 */

#include <avr/io.h>

void set_port(void);

int main(void) {
    unsigned int i, j;
    set_port();
    while (1) {
        for(i=0;i<10;i++){
            for(j=0;j<60000;j++);
        }
        PORTB ^= (1<<PORTB5);       // toggle LED every cycle completion
    }
}

void set_port (){
    PORTB &= (~(1<<PORTB5));    // TURN OFF LED BY DEFAULT
    DDRB |= (1<<DDB5);          // SET LED PORT AS OUTPUT
}