#define LEDPIN 13;
#define FORMULTIPLIER 445;

int prescaler = 0;

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

void my_delay(volatile long ms) {
  long maximum = ms * FORMULTIPLIER
  
  if(CLKPR > 0){
    maximum = maximum / pow(2,CLKPR);
  }
  
  for (volatile long i = 0; i < maximum; i++) { }
}

void setup() {
  setBit(&DDRB, 5, true); // Set led pin 13 as an output.

  // Turn off ADC
  ADCSRA = 0;

  // Turn off timers and Peripherals
  PRR = 0;
  
  Serial.begin(9600);
}

void setClockPrescaler(int newValue){
  Serial.println("Setting prescaler to :");
  Serial.println(newValue);

  //CLKPR |= 0;
  CLKPR = (1 << CLKPCE); // enable a change to CLKPR

  CLKPR = 1;
  //CLKPR |= newValue;//((byte) (log (newValue) / log (2)));

  Serial.println(((int) (log (newValue) / log (2))));
  Serial.println(CLKPR, BIN);
}

void loop() {
  //setBit(&PORTB, 5, true); // Set led pin 13 to HIGH.
  setClockPrescaler(4);
  my_delay(2000);
  //setBit(&PORTB, 5, false);  // Set led pin 13 to LOW.
  setClockPrescaler(8);
  my_delay(2000);
}

