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
    int thresh;
    int value;
    bool state;
    bool prev;
    bool safe;

  public:

    Sensor(int po, int pu) {
      powPin = po;
      pulPin = pu;
      thresh = 0;
      value = 0;
      state = false;
      prev = false;
      safe = false;


      pinMode(pulPin, INPUT);
      pinMode(powPin, OUTPUT);
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
      
      if (state && prev) {
        safe = true;
      } else {
        safe = false;
      }
      
      swapState();
    }
    //after this update, safe value must be evaluated
    bool getState() {
      return state;
    }
    bool getPrev() {
      return prev;
    }
    bool getSafe() {
      return safe;
    }
    void swapState() {
      prev = state;
    }
    void setThresh(int input) {

      thresh = input;
    }
    int getValue() {
      return value;
    }

};
//zone class definition:
class Zone {

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
    void enable() {
      //in case Ball has been punched, Brightsign active value is set
      setActive();
      setNormal();
    }

};

//sensor objects creation
Sensor sensorA(11, 12);
Sensor sensorB(9, 10);
Sensor sensorC(7, 8);

//BS GPIO port creation
Gport gport2(2);
Gport gport3(3);
Gport gport4(4);
Gport gport5(5);
Gport gport6(6);

//Pot objects creation
Pot potA(0);
Pot potB(1);
Pot potC(2);

void setup() {

  //blinkAlive();

  Serial.begin(9600);
}

void loop() {
  routineA();
  routineB();
  routineC();

  outputZone(checkZone(sensorA.getSafe(), sensorB.getSafe(), sensorC.getSafe()));
}
void routineA() {
  potA.readValue();
  sensorA.readValue();
  sensorA.setThresh(potA.getValue());
  sensorA.updateState();
  //sensorA.displayState();
}
//B
void routineB() {
  potB.readValue();
  sensorB.readValue();
  sensorB.setThresh(potB.getValue());
  sensorB.updateState();
  //sensorB.displayState();
}
//C
void routineC() {
  potC.readValue();
  sensorC.readValue();
  sensorC.setThresh(potC.getValue());
  sensorC.updateState();
  //sensorC.displayState();
}

int checkZone(bool inputA, bool inputB, bool inputC) {
  if (inputA && inputB && (inputC == false)) {
    //zones 0+1=zone 10;
    return 10;
  }
  else if (inputB && inputC && (inputA == false)) {
    //zones 1+2=12
    return 12;
  }
  else if (inputA && (inputB == false) && (inputC == false)) {
    return 0;
  }
  else if (inputB && (inputC == false) && (inputA == false)) {
    return 1;
  }
  else if (inputC && (inputA == false) && (inputB == false)) {
    return 2;
  }
  else {
    return 7;
  }
}
void outputZone(int input) {
  switch (input) {

    case 0:
      //from sensorA
      gport2.enable();
      break;

    case 1:
      //from sensorB
      gport4.enable();
      break;

    case 2:
      //from sensorC
      gport6.enable();
      break;

    case 10:
      //from sensorA+B
      gport3.enable();
      break;

    case 12:
      //from sensorB+C
      gport5.enable();
      break;

    default:
      //no sensor or 3 at a time
      //donthn
      break;
  }
}
