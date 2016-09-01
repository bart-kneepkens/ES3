#define baudRate 9600
#define DDR_BUTTONS &DDRB
#define PORT_BUTTONS &PORTB
#define PIN_BUTTONS &PINB
#define DDR_LAMPS &DDRD
#define PORT_LAMPS &PORTD

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
  // This is needed because of the Danger shield we use, which does not have a pull-up resistor.
  setToOutputAndSetHigh(DDR_BUTTONS, PORT_BUTTONS, PORTB2);
  setToOutputAndSetHigh(DDR_BUTTONS, PORT_BUTTONS, PORTB3);
  setToOutputAndSetHigh(DDR_BUTTONS, PORT_BUTTONS, PORTB4);

  // Makes the buzzer and lamps outputs, because this
  // somehow improves the signal strength.
  setBit(DDR_LAMPS, PORTD3, true); // Buzzer
  setBit(DDR_LAMPS, PORTD5, true); // LED on pin 5
  setBit(DDR_LAMPS, PORTD6, true); // LED on pin 6
}

void loop()
{
  // If button D11 is not pressed, do stuff.
  if (!bitIsTrue(PIN_BUTTONS, PINB3))
  {
    // Set lamp D5.
    bool buttonD10IsPressed = bitIsTrue(PIN_BUTTONS, PINB2);
    setBit(PORT_LAMPS, PORTD5, buttonD10IsPressed);

    // Set lamp D6.
    bool buttonD12IsPressed = bitIsTrue(PIN_BUTTONS, PINB4);
    setBit(PORT_LAMPS, PORTD6, buttonD12IsPressed);

    // If button D12 is pressed, print hello world.
    if (buttonD12IsPressed)
    {
      Serial.println("Hello, world!");

      // If button D10 is also pressed, make buzz sound.
      if (buttonD10IsPressed)
      {
        setBit(PORT_LAMPS, PORTD3, true);
        delay(5);
        setBit(PORT_LAMPS, PORTD3, false);
      }
    }
  }
  else
  {
    // It could be that one of the buttons (D10 or D12) was pressed prior, but now button D11 has been pressed.
    // Turn off lamps.
    setBit(PORT_LAMPS, PORTD5, false);
    setBit(PORT_LAMPS, PORTD6, false);
  }
}
