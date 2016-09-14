#define LEDPIN 13;
#define FORMULTIPLIER 445;
#include <avr/wdt.h>

void setBit(volatile unsigned char* reg, int bitNr, bool value)
{
  if (value)
  {
    *reg |= _BV(bitNr);
  }
  else
  {
    *reg &= ~_BV(bitNr);
  } 
}

void setPrescaler(int newValue){
  CLKPR = 0x80;            // Enable the change bit.
  CLKPR = newValue;        // Set the prescaler.
}


void my_delay(volatile long ms) {
  long maximum = ms * FORMULTIPLIER
  
  maximum = maximum / pow(2,CLKPR);
  
  for (volatile long i = 0; i < maximum; i++) {}
}

ISR(WDT_vect){
  Serial.println("Watchdoge");
}

void setup() {
  Serial.begin(9600);
  
  setBit(&DDRB, 5, true);   // Set led pin 13 as an output.
  
  ADCSRA = 0;               // Turn off ADC.

  PRR = 0;                  // Turn off timers and Peripherals.


  /** Set the Watchdog interrupt and watchdog prescaler at 4sec **/
  cli();
  //wdt_reset();
  
  WDTCSR = (1<<4)|(1<<3); // B00011000    // Set up WDT interrupt.
  
  WDTCSR = (1<<6)|(1<<3)|(1<<5);  // B01101000; // Start watchdog timer with 4sec prescaler.
  sei();
}

void loop() {
  setPrescaler(1);
  setBit(&PORTB, 5, true);  // Set LED pin 13 to HIGH.
  
  my_delay(2000);

  setPrescaler(4);
  setBit(&PORTB, 5, false); // Set LED pin 13 to LOW.
  
  my_delay(2000);

  setPrescaler(16);
  setBit(&PORTB, 5, true);  // Set LED pin 13 to HIGH.
  
  my_delay(2000);
  
  setPrescaler(256);
  setBit(&PORTB, 5, false); // Set LED pin 13 to LOW.
  
  my_delay(2000);
}

