#include "DHT.h"  // required library for the commands used below

#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 
// RGB LED output ports
#define BLUE 3
#define GREEN 5
#define RED 6

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

int POTPin = A0;  // potentionmeter input at pin A0;
int POTreadvalue; // read value variable
int DataFreq1;     // user defined rate of data collection
int DataFreq2;     // rate of DHT data collection
float Voltage;      // true voltage value across potentionmeter
int redValue;       // rgb RED value
int greenValue;     // rgb green value
int blueValue;      // rgb BLUE value
int ColourCycle = 255;

void setup() {

  // set pins to output
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // write to pins
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  
  // put your setup code here, to run once:
  pinMode(POTPin, INPUT);
  Serial.begin(9600); 

  Serial.println("Potentionmeter test!");

  Serial.println("Please enter Frequency of data collection");
  Serial.println("Values of 100+ are suitable");
  while(Serial.available()==0){
        // makes the code wait until a number is entered 
  }
  DataFreq1 = Serial.parseInt();
  DataFreq2 = 5 * DataFreq1;
  dht.begin(); 
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(DataFreq2);

  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }


  POTreadvalue = analogRead(POTPin);
  Voltage = POTreadvalue/204.6;
  //  Serial.println(Voltage);
  delay(DataFreq1);

  if ( Voltage > 4.00){

      Serial.print("Humidity: "); 
      Serial.print(h);
      Serial.println(" %\t");

      blueValue = Voltage * 51;
      analogWrite(BLUE, blueValue);
      analogWrite(GREEN, 0);
      analogWrite(RED, 0);
      delay(DataFreq2);
  }
  if ( Voltage < 1.00){

      Serial.print("Temperature: "); 
      Serial.print(t);
      Serial.println(" C\t");
      
      greenValue = Voltage * 51;
      analogWrite(GREEN, greenValue);
      analogWrite(RED, 0);
      analogWrite(BLUE, 0);
      delay(DataFreq2);
  }
  if ( Voltage > 1.00 && Voltage < 4.00){

      redValue = Voltage * 51;
      analogWrite(RED, redValue);
      analogWrite(GREEN, 0);
      analogWrite(BLUE, 0);
      delay(DataFreq1);
  }
  
}
