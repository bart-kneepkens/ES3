#include    <avr/io.h>
.global main
main:       ldi     r16,                  0b111000    // define constant in r16
            out     _SFR_IO_ADDR(DDRB),   r16         // Set pins 11 through 13 to output using constant
            ldi     r16,                  0b100000
main_loop:  out     _SFR_IO_ADDR(PORTB),  r16
            ldi     r18,                  255
            ldi     r19,                  211
            ldi     r20,                  48
delay_loop: subi    r18,                  1           // Delay loop start
            sbci    r19,                  0
            sbci    r20,                  0
            brcc    delay_loop                        // Delay loop end
            lsr     r16
            sbic    _SFR_IO_ADDR(PORTB),  3
            ldi     r16,                  0b100000
            rjmp    main_loop
