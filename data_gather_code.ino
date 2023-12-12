#include <Wire.h>
#include <DHT.h>

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

void setup() {
  // put your setup code here, to run once:
  dht.begin();
  Serial.begin(9600);              //Initialise serial COM for displaying the value

  delay (1000);

}


void loop() {
  // put your main code here, to run repeatedly:
  // Read data from gas sensors and DHT11
  double mq138Value = analogRead(mq138Pin);
  double mq3Value = analogRead(mq3Pin);
  double mq135Value = analogRead(mq135Pin);
  double mq2Value = analogRead(mq2Pin);
  double mq5Value = analogRead(mq5Pin);
  double mq136Value = analogRead(mq136Pin);
  double mq137Value = analogRead(mq137Pin);
  double mq4Value = analogRead(mq4Pin);
  double mq6Value = analogRead(mq6Pin);
  double mq8Value = analogRead(mq8Pin);
  double temperature = dht.readTemperature();
  double humidity = dht.readHumidity();

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

  Serial.print(acetoneConcentration,5);
  Serial.print("\t");

  Serial.print(ethanolConcentration,5);
  Serial.print("\t");

  Serial.print(alcoholConcentration,5);
  Serial.print("\t");

  Serial.print(ammoniaConcentration,5);
  Serial.print("\t");

  Serial.print(ammoniumConcentration,5);
  Serial.print("\t");

  Serial.print(carbondioxideConcentration,5);
  Serial.print("\t");

  Serial.print(MQ2methaneConcentration,5);
  Serial.print("\t");

  Serial.print(MQ4methaneConcentration,5);
  Serial.print("\t");

  Serial.print(MQ6methaneConcentration,5);
  Serial.print("\t");

  Serial.print(MQ8hydrogenGasConcentration,5);
  Serial.print("\t");

  Serial.print(MQ5hydrogenGasConcentration,5);
  Serial.print("\t");

  Serial.print(MQ2hydrogenGasConcentration,5);
  Serial.print("\t");

  Serial.print(hydrogensulfideConcentration,5);
  Serial.print("\t");

  Serial.print(temperature,1);
  Serial.print("\t");
  Serial.println(humidity,1);

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