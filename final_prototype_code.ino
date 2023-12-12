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
const int dhtPin = A10; // Connect DHT11 to digital pin 10 in my case it put it on analog

DHT dht(dhtPin, DHT11);

float R0mq138 = 0;
float R0mq3 = 0;
float R0mq135 = 0;
float R0mq2 = 0;
float R0mq5 = 0;
float R0mq136 = 0;
float R0mq137  = 0;
float R0mq4 = 0;
float R0mq6 = 0;
float R0mq8 = 0;

void setup() {
  // put your setup code here, to run once:
  lcd.init();                      // Initialize the LCD
  lcd.backlight();                 // Turn on the backlight
  dht.begin();
  Serial.begin(9600);              //Initialise serial COM for displaying the value

  delay (1000);

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

  R0mq138 = getR0(mq138Value, 3.7);
  R0mq3 = getR0(mq3Value, 60);
  R0mq135 = getR0(mq135Value, 3.7);
  R0mq2 = getR0(mq2Value, 9.5);
  R0mq5 = getR0(mq5Value, 6.5);
  R0mq136 = getR0(mq136Value, 3.6);
  R0mq137  = getR0(mq137Value, 3.6);
  R0mq4 = getR0(mq4Value, 4.4);
  R0mq6 = getR0(mq6Value, 10);
  R0mq8 = getR0(mq8Value, 70);

  Serial.print("R0mq138\t");
  Serial.print("R0mq3\t");
  Serial.print("R0mq135\t");
  Serial.print("R0mq2\t");
  Serial.print("R0mq5\t");
  Serial.print("R0mq136\t");
  Serial.print("R0mq137\t");
  Serial.print("R0mq4\t");
  Serial.print("R0mq6\t");
  Serial.println("R0mq8");

  Serial.print(R0mq138, 3);
  Serial.print("\t");
  Serial.print(R0mq3, 3);
  Serial.print("\t");
  Serial.print(R0mq135, 3);
  Serial.print("\t");
  Serial.print(R0mq2, 3);
  Serial.print("\t");
  Serial.print(R0mq5, 3);
  Serial.print("\t");
  Serial.print(R0mq136, 3);
  Serial.print("\t");
  Serial.print(R0mq137, 3);
  Serial.print("\t");
  Serial.print(R0mq4, 3);
  Serial.print("\t");
  Serial.print(R0mq6, 3);
  Serial.print("\t");
  Serial.println(R0mq8, 3);
  
  Serial.print("C3H6O \t");
  Serial.print("C2H5OH \t");
  Serial.print("alcohol \t");
  Serial.print("NH3 \t");
  Serial.print("NH4 \t");
  Serial.print("CO2 \t");
  Serial.print("MQ2CH4 \t");
  Serial.print("MQ4CH4 \t");
  Serial.print("MQ6CH4 \t");
  Serial.print("MQ8H2 \t");
  Serial.print("MQ5H2 \t");
  Serial.print("MQ2H2 \t");
  Serial.print("H2S \t");
  Serial.print("Temperature\t");
  Serial.println("Humidity");
  
}

double getR0 (float sensorValue, float RS0) {
  
  float R0sum = 0;

  for(int test_cycle = 1 ; test_cycle <= 100 ; test_cycle++) { //Read the analog output of the sensor for 100 times

    float sensorValueVolts = (sensorValue * 5) / 1023; // convert to volts
    // calculate RS of air
    float RS = (5 - sensorValueVolts)/sensorValueVolts;
    float R0 = RS/RS0; // according to datasheet RS/R0 =  which is constant so..
    R0sum = R0sum + R0;
  }
  return R0sum/100;
}

