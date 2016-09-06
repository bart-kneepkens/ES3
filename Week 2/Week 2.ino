// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>

#define BUTTON11 11
#define LEDPIN5 5
#define LEDPIN6 6
#define MINCOUNT 1562
#define MAXCOUNT 15624
#define SLIDERMAX 1023

int lastSliderValue; // last value from the slider
const int minSliderDiff = 50; // minimum difference for slider value

ISR(TIMER1_COMPA_vect) {
    digitalWrite(LEDPIN5, !digitalRead(LEDPIN5));
}

ISR (PCINT0_vect)
{
    digitalWrite(LEDPIN6, !digitalRead(LEDPIN6));
    delay(100);
}


void setup()
{
    Serial.begin(9600);
    pinMode(BUTTON11, INPUT);
    digitalWrite(BUTTON11, HIGH);
    pinMode(LEDPIN5, OUTPUT);
    pinMode(LEDPIN6, OUTPUT);
 
    // initialize Timer1
    cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
 
    // set compare match register to desired timer count:
    OCR1A = MINCOUNT;
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);
    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);

    // initialize BUTTON11
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT3);
    
    sei(); // enable global interrupts
}
 
void loop()
{
    cli(); // disable global interrupts
    int currentSliderValue = analogRead(2);

    // If significant diff in slider value, update timer maximum count.
    if (abs(currentSliderValue - lastSliderValue) >= minSliderDiff) {
      int newMaxCount = map(currentSliderValue, 0, SLIDERMAX, MINCOUNT, MAXCOUNT);   
      OCR1A = newMaxCount;

      // If the timer value is higher than the new maximum count, then reset the timer to 0.
      if (TCNT1 >= newMaxCount) {
        TCNT1 = 0;
        digitalWrite(LEDPIN5, !digitalRead(LEDPIN5));
      }      
      lastSliderValue = currentSliderValue;
    }
    sei(); // enable global interrupts:
}
