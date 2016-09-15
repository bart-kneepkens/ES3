#define FORMULTIPLIER 445;

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

void setup() {
  DDRB |= (1<<DDB5);        // Set led pin 13 as an output.
  ADCSRA = 0;               // Turn off ADC.
  PRR = 0;                  // Turn off timers and Peripherals.
}

void blink(int delay){
  PORTB |= (1<<PORTB5);       // Set led pin 13 to HIGH.
  my_delay(delay);
  PORTB &= ~(1<<PORTB5);      // Set led pin 13 to LOW.
  my_delay(delay);
}

void loop() {
   blink(2000);
   blink(2000);
   setPrescaler(8);
}

