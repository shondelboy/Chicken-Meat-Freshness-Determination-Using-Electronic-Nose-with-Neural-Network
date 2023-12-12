#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the I2C address and LCD size (16x2)

// Define the pins for each sensor
const int mq138Pin = A0;
const int mq3Pin = A1;
const int mq135Pin = A2;
const int mq2Pin = A3;
const int mq5Pin = A4;
const int mq136Pin = A5;
const int mq137Pin = A6;
const int mq4Pin = A7;
const int mq6Pin = A8;
const int mq8Pin = A9;
const int dhtPin = A10;             // Connect DHT11 to digital pin 10 in my case it put it on analog

DHT dht(dhtPin, DHT11);

const int buttonPin = 53;           // The pin where the button is connected
const int blueLedPin = 51;          // The pin for the blue LED
const int greenLedPin = 49;         // The pin for the green LED
const int redLedPin = 46;           // The pin for the red LED

bool buttonState = false;           // Current state of the button
bool lastButtonState = false;       // Previous state of the button
bool sendDataFlag = false;          // Flag to indicate when to send data
unsigned long sendDataStartTime;    // Time when data sending started

char receivedData[8];  // Assuming a maximum word length of 7 characters

void setup() {
  // put your setup code here, to run once:
  lcd.init();                        // Initialize the LCD
  lcd.backlight();                   // Turn on the backlight
  dht.begin();
  Serial.begin(9600);                // Initialise serial COM for displaying the value
  
  pinMode(buttonPin, INPUT_PULLUP);  // Enable internal pull-up resistor for the button
  pinMode(blueLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  delay(1000);
}

void loop() {
  // Read the state of the button
  buttonState = digitalRead(buttonPin);

  // Check if the button is pressed (LOW)
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Button is pressed, turn on the blue LED
    digitalWrite(blueLedPin, HIGH);
    sendDataFlag = true;  // Set the flag to start sending data
    sendDataStartTime = millis();  // Record the start time for sending data
  }

  // Check if the button is released (HIGH)
  if (buttonState == HIGH && lastButtonState == LOW) {
    // Button is released, turn off the blue LED
    digitalWrite(blueLedPin, LOW);
  }

  // Check if the flag is set to send data and 5 seconds haven't passed yet
  if (sendDataFlag && millis() - sendDataStartTime < 5000) {
    // Read data from gas sensors and DHT11
    float mq138Value = analogRead(mq138Pin);
    float mq3Value = analogRead(mq3Pin);
    float mq135Value = analogRead(mq135Pin);
    float mq2Value = analogRead(mq2Pin);
    float mq5Value = analogRead(mq5Pin);
    float mq136Value = analogRead(mq136Pin);
    float mq137Value = analogRead(mq137Pin);
    float mq4Value = analogRead(mq4Pin);
    float mq6Value = analogRead(mq6Pin);
    float mq8Value = analogRead(mq8Pin);

    // Calculate gas concentrations (placeholders, replace with actual formulas)
    double acetoneConcentration = calculate(mq138Value, 0.82377337561, -0.24309198348, 74.74160, 20);
    double ethanolConcentration = calculate(mq138Value, 0.76430992587, -0.31930911832, 74.74160, 20);
    double alcoholConcentration = calculate(mq3Value, 0.47869088838, -0.65051499783, 48.33522, 200);
    double ammoniaConcentration = calculate(mq137Value, 0.32342008711, -0.24352920263, 56.86693, 47);
    double ammoniumConcentration = calculate(mq135Value, 0.99786419626, -0.42324587393, 65.47914, 20);
    double carbondioxideConcentration = calculate(mq135Value, 0.9453422278, -0.36672579134, 65.47914, 20);
    double MQ2methaneConcentration = calculate(mq2Value, 1.349158571, -0.372003751, 3.68012, 5);
    double MQ4methaneConcentration = calculate(mq4Value, 1.07448668002, -0.3384580239, 12.87112, 20);
    double MQ6methaneConcentration = calculate(mq6Value, 1.3335380011, -0.39794000867, 45.57796, 20);
    double MQ8hydrogenGasConcentration = calculate(mq8Value, 5.98143535699, -1.44340257023, 2.77994, 10);
    double MQ5hydrogenGasConcentration = calculate(mq5Value, 1.14142498046, -0.33619441272, 14.70573, 20);
    double MQ2hydrogenGasConcentration = calculate(mq2Value, 1.48575946975, -0.48027201909, 3.68012, 5);
    double hydrogensulfideConcentration = calculate(mq136Value, 0.42973563863, -0.27115996119, 23.89024, 20);
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    sendData(acetoneConcentration, ethanolConcentration, alcoholConcentration, ammoniaConcentration, ammoniumConcentration, carbondioxideConcentration, MQ2methaneConcentration, MQ4methaneConcentration, MQ6methaneConcentration, MQ8hydrogenGasConcentration, MQ5hydrogenGasConcentration, MQ2hydrogenGasConcentration, hydrogensulfideConcentration, temperature, humidity);
  
    // Delay to control the sending rate (adjust as needed)
    delay(1000);
  } else {
    // 5 seconds have passed or the flag is not set, reset the flag
    sendDataFlag = false;
  }

  // Read the freshness classification result from Python
  if (Serial.available() > 0) {
    Serial.readBytesUntil('\n', receivedData, sizeof(receivedData) - 1);  // Read data into the buffer
    receivedData[sizeof(receivedData) - 1] = '\0';  // Null-terminate the string
    displayLCDandLEDs(receivedData);
  } else {
    // No serial data received, turn off the LEDs
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, LOW);
  }

  // Update the last button state
  lastButtonState = buttonState;
}

