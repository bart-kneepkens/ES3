#define baudRate 9600

// Sets a bit in a register.
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

// Sets a bit in a DDR and a PORT to 1.
void setToOutputAndSetHigh(volatile unsigned char* ddr, volatile unsigned char* port, int bitNr)
{
  setBit(ddr, bitNr, true);
  setBit(port, bitNr, true);
}

// Returns true if the bit in the register is false.
boolean bitIsTrue(volatile unsigned char* reg, int bitNr)
{
  return bitRead(*reg, bitNr) != HIGH;
}

void setup()
{
  Serial.begin(baudRate);

  // Set the buttons to outputs and their signals to high.
  setToOutputAndSetHigh(&DDRB, &PORTB, PINB2);
  setToOutputAndSetHigh(&DDRB, &PORTB, PINB3);
  setToOutputAndSetHigh(&DDRB, &PORTB, PINB4);

  // Makes the buzzer and lamps outputs, because this
  // somehow improves the signal strength.
  setBit(&DDRD, PORTD3, true); // Buzzer
  setBit(&DDRD, PORTD5, true); // LED on pin 5
  setBit(&DDRD, PORTD6, true); // LED on pin 6
}

void loop()
{
  // If button D11 is not pressed, do stuff.
  if (!bitIsTrue(&PINB, PINB3))
  {
    // Set lamp D5.
    bool buttonD10IsPressed = bitIsTrue(&PINB, PINB2);
    setBit(&PORTD, PORTD5, buttonD10IsPressed);

    // Set lamp D6.
    bool buttonD12IsPressed = bitIsTrue(&PINB, PINB4);
    setBit(&PORTD, PORTD6, buttonD12IsPressed);

    // If button D12 is pressed, print hello world.
    if (buttonD12IsPressed)
    {
      Serial.println("Hello, world!");

      // If button D10 is also pressed, make buzz sound.
      if (buttonD10IsPressed)
      {
        setBit(&PORTD, PORTD3, true);
        delay(5);
        setBit(&PORTD, PORTD3, false);
      }
    }
  }
  else
  {
    // Turn off lamps.
    setBit(&PORTD, PORTD5, false);
    setBit(&PORTD, PORTD6, false);
  }
}
