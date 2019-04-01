//INICIA CONFIGURACION
int threshA = 40;
int threshB = 40;
int threshC = 40;
//TERMINA CONFIGURACION

class Sensor {
    int powPin;
    int pulPin;
    int value;
    int thresh;
    bool state;

  public:
    Sensor(int po, int pu, int th) {
      powPin = po;
      pulPin = pu;
      thresh = th;
      value = 0;
      state = false;

      pinMode(pulPin, INPUT);
      pinMode(powPin, OUTPUT);
    }

    void readValue() {
      digitalWrite(powPin, HIGH);

      int pulse = pulseIn(pulPin, HIGH);

      value = int((pulse * 2.54) / 147);

      if (value < 1) {
        value = 0;
      }

      Serial.println(value);

      digitalWrite(powPin, LOW);
    }
    void updateState() {
      if ((value > 1) && (value < thresh)) {
        state = true;
      } else {
        state = false;
      }
    }
    bool getState() {
      return state;
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
    void blinkOnce() {
      digitalWrite(ledPin, HIGH);
      delay(250);
      digitalWrite(ledPin, LOW);
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
Sensor sensorA(2, 3, threshA);
Sensor sensorB(4, 5, threshB);
Sensor sensorC(6, 7, threshC);

//BS GPIO port creation
Gport gport0(12, 13);

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensorA.readValue();
  sensorB.readValue();
  sensorC.readValue();

  sensorA.updateState();
  sensorB.updateState();
  sensorC.updateState();

  if (sensorA.getState() || sensorB.getState() || sensorC.getState()) {
    gport0.setActive();
    //gport0.blinkOnce();
  } else {}

}
