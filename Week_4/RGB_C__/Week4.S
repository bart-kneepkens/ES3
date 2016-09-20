#include <avr/io.h>
.global main
delay1s:    ldi     r18,                  255         // Subroutine that delays for 1 second
            ldi     r19,                  211
            ldi     r20,                  48
delay_loop: subi    r18,                  1           // Loop for the delay subroutine
            sbci    r19,                  0
            sbci    r20,                  0
            brcc    delay_loop
            ret
main:       ldi     r16,                  0b111000    // Main starting point of program; define constant in r16
            out     _SFR_IO_ADDR(DDRB),   r16         // Set pins 11 through 13 to output using constant
infinity:   cbi     _SFR_IO_ADDR(PORTB),  5           // Infinite loop in main
            rcall   delay1s
            sbi     _SFR_IO_ADDR(PORTB),  5
            rcall   delay1s
            rjmp    infinity
