#include <SoftwareSerial.h>


// Define pin numbers for ultrasonic sensors
const int trigPin1 = A1; // Trigger pin of first ultrasonic sensor
const int echoPin1 = A0; // Echo pin of first ultrasonic sensor
const int trigPin2 = A3; // Trigger pin of second ultrasonic sensor
const int echoPin2 = A2; // Echo pin of second ultrasonic sensor


// Define pin number for the buzzer
const int buzzerPin = 8;


// Define pin numbers for GPS module
const int gpsTx = 10; // TX pin of GPS module connected to D10
const int gpsRx = 11; // RX pin of GPS module connected to D11


// Define pin numbers for GSM module
const int gsmTx = 2; // TX pin of SIM800L connected to D2
const int gsmRx = 3; // RX pin of SIM800L connected to D3


// Define pin number for the push button
const int buttonPin = 6;


// SoftwareSerial objects for GPS and GSM communication
SoftwareSerial gpsSerial(gpsRx, gpsTx);
SoftwareSerial gsmSerial(gsmRx, gsmTx);


// Define phone number to send the SMS
String phoneNumber = "+919497632369"; // Change to your desired phone number


void setup() {
  // Initialize serial communication
  Serial.begin(9600);


  // Initialize ultrasonic sensor pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);


  // Initialize buzzer pin
  pinMode(buzzerPin, OUTPUT);


  // Initialize GPS module communication
  gpsSerial.begin(9600);


  // Initialize GSM module communication
  gsmSerial.begin(9600);


  // Initialize push button pin
  pinMode(buttonPin, INPUT_PULLUP);
}


void loop() {
  // Read distance from first ultrasonic sensor
  long distance1 = measureDistance(trigPin1, echoPin1);


  // Read distance from second ultrasonic sensor
  long distance2 = measureDistance(trigPin2, echoPin2);


  // Check obstacle distance and sound the buzzer accordingly
  if (distance1 >= 0 && distance1 < 10) {
    tone(buzzerPin, 1000); // Sound buzzer every 0.1 seconds
  } else if (distance1 >= 10 && distance1 < 20) {
    tone(buzzerPin, 1000); // Sound buzzer every 0.5 seconds
  } else if (distance1 >= 20 && distance1 <= 30) {
    tone(buzzerPin, 1000); // Sound buzzer every second
  } else {
    noTone(buzzerPin); // Turn off the buzzer if no obstacle
  }


  if (distance2 >= 0 && distance2 < 10) {
    tone(buzzerPin, 1000); // Sound buzzer every 0.1 seconds
  } else if (distance2 >= 10 && distance2 < 20) {
    tone(buzzerPin, 1000); // Sound buzzer every 0.5 seconds
  } else if (distance2 >= 20 && distance2 <= 30) {
    tone(buzzerPin, 1000); // Sound buzzer every second
  } else {
    noTone(buzzerPin); // Turn off the buzzer if no obstacle
  }


  // Check if the push button is pressed
  if (digitalRead(buttonPin) == LOW) {
    sendLocationSMS();
    delay(1000); // Delay to avoid multiple sends from button press
  }
}




// Function to measure distance using ultrasonic sensor
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}


// Function to send SMS
void sendSMS(String message) {
  // Set SMS text mode
  gsmSerial.println("AT+CMGF=1");
  delay(1000);


  // Send SMS
  gsmSerial.print("AT+CMGS=\"");
  gsmSerial.print(phoneNumber);
  gsmSerial.println("\"");
  delay(1000);
  gsmSerial.print(message);
}


// Function to send location SMS
void sendLocationSMS() {
  // Send command to GPS module to get latitude and longitude
  gpsSerial.println("AT+CGNSINF");
  delay(1000);


  // Read GPS module response
  String response = "";
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    response += c;
    if (response.indexOf("CGNSINF") >= 0) {
      break;
    }
  }


  // Extract latitude and longitude from GPS response
  int latIndex = response.indexOf(",", 14) + 1;
  int longIndex = response.indexOf(",", latIndex + 1) + 1;
  String latitude = response.substring(latIndex, latIndex + 9);
  String longitude = response.substring(longIndex, longIndex + 10);


  // Send SMS with location coordinates
  sendSMS("Your location: Latitude " + latitude + ", Longitude " + longitude);
}



