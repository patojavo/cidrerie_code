# 1 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\cidrerie_code.ino"
# 13 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\cidrerie_code.ino"
//encoder







byte prevPattern = 0; // previous signal pattern
byte pattern = 0; // actual signal pattern
bool keyPushed = false; // button push status

int tempSelect = 0;
int maxtemp = 30;
int mintemp = 1;

const int numReadings = 60;
int inputPin = A7;
int sensorValue = 0;
int Relay1Pin = 5;
int Relay2Pin = 6;

int readings[numReadings]; // the readings from the analog input
int readIndex = 0; // the index of the current reading
int total = 0; // the running total
int average = 0; // the average
float v = 0.0;
float temp = 0.0;

byte aktS1 = digitalRead(4 /* S1 pin*/);
byte aktS2 = digitalRead(3 /* S2 pin*/);
byte aktKey = !digitalRead(2 /* KEY pin*/);
byte button1Last = 0;
byte button1State = 0;


int temperature;
float temperature_consigne = 4;

struct NOMBRE
{
  int dizaine;
  int unite;
  int decimale;
};

int readTemp();
void affiche_temperature(NOMBRE nombre);
NOMBRE creer_nombre(float temp);
void relais();

void loop() {

  selectTemp(); // potntiometre + bouton
  temperature = readTemp(); // lecture de temperature par NTC
  NOMBRE nombre = creer_nombre(temperature);
  affiche_temperature(nombre); // Affichage
  relais();


}
# 1 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\7segs.ino"
void affiche_numero(int numero) {
  switch (numero) {
    case 0:
      digitalWrite(53, 0x1);
      digitalWrite(49, 0x1);
      digitalWrite(50, 0x1);
      digitalWrite(46, 0x1);
      digitalWrite(44, 0x1);
      digitalWrite(51, 0x1);
      digitalWrite(52, 0x0);
      digitalWrite(48, 0x0);
      break;
    case 1:
      digitalWrite(53, 0x0);
      digitalWrite(49, 0x1);
      digitalWrite(50, 0x1);
      digitalWrite(46, 0x0);
      digitalWrite(44, 0x0);
      digitalWrite(51, 0x0);
      digitalWrite(52, 0x0);
      digitalWrite(48, 0x0);
      break;
    case 2:
      digitalWrite(53, 0x1);
      digitalWrite(49, 0x1);
      digitalWrite(50, 0x0);
      digitalWrite(46, 0x1);
      digitalWrite(44, 0x1);
      digitalWrite(51, 0x0);
      digitalWrite(52, 0x1);
      digitalWrite(48, 0x0);
      break;
    case 3:
      digitalWrite(53, 0x1);
      digitalWrite(49, 0x1);
      digitalWrite(50, 0x1);
      digitalWrite(46, 0x1);
      digitalWrite(44, 0x0);
      digitalWrite(51, 0x0);
      digitalWrite(52, 0x1);
      digitalWrite(48, 0x0);
      break;
    case 4:
      digitalWrite(53, 0x0);
      digitalWrite(49, 0x1);
      digitalWrite(50, 0x1);
      digitalWrite(46, 0x0);
      digitalWrite(44, 0x0);
      digitalWrite(51, 0x1);
      digitalWrite(52, 0x1);
      digitalWrite(48, 0x0);
      break;
    case 5:
      digitalWrite(53, 0x1);
      digitalWrite(49, 0x0);
      digitalWrite(50, 0x1);
      digitalWrite(46, 0x1);
      digitalWrite(44, 0x0);
      digitalWrite(51, 0x1);
      digitalWrite(52, 0x1);
      digitalWrite(48, 0x0);
      break;
    case 6:
      digitalWrite(53, 0x1);
      digitalWrite(49, 0x0);
      digitalWrite(50, 0x1);
      digitalWrite(46, 0x1);
      digitalWrite(44, 0x1);
      digitalWrite(51, 0x1);
      digitalWrite(52, 0x1);
      digitalWrite(48, 0x0);
      break;
    case 7:
      digitalWrite(53, 0x1);
      digitalWrite(49, 0x1);
      digitalWrite(50, 0x1);
      digitalWrite(46, 0x0);
      digitalWrite(44, 0x0);
      digitalWrite(51, 0x0);
      digitalWrite(52, 0x0);
      digitalWrite(48, 0x0);
      break;
    case 8:
      digitalWrite(53, 0x1);
      digitalWrite(49, 0x1);
      digitalWrite(50, 0x1);
      digitalWrite(46, 0x1);
      digitalWrite(44, 0x1);
      digitalWrite(51, 0x1);
      digitalWrite(52, 0x1);
      digitalWrite(48, 0x0);
      break;
    case 9:
      digitalWrite(53, 0x1);
      digitalWrite(49, 0x1);
      digitalWrite(50, 0x1);
      digitalWrite(46, 0x1);
      digitalWrite(44, 0x0);
      digitalWrite(51, 0x1);
      digitalWrite(52, 0x1);
      digitalWrite(48, 0x0);
      break;
  }


}
# 1 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\fonctions.ino"

