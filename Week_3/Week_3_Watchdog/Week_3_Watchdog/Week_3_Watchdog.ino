#define LEDPIN 13;
#define FORMULTIPLIER 445;
#include <avr/wdt.h>
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
  Serial.println("ISR");
  } // The cpu_sleep is disabled by default when ISR is called.

void setup() {
  Serial.begin(9600);
  Serial.println("Setup");
  setBit(&DDRB, 5, true);   // Set led pin 13 as an output.
  
  ADCSRA = 0;               // Turn off ADC.

  PRR = 0;                  // Turn off timers and Peripherals.

  /** Set the Watchdog interrupt and watchdog prescaler at 4sec **/
  cli();
  
  WDTCSR = (1<<4)|(1<<3);         //(B00011000) Set up WDT interrupt.
  
  WDTCSR = (1<<6)|(1<<3)|(1<<5);  //(B01101000) Start watchdog timer with 4 second prescaler.
 
  sei();
}

// NOTE : This doesn't work :(
void feedDog(){
  MCUSR &= ~(0<<3);
  WDTCSR &= ~(0<<3);
}

void loop() {
  
  //feedDog();
  wdt_reset();
  
  /*  Blink 2 times.  */
  setBit(&PORTB, 5, true);  // Set LED pin 13 to HIGH.
  my_delay(500);
  setBit(&PORTB, 5, false); // Set LED pin 13 to LOW.
  my_delay(500);
  setBit(&PORTB, 5, true);  // Set LED pin 13 to HIGH.
  my_delay(500);
  setBit(&PORTB, 5, false); // Set LED pin 13 to LOW.
  my_delay(500);

  /*  Power down mode.  */
  SMCR |= (1<<2)|(1<<0);
  sleep_cpu();
}

