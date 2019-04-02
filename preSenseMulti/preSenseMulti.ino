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

    void readValue() {
      digitalWrite(powPin, HIGH);

      int pulse = pulseIn(pulPin, HIGH);
      //int pulse = pulseIn(pulPin, HIGH, 400000);

      Serial.println(pulse);

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
    void displayState() {
      if (state) {
        digitalWrite(ledPin, HIGH);
      } else {
        digitalWrite(ledPin, LOW);
      }
    }

    bool getState() {
      return state;
    }
    void blinkOn() {
      digitalWrite(ledPin, HIGH);
    }
    void blinkOff() {
      digitalWrite(ledPin, LOW);
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

  public:

    void setNormal() {
      digitalWrite(pin, normal);
    }

    Gport(int gp) {
      pin = gp;

      pinMode(pin, OUTPUT);
      setNormal();
    }

    void setActive() {
      digitalWrite(pin, active);
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

void blinkAlive() {

  sensorA.blinkOn();
  sensorB.blinkOn();
  sensorC.blinkOn();
  delay(1000);
  sensorA.blinkOff();
  sensorB.blinkOff();
  sensorC.blinkOff();
  delay(200);

}

void setup() {

  blinkAlive();

  Serial.begin(9600);
}

void loop() {
  routineA();
  routineB();
  routineC();
  gport0.setOutput(sensorA.getState() || sensorB.getState() || sensorC.getState());
}
void routineA() {
  potA.readValue();
  sensorA.readValue();
  sensorA.setThresh(potA.getValue());
  sensorA.updateState();
  sensorA.displayState();
}
//B
void routineB() {
  potB.readValue();
  sensorB.readValue();
  sensorB.setThresh(potB.getValue());
  sensorB.updateState();
  sensorB.displayState();
}
//C
void routineC() {
  potC.readValue();
  sensorC.readValue();
  sensorC.setThresh(potC.getValue());
  sensorC.updateState();
  sensorC.displayState();
}