void sendData(float acetone, float ethanol, float alcohol, float ammonia, float ammonium, float carbondioxide, float MQ2methane, float MQ4methane, float MQ6methane, float MQ8hydrogenGas, float MQ5hydrogenGas, float MQ2hydrogenGas, float hydrogensulfide, float temperature, float humidity) {
  Serial.print(acetone, 5);
  Serial.print("\t");
  Serial.print(ethanol, 5);
  Serial.print("\t");
  Serial.print(alcohol, 5);
  Serial.print("\t");
  Serial.print(ammonia, 5);
  Serial.print("\t");
  Serial.print(ammonium, 5);
  Serial.print("\t");
  Serial.print(carbondioxide, 5);
  Serial.print("\t");
  Serial.print(MQ2methane, 5);
  Serial.print("\t");
  Serial.print(MQ4methane, 5);
  Serial.print("\t");
  Serial.print(MQ6methane, 5);
  Serial.print("\t");
  Serial.print(MQ8hydrogenGas, 5);
  Serial.print("\t");
  Serial.print(MQ5hydrogenGas, 5);
  Serial.print("\t");
  Serial.print(MQ2hydrogenGas, 5);
  Serial.print("\t");
  Serial.print(hydrogensulfide, 5);
  Serial.print("\t");
  Serial.print(temperature, 1);
  Serial.print("\t");
  Serial.println(humidity, 1);
}

double calculate(double sensorValue, double b, double m, double RO, double RL) {
  double sensorValueVolts_sum = 0;
  for(int test_cycle = 1 ; test_cycle <= 10 ; test_cycle++) { //Read the analog output of the sensor for 10 times
    double sensorValueVolts = (sensorValue * 5) / 1023;
    sensorValueVolts_sum = sensorValueVolts_sum + sensorValueVolts;
  }
  //get b & m value on datasheet
  // calculate PPM of gas
  double sensorValueVolts = sensorValueVolts_sum/10;
  double RS = ((5*RL)/sensorValueVolts)-RL;
  double RS_RO_ratio = RS/RO;
  double PPMinLog = (log10(RS_RO_ratio) - b)/m;
  double concentration = pow(10,PPMinLog);
  return concentration;
}

void controlLEDs(char* classification) {
  // Compare the received classification and control LEDs accordingly
  switch (classification[0]) {
    case 'F':
      // Fresh classification, turn on the green LED
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(redLedPin, LOW);
      break;
    case 'R':
      // Rancid classification, turn on both green and red LEDs
      digitalWrite(greenLedPin, HIGH);
      digitalWrite(redLedPin, HIGH);
      break;
    case 'S':
      // Spoiled classification, turn on the red LED
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, HIGH);
      break;
    default:
      // Handle unexpected classification
      digitalWrite(greenLedPin, LOW);
      digitalWrite(redLedPin, LOW);
  }
}

void displayLCDandLEDs(char* receivedData) {
  // Clear the LCD
  lcd.clear();

  // Display the received data on the LCD
  lcd.setCursor(0, 0);  // Set the cursor to the first row, first column
  lcd.print(receivedData);

  // Check the received classification and control LEDs
  controlLEDs(receivedData);

  delay(1000);  // Display the data for 2 seconds (adjust as needed)
      
  // Clear the LCD again
  lcd.clear();
}
