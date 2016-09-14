#define LEDPIN 13;
#define FORMULTIPLIER 445;

#include <avr/power.h>
#include <avr/sleep.h>

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

// NOTE : Unused At the moment.
void setPrescaler(int newValue){
  CLKPR = 0x80;             //Tell the chip we want to change system clock
  CLKPR = newValue;         //Set the prescaler
}


void my_delay(volatile long ms) {
  long maximum = ms * FORMULTIPLIER
  
  if(CLKPR > 0){
    maximum = maximum / pow(2,CLKPR);
  }
  
  for (volatile long i = 0; i < maximum; i++) { }
}

void setup() {
  setBit(&DDRB, 5, true);   // Set led pin 13 as an output.
  
  ADCSRA = 0;               // Turn off ADC.

  PRR = 0;                  // Turn off timers and Peripherals.
  
  Serial.begin(9600);
}

void loop() {
  
  setPrescaler(1);
  setBit(&PORTB, 5, true); // Set led pin 13 to HIGH.
  
  my_delay(2000);

  setPrescaler(4);
  setBit(&PORTB, 5, false);  // Set led pin 13 to LOW.
  
  my_delay(2000);

  setPrescaler(16);
  setBit(&PORTB, 5, true); // Set led pin 13 to HIGH.
  
  my_delay(2000);
  
  setPrescaler(256);
  setBit(&PORTB, 5, false);  // Set led pin 13 to LOW.
  
  my_delay(2000);
}

