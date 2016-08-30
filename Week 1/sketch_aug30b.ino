#define baudRate 9600
 
void setup() {    
 Serial.begin(baudRate);

 // Make pin 10 an output
 DDRB |= _BV(PINB2);

 // Set HIGH for pin 10
 PORTB |= _BV(PINB2);

 // Make pin 5 an output
  DDRD |= _BV(PORTD5);
}
 
void loop(){

 // Read the current value
 int readBit = bitRead(PINB, PINB2);

 if (readBit == LOW)
 {  
  // Power the lamp at pin 5
  PORTD |= _BV(PORTD5);
 }
 else
 {
  // IS MASKER!!!! 
  PORTD &= B11011111;
 }
}
