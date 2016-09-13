#define LEDPIN 13;
#define FORMULTIPLIER 445;

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
  long maximum = ms * FORMULTIPLIER;
  for (volatile long i = 0; i < maximum; i++) { }
}

void setup() {
  setBit(&DDRB, 5, true); // Set led pin 13 as an output.
  Serial.begin(9600);
}

void loop() {
  setBit(&PORTB, 5, true); // Set led pin 13 to HIGH.
  my_delay(1000);
  //delay(2000);
  setBit(&PORTB, 5, false);  // Set led pin 13 to LOW.
  //delay(2000);
  my_delay(1000);
}

