#define baudRate 9600
 
void setup() {    
 Serial.begin(baudRate);
 DDRB |= _BV(PINB2); // Make pin 10 an output
 PORTB |= _BV(PINB2); // Set HIGH for pin 10
 DDRD |= _BV(PORTD5); // Make pin 5 an output
}
 
void loop(){
 int readBit = bitRead(PINB, PINB2); // Read the current value

 if (readBit == LOW)
 {  
  PORTD |= _BV(PORTD5); // Power the lamp at pin 5
 }
 else
 {
  PORTD &= ~_BV(PORTD5); // Apply mask to set the 5th bit to 0
 }
}
