#define FORMULTIPLIER 445;
#include <avr/sleep.h>

/*  NOTE: Call with values 1-8  */
void setPrescaler(int newValue){ 
  CLKPR = 0x80;            // Enable the change bit.
  CLKPR = newValue;        // Set the prescaler.
}

void my_delay(volatile long ms) {
  long maximum = ms * FORMULTIPLIER;
  maximum = maximum / pow(2,CLKPR);               // Since the CLKPR register is only 1-8, we have to divide by the CLKPR'th power of 2.
  for (volatile long i = 0; i < maximum; i++) {}  // Empty for-loop. (Busy waiting).
}

ISR(WDT_vect){}             // The cpu_sleep is disabled by default when ISR is called.

void setup() {
  DDRB |= (1<<DDB5);        // Set led pin 13 as an output.
  ADCSRA = 0;               // Turn off ADC.
  PRR = 0;                  // Turn off timers and Peripherals.

  cli();
  WDTCSR = (1<<4)|(1<<3);         //(B00011000) Set up WDT interrupt.
  WDTCSR = (1<<6)|(1<<3)|(1<<5);  //(B01101000) Start watchdog timer with 4 second prescaler.
  sei();
}

void blink(int delay){
  PORTB |= (1<<PORTB5);       // Set led pin 13 to HIGH.
  my_delay(delay);
  PORTB &= ~(1<<PORTB5);      // Set led pin 13 to LOW.
  my_delay(delay);
}

void loop() {
  blink(500);
  blink(500);
  SMCR |= (1<<2)|(1<<0);      // Power-down mode. 
  sleep_cpu();  
}

