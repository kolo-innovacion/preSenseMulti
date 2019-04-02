//INICIA CONFIGURACION
int threshA = 40;
int threshB = 40;
int threshC = 40;
//TERMINA CONFIGURACION

class Pot {
    int volPin;
    int value;

  public:
    Pot(int pin) {
      volPin = pin;
      value = 0;
    }
    int getValue() {
      return value;
    }
    void readValue() {
      value = analogRead(volPin);
      value = map(value, 0, 1023, 20, 300);
      //Serial.println(value);
    }
};

class Sensor {
    int powPin;
    int pulPin;
    int ledPin;
    int thresh;
    int value;
    bool state;

  public:
    Sensor(int po, int pu, int le) {
      powPin = po;
      pulPin = pu;
      ledPin = le;
      thresh = 0;
      value = 0;
      state = false;

      pinMode(pulPin, INPUT);
      pinMode(powPin, OUTPUT);
      pinMode(ledPin, OUTPUT);
    }

    //
    //    Sensor(int po, int pu, int le, int th) {
    //      powPin = po;
    //      pulPin = pu;
    //      ledPin = le;
    //      thresh = th;
    //      value = 0;
    //      state = false;
    //
    //      pinMode(pulPin, INPUT);
    //      pinMode(powPin, OUTPUT);
    //      pinMode(ledPin, OUTPUT);
    //    }

    void readValue() {
      digitalWrite(powPin, HIGH);

      int pulse = pulseIn(pulPin, HIGH);

      value = int((pulse * 2.54) / 147);

      if (value < 1) {
        value = 0;
      }

      //Serial.println(value);

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
    void blinkLed() {
      digitalWrite(ledPin, HIGH);
      delay(1000);
      digitalWrite(ledPin, LOW);
      delay(500);
    }
    void setThresh(int input) {

      thresh = input;
    }
    int getValue() {
      return value;
    }

};

//Brightsign gpio class definition:

bool active = true;
bool normal = false;

int gDelay = 500;

class Gport
{
    int pin;
    //int ledPin;

  public:

    void setNormal() {
      digitalWrite(pin, normal);
      //digitalWrite(ledPin, LOW);
    }

    Gport(int gp) {
      pin = gp;
      //ledPin = 13;

      pinMode(pin, OUTPUT);
      //pinMode(ledPin, OUTPUT);
      setNormal();
    }

    void setActive() {
      digitalWrite(pin, active);
      //digitalWrite(ledPin, HIGH);
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
Sensor sensorA(11, 12, 13);
Sensor sensorB(9, 10, 5);
Sensor sensorC(7, 8, 4);

//BS GPIO port creation
Gport gport0(2);

//Pot objects creation
Pot potA(0);
Pot potB(1);
Pot potC(2);

void setup() {
  Serial.begin(9600);
}

void loop0() {
  sensorA.readValue();
  sensorB.readValue();
  sensorC.readValue();

  sensorA.updateState();
  sensorB.updateState();
  sensorC.updateState();

  gport0.setOutput(sensorA.getState() || sensorB.getState() || sensorC.getState());

}
void loop() {
  //read 3 pot values
  potA.readValue();
  sensorA.readValue();
  Serial.println(potA.getValue());
  sensorA.setThresh(potA.getValue());
}
