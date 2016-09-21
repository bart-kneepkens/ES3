.global main
main:       ldi     r16,          0b1110000   // Define constant in r16
            out     0x0a,         r16         // Set pins 11 through 13 to output using constant (0x04 is DDRB)
reset:      ldi     r16,          0b10000     // Reset r16 to starting value
main_loop:  lsl     r16                       // Shift r16 to the left
            brcs    reset                     // If r16 shifted off the left (we have carry flag set) then reset
            out     0x0b,         r16         // Push value of r16 to led lights (0x05 is PORTB)
            ldi     r18,          255         // Set r18 to max value
            ldi     r19,          211         // Set r19 to 211
            ldi     r20,          48          // Set r20 to 48
delay_loop: subi    r18,          1           // Subtract 1 from r18
            sbci    r19,          0           // Subtract possible previous carry from r19
            sbci    r20,          0           // Subtract possible previous carry from r20
            brcc    delay_loop                // Keep on looping if carry was cleared
            rjmp    main_loop                 // Return to main loop
