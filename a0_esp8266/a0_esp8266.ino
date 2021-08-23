/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

#define lenarr 1480
int sampleI[lenarr];  // value read from the pot

void setup() {
  // initialize serial communication at 115200
  Serial.begin(115200);
}


void loop() {
  // read the analog in value
  
  for (unsigned int n = 0; n < lenarr; n++)
  {
    sampleI[n] = analogRead(analogInPin);
    
    
    
  }
 
  // print the readings in the Serial Monitor

  for (unsigned int n = 0; n<lenarr; n++){
    Serial.print(sampleI[n]);
    Serial.print(", ");
    }
  Serial.println(" ");
  delay(5000);
    
}
