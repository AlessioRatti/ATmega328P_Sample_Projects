/*
 * File:   newavr-main.c
 * Author: aless
 *
 * Created on January 27, 2020, 6:16 PM
 */

/*
 * INSERIRE UN PULSANTE IN PULL DOWN SUL PIEDINO PC5.
 * SCRIVERE UN FIRMWARE CHE REPLICHI LO STATO DEL
 * PULSANTE TRAMITE L?ACCENSIONE/SPEGNIMENTO DEL LED PB5.
 * QUANDO IL PULSANTE E? PREMUTO IL LED DEVE RESTARE
 * ACCESO PER TUTTA LA DURATA DELLA PRESSIONE.
 * COME ANDREBBE MODIFICATO IL CODICE SE IL PULSANTE
 * FOSSE IN PULL UP?
 */

#include <avr/io.h>

void set_port(void);

int main(void) {
    set_port();
    while (1) {
        // KEEP IN MIND THAT THE BUTTON PULLS DOWN THE VOLTAGE
        if(PINC & (1<<PINC5))           // (1&1 = 1) or (1&0 = 0)
            PORTB &= (~(1<<PORTB5));    // LED OFF
        else
            PORTB |= (1<<PORTB5);       // LED ON
    }
}

void set_port(void){
    PORTB &= (~(1<<PORTB5));            // TURN OFF LED BY DEFAULT
    DDRB |= (1<<DDB5);                  // SET PB5 (LED) AS OUTPUT
    PORTC |= (1<<PORTC5);            // SET PC5 AS PULL-UP
    DDRC &= (~(1<<DDC5));               // SET PC5 AS INPUT (BUTTON)
}