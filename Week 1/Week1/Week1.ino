#define baudRate 9600
 
void setup() {    
 Serial.begin(baudRate);
 DDRB |= _BV(PINB2); // Make pin 10 an output
 PORTB |= _BV(PINB2); // Set HIGH for pin 10

 DDRB |= _BV(PINB4); // Make pin 12 an output
 PORTB |= _BV(PINB4); // Set HIGH for pin 12

 DDRB |= _BV(PINB3); // Make pin 12 an output
 PORTB |= _BV(PINB3); // Set HIGH for pin 12

 DDRD |= _BV(PINB3); // Buzzer
  


 DDRD |= _BV(PORTD6); // Make pin 6 an output
 DDRD |= _BV(PORTD5); // Make pin 5 an output
}
 
void loop(){
  int stateOfButton11 = bitRead(PINB, PINB3); // Read the current value

  if(stateOfButton11 != LOW){
  
 int stateOfButton10 = bitRead(PINB, PINB2); // Read the current value

 if (stateOfButton10 == LOW)
 {  
  PORTD |= _BV(PORTD5); // Power the lamp at pin 5
 }
 else
 {
  PORTD &= ~_BV(PORTD5); // Apply mask to set the 5th bit to 0
 }

 int stateOfButton12 = bitRead(PINB, PINB4); // Read the current value

 if (stateOfButton12 == LOW)
 {  
  PORTD |= _BV(PORTD6); // Power the lamp at pin 5
  Serial.println("Hello, world!");
 }
 else
 {
  PORTD &= ~_BV(PORTD6); // Apply mask to set the 5th bit to 0
 }

int stateOfButton11 = bitRead(PINB, PINB3); // Read the current value



 if(stateOfButton10 == LOW && stateOfButton12 == LOW){
  
  PORTD |= _BV(PORTD3);
  delay(5);
  PORTD &= ~_BV(PORTD3);
  delay(5);
  
 }
  }
 
}