void loop() {
  // put your main code here, to run repeatedly:
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
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Calculate gas concentrations (placeholders, replace with actual formulas)
  float acetoneConcentration = calculate(mq138Value, 0.82377337561, -0.24309198348, R0mq138);
  float ethanolConcentration = calculate(mq138Value, 0.76430992587, -0.31930911832, R0mq138);
  float alcoholConcentration = calculate(mq3Value, 0.47869088838, -0.65051499783, R0mq3);
  float ammoniaConcentration = calculate(mq137Value, 0.32342008711, -0.24352920263, R0mq137);
  float ammoniumConcentration = calculate(mq135Value, 0.99786419626, -0.42324587393, R0mq135);
  float carbondioxideConcentration = calculate(mq135Value, 0.9453422278, -0.36672579134, R0mq135);
  float MQ2methaneConcentration = calculate(mq2Value, 1.349158571, -0.372003751, R0mq2);
  float MQ4methaneConcentration = calculate(mq4Value, 1.07448668002, -0.3384580239, R0mq4);
  float MQ6methaneConcentration = calculate(mq6Value, 1.3335380011, -0.39794000867, R0mq6);
  float MQ8hydrogenGasConcentration = calculate(mq8Value, 5.98143535699, -1.44340257023, R0mq8);
  float MQ5hydrogenGasConcentration = calculate(mq5Value, 1.14142498046, -0.33619441272, R0mq5);
  float MQ2hydrogenGasConcentration = calculate(mq2Value, 1.48575946975, -0.48027201909, R0mq2);
  float hydrogensulfideConcentration = calculate(mq136Value, 0.42973563863, -0.27115996119, R0mq136);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ138");
  lcd.setCursor(0, 1);
  lcd.print("C3H6O: ");
  lcd.print(acetoneConcentration);
  lcd.print("ppm");
  Serial.print(acetoneConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ138");
  lcd.setCursor(0, 1);
  lcd.print("C2H5OH: ");
  lcd.print(ethanolConcentration);
  lcd.print("ppm");
  Serial.print(ethanolConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ3");
  lcd.setCursor(0, 1);
  lcd.print("alcohol: ");
  lcd.print(alcoholConcentration);
  lcd.print("ppm");
  Serial.print(alcoholConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ137");
  lcd.setCursor(0, 1);
  lcd.print("NH3: ");
  lcd.print(ammoniaConcentration);
  lcd.print("ppm");
  Serial.print(ammoniaConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ135");
  lcd.setCursor(0, 1);
  lcd.print("NH4: ");
  lcd.print(ammoniumConcentration);
  lcd.print("ppm");
  Serial.print(ammoniumConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ135");
  lcd.setCursor(0, 1);
  lcd.print("CO2: ");
  lcd.print(carbondioxideConcentration);
  lcd.print("ppm");
  Serial.print(carbondioxideConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ2");
  lcd.setCursor(0, 1);
  lcd.print("CH4: ");
  lcd.print(MQ2methaneConcentration);
  lcd.print("ppm");
  Serial.print(MQ2methaneConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ4");
  lcd.setCursor(0, 1);
  lcd.print("CH4: ");
  lcd.print(MQ4methaneConcentration);
  lcd.print("ppm");
  Serial.print(MQ4methaneConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ6");
  lcd.setCursor(0, 1);
  lcd.print("CH4: ");
  lcd.print(MQ6methaneConcentration);
  lcd.print("ppm");
  Serial.print(MQ6methaneConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ8");
  lcd.setCursor(0, 1);
  lcd.print("H2: ");
  lcd.print(MQ8hydrogenGasConcentration);
  lcd.print("ppm");
  Serial.print(MQ8hydrogenGasConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ5");
  lcd.setCursor(0, 1);
  lcd.print("H2: ");
  lcd.print(MQ5hydrogenGasConcentration);
  lcd.print("ppm");
  Serial.print(MQ5hydrogenGasConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ2");
  lcd.setCursor(0, 1);
  lcd.print("H2: ");
  lcd.print(MQ2hydrogenGasConcentration);
  lcd.print("ppm");
  Serial.print(MQ2hydrogenGasConcentration, 3);
  Serial.print("\t");

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MQ136");
  lcd.setCursor(0, 1);
  lcd.print("H2S: ");
  lcd.print(hydrogensulfideConcentration);
  lcd.print("ppm");
  Serial.print(hydrogensulfideConcentration, 3);
  Serial.print("\t");

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity);
  lcd.print("%");
  Serial.print(temperature, 3);
  Serial.print("\t");
  Serial.println(humidity, 3);
 // Adjust the delay as needed

 static int readingCounter = 0;

 readingCounter++;

 if (readingCounter == 10) {
    // Add your delay here (e.g., 1 second delay)
    Serial.println();
    // 1000 milliseconds = 1 second
    // Reset the counter to 0 for the next cycle
    readingCounter = 0;
  }

}

float calculate(float sensorValue, double b, double m, float Ro) {
  //get b & m value on datasheet
  float sensorValueVolts = (sensorValue * 5) / 1023;
  // calculate PPM of gas
  float RS = (5 - sensorValueVolts)/sensorValueVolts;
  float RS_R0_ratio = RS/Ro;
  float PPMinLog = (log10(RS_R0_ratio) - b)/m;
  float concentration = pow(10,PPMinLog);
  return concentration;
}