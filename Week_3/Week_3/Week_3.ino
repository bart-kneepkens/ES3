#define LEDPIN 13;
#define FORMULTIPLIER 445;

#include <avr/Power.h>

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
//  CLKPR = 0x80;            // Enable the change bit.
//  CLKPR |= newValue;        // Set the prescaler.
}


void my_delay(volatile long ms) {
  long maximum = ms * FORMULTIPLIER
  
  maximum = maximum / pow(2,CLKPR);
  
  for (volatile long i = 0; i < maximum; i++) {}
}

void setup() {
  setBit(&DDRB, 5, true);   // Set led pin 13 as an output.
  
  ADCSRA = 0;               // Turn off ADC.

  PRR = 0;                  // Turn off timers and Peripherals.
}

void blink(int delay){
  setBit(&PORTB, 5, true); // Set led pin 13 to HIGH.
  my_delay(delay);
  setBit(&PORTB, 5, false);  // Set led pin 13 to LOW.
  my_delay(delay);
}

void loop() {
   clock_prescale_set(clock_div_256);
   blink(2000);
//   clock_prescale_set(clock_div_2);
//   blink();
//   clock_prescale_set(clock_div_4);
//   blink();
//   clock_prescale_set(clock_div_8);
//   blink();
//   clock_prescale_set(clock_div_16);
//   blink();
//   clock_prescale_set(clock_div_32);
//   blink();
//   clock_prescale_set(clock_div_64);
//   blink();
//   clock_prescale_set(clock_div_128);
//   blink();
//   clock_prescale_set(clock_div_256);
}

