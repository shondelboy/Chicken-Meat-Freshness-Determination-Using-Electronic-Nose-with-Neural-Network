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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);              //Initialise serial COM for displaying the value

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
}

void loop() {
  // put your main code here, to run repeatedly:
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

  double R0mq138 = getR0(mq138Value, 3.7, 20);
  double R0mq3 = getR0(mq3Value, 60, 200);
  double R0mq135 = getR0(mq135Value, 3.7, 20);
  double R0mq2 = getR0(mq2Value, 9.5, 5);
  double R0mq5 = getR0(mq5Value, 6.5, 20);
  double R0mq136 = getR0(mq136Value, 3.6, 20);
  double R0mq137  = getR0(mq137Value, 3.6, 47);
  double R0mq4 = getR0(mq4Value, 4.4, 20);
  double R0mq6 = getR0(mq6Value, 10, 20);
  double R0mq8 = getR0(mq8Value, 70, 10);

  Serial.print(R0mq138,5);
  Serial.print("\t");
  Serial.print(R0mq3,5);
  Serial.print("\t");
  Serial.print(R0mq135,5);
  Serial.print("\t");
  Serial.print(R0mq2,5);
  Serial.print("\t");
  Serial.print(R0mq5,5);
  Serial.print("\t");
  Serial.print(R0mq136,5);
  Serial.print("\t");
  Serial.print(R0mq137,5);
  Serial.print("\t");
  Serial.print(R0mq4,5);
  Serial.print("\t");
  Serial.print(R0mq6,5);
  Serial.print("\t");
  Serial.println(R0mq8,5);
}

double getR0 (double sensorValue, double RS0, double RL) {
  
  double sensorValueVolts_sum = 0;
  for(int test_cycle = 1 ; test_cycle <= 10000 ; test_cycle++) { //Read the analog output of the sensor for 10000 times
    double sensorValueVolts = (sensorValue * 5) / 1023;
    sensorValueVolts_sum = sensorValueVolts_sum + sensorValueVolts;
  }
  // calculate RS of air
  double sensorValueVolts = sensorValueVolts_sum/10000;
  double RS = ((5.0/sensorValueVolts)-1)*RL;
  double R0 = RS/RS0; // according to datasheet RS/R0 =  which is constant so..
  return R0;
}