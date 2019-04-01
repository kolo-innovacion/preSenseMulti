//INICIA CONFIGURACION

//TERMINA CONFIGURACION

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

//Brightsign gpio class definition:

bool active = true;
bool normal = false;

int gDelay = 500;

class Gport
{
    int pin;
    int ledPin;

  public:

    void setNormal() {
      digitalWrite(pin, normal);
      digitalWrite(ledPin, LOW);
    }

    Gport(int gp, int lp) {
      pin = gp;
      ledPin = lp;

      pinMode(pin, OUTPUT);
      pinMode(ledPin, OUTPUT);
      setNormal();
    }
    void setActive() {
      digitalWrite(pin, active);
      digitalWrite(ledPin, HIGH);
      delay(gDelay);
    }
    void setOutput(bool input) {
      if (input) {
        //in case Ball has been punched, Brightsign active value is set
        setActive();
        setNormal();
      } else {
        setNormal();
      }
    }

};

//sensor objects creation
Sensor sensorA(2, 3);
Sensor sensorB(4, 5);
Sensor sensorC(6, 7);

//BS GPIO port creation
Gport gport0(12, 13);

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorA.readValue();
  sensorB.readValue();
  sensorC.readValue();
}