void affiche_temperature(NOMBRE nombre) {
  digitalWrite(43, 0x0);
  digitalWrite(47, 0x1);
  digitalWrite(45, 0x1);
  affiche_numero(nombre.decimale);
  digitalWrite(48, 0x0);
  delay(1);
  digitalWrite(43, 0x1);
  digitalWrite(47, 0x0);
  digitalWrite(45, 0x1);
  affiche_numero(nombre.unite);
  digitalWrite(48, 0x1);
  delay(1);
  digitalWrite(43, 0x1);
  digitalWrite(47, 0x1);
  digitalWrite(45, 0x0);
  affiche_numero(nombre.dizaine);
  digitalWrite(48, 0x0);
  delay(1);
}

NOMBRE creer_nombre(float temp) {
  NOMBRE nombre;
  nombre.dizaine = (int)temp / 100;
  nombre.unite = (int)(temp - (nombre.dizaine * 100)) / 10;
  nombre.decimale = temp - nombre.dizaine * 100 - nombre.unite * 10;
  return nombre;
}
void relais() {

  if ((temp >= temperature_consigne + .5) && (button1State == 1))
  {
    digitalWrite(Relay1Pin, 0x0);
    digitalWrite(Relay2Pin, 0x0);
  }
  else if (temp >= temperature_consigne - .5 && button1State == 0) {
    digitalWrite(Relay1Pin, 0x1);
    digitalWrite(Relay2Pin, 0x1);
  }
}


int readTemp() {
  // subtract the last reading:
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits

  v = average * (5.0 / 1023.0);
  temp = 0.1939 * pow(v, 6) - 3.5228 * pow(v, 5) + 24.898 * pow(v, 4) - 88.351 * pow(v, 3) + 168.26 * pow(v, 2) - 185.45 * v + 132.38;
  temp = temp * 10;
  return temp;
}

int selectTemp() {
  //Test this button function, it might have errors. It will be used to start the cooling and regulation process

  if (aktKey == 1 && button1State == 0) {
    button1State = 1;
  }
  if (aktKey == 1 && button1State == 1) {
    button1State == 0;
  }

  Serial.println(button1State);

  byte aktPattern = (aktS1 << 1) | aktS2; //je ne comprend pas vraiment cette partie :/

  if (aktPattern != prevPattern) {
    prevPattern = aktPattern;
    if ( aktPattern != 0b11 /* default / base value of encoder signal*/ ) {
      pattern = (pattern << 2) | aktPattern;
    } else {
      if (pattern == 0b100001 /* knob turn CW pattern*/ ) {
        tempSelect++;
        if (tempSelect >= maxtemp) {
          tempSelect = maxtemp;
        }
      } else if (pattern == 0b010010 /* knob turn CCW pattern*/ ) {
        tempSelect--;
        if (tempSelect <= mintemp) {
          tempSelect = mintemp;
        }
      }
      pattern = 0;
    }
  }
}
# 1 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\setup.ino"
void setup() {
  //  setup serial
  Serial.begin(9600);
  // Set RelayPin as an output pin

  pinMode(Relay1Pin, 0x1);
  pinMode(Relay2Pin, 0x1);

  pinMode(2 /* KEY pin*/, 0x0);
  pinMode(3 /* S2 pin*/, 0x0);
  pinMode(4 /* S1 pin*/, 0x0);

  pinMode(53, 0x1);
  pinMode(49, 0x1);
  pinMode(50, 0x1);
  pinMode(46, 0x1);
  pinMode(44, 0x1);
  pinMode(51, 0x1);
  pinMode(52, 0x1);
  pinMode(48, 0x1);
  pinMode(43, 0x1);
  pinMode(47, 0x1);
  pinMode(45, 0x1);
}
