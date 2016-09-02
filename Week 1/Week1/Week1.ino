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
boolean bitIsFalse(volatile unsigned char* reg, int bitNr)
{
  return ((*reg >> bitNr) & 0b1) == 0;
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
  // Check button presses.
  bool buttonD10IsPressed = bitIsFalse(PIN_BUTTONS, PINB2);
  bool buttonD11IsPressed = bitIsFalse(PIN_BUTTONS, PINB3);
  bool buttonD12IsPressed = bitIsFalse(PIN_BUTTONS, PINB4);

  // According to button presses, take action.
  if (buttonD11IsPressed)
  {
    // It could be that one of the buttons (D10 or D12) was pressed prior, but now button D11 has been pressed.
    // Turn off lamps.
    setBit(PORT_LAMPS, PORTD5, false);
    setBit(PORT_LAMPS, PORTD6, false);
  }
  else if (buttonD10IsPressed && !buttonD12IsPressed)
  {
    // Set lamps.
    setBit(PORT_LAMPS, PORTD5, true);
    setBit(PORT_LAMPS, PORTD6, false);
  }
  else if (!buttonD10IsPressed && buttonD12IsPressed)
  {
    // Set lamps.
    setBit(PORT_LAMPS, PORTD5, false);
    setBit(PORT_LAMPS, PORTD6, true);
    Serial.println("Hello, world!");  // Print 'Hello, world!'.
  }
  else if (buttonD10IsPressed && buttonD12IsPressed)
  {
    // Set lamps.
    setBit(PORT_LAMPS, PORTD5, false);
    setBit(PORT_LAMPS, PORTD6, false);

    // Make buzz sound.
    setBit(PORT_LAMPS, PORTD3, true);
    delay(5);
    setBit(PORT_LAMPS, PORTD3, false);
  }
  else
  {
    // If nothing is pressed, disable lamps.
    setBit(PORT_LAMPS, PORTD5, false);
    setBit(PORT_LAMPS, PORTD6, false);
  }
}
