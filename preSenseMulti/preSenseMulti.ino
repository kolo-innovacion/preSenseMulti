class Sensor {
    int powPin;
    int pulPin;
    int value;
    int thresh;

  public:
    Sensor(int po, int pu) {
      powPin = po;
      pulPin = pu;
      value = 0;
      thresh = 0;

      pinMode(pulPin, INPUT);
      pinMode(powPin, OUTPUT);
    }

    void readValue() {
      digitalWrite(powPin, HIGH);

      int pulse = pulseIn(pulPin, HIGH);

      value = (pulse * 2.54) / 147;
      Serial.println(value);

      digitalWrite(powPin, LOW);
    }

};

Sensor sensorA(2, 3);
Sensor sensorB(4, 5);
Sensor sensorC(6, 7);

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorA.readValue();
  sensorB.readValue();
  sensorC.readValue();
}
