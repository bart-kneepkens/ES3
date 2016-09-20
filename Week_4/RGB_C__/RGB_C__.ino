#define redPin 11
#define greenPin 12
#define bluePin 13
 
void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);  
}
 
void loop()
{
  setColor(0, 255, 255);  // red
  delay(1000);
  setColor(255, 0, 255);  // green
  delay(1000);
  setColor(255, 255, 0);  // blue
  delay(1000);
}
 
void setColor(int red, int green, int blue)
{
  digitalWrite(redPin, red);
  digitalWrite(greenPin, green);
  digitalWrite(bluePin, blue);  
}
