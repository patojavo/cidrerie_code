#include <Arduino.h>
#line 1 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\cidrerie_code.ino"
#define DIGIT3 45
#define DIGIT1 43
#define DIGIT2 47
#define SEGA 53
#define SEGB 49
#define SEGC 50
#define SEGD 46
#define SEGE 44
#define SEGF 51
#define SEGG 52
#define SEGP 48

//encoder
#define KEY 2 // KEY pin
#define S2 3  // S2 pin
#define S1 4  // S1 pin
#define CCW 0b010010  // knob turn CCW pattern
#define CW 0b100001   // knob turn CW pattern
#define DEF 0b11  // default / base value of encoder signal

byte prevPattern = 0; // previous signal pattern
byte pattern = 0;     // actual signal pattern
bool keyPushed = false;     // button push status

int tempSelect = 0;
int maxtemp = 30;
int mintemp = 1;

const int numReadings = 60;
int inputPin = A7;
int sensorValue = 0;
int Relay1Pin = 5;
int Relay2Pin = 6;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
float v = 0.0;
float temp = 0.0;

byte aktS1 = digitalRead(S1);
byte aktS2 = digitalRead(S2);
byte aktKey = !digitalRead(KEY);
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

#line 64 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\cidrerie_code.ino"
void loop();
#line 1 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\7segs.ino"
void affiche_numero(int numero);
#line 67 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\fonctions.ino"
int selectTemp();
#line 1 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\setup.ino"
void setup();
#line 64 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\cidrerie_code.ino"
void loop() {

  selectTemp(); // potntiometre + bouton
  temperature = readTemp(); // lecture de temperature par NTC
  NOMBRE nombre = creer_nombre(temperature);
  affiche_temperature(nombre); // Affichage
  relais();


}

#line 1 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\7segs.ino"
void affiche_numero(int numero) {
  switch (numero) {
    case 0:
      digitalWrite(SEGA, HIGH);
      digitalWrite(SEGB, HIGH);
      digitalWrite(SEGC, HIGH);
      digitalWrite(SEGD, HIGH);
      digitalWrite(SEGE, HIGH);
      digitalWrite(SEGF, HIGH);
      digitalWrite(SEGG, LOW);
      digitalWrite(SEGP, LOW);
      break;
    case 1:
      digitalWrite(SEGA, LOW);
      digitalWrite(SEGB, HIGH);
      digitalWrite(SEGC, HIGH);
      digitalWrite(SEGD, LOW);
      digitalWrite(SEGE, LOW);
      digitalWrite(SEGF, LOW);
      digitalWrite(SEGG, LOW);
      digitalWrite(SEGP, LOW);
      break;
    case 2:
      digitalWrite(SEGA, HIGH);
      digitalWrite(SEGB, HIGH);
      digitalWrite(SEGC, LOW);
      digitalWrite(SEGD, HIGH);
      digitalWrite(SEGE, HIGH);
      digitalWrite(SEGF, LOW);
      digitalWrite(SEGG, HIGH);
      digitalWrite(SEGP, LOW);
      break;
    case 3:
      digitalWrite(SEGA, HIGH);
      digitalWrite(SEGB, HIGH);
      digitalWrite(SEGC, HIGH);
      digitalWrite(SEGD, HIGH);
      digitalWrite(SEGE, LOW);
      digitalWrite(SEGF, LOW);
      digitalWrite(SEGG, HIGH);
      digitalWrite(SEGP, LOW);
      break;
    case 4:
      digitalWrite(SEGA, LOW);
      digitalWrite(SEGB, HIGH);
      digitalWrite(SEGC, HIGH);
      digitalWrite(SEGD, LOW);
      digitalWrite(SEGE, LOW);
      digitalWrite(SEGF, HIGH);
      digitalWrite(SEGG, HIGH);
      digitalWrite(SEGP, LOW);
      break;
    case 5:
      digitalWrite(SEGA, HIGH);
      digitalWrite(SEGB, LOW);
      digitalWrite(SEGC, HIGH);
      digitalWrite(SEGD, HIGH);
      digitalWrite(SEGE, LOW);
      digitalWrite(SEGF, HIGH);
      digitalWrite(SEGG, HIGH);
      digitalWrite(SEGP, LOW);
      break;
    case 6:
      digitalWrite(SEGA, HIGH);
      digitalWrite(SEGB, LOW);
      digitalWrite(SEGC, HIGH);
      digitalWrite(SEGD, HIGH);
      digitalWrite(SEGE, HIGH);
      digitalWrite(SEGF, HIGH);
      digitalWrite(SEGG, HIGH);
      digitalWrite(SEGP, LOW);
      break;
    case 7:
      digitalWrite(SEGA, HIGH);
      digitalWrite(SEGB, HIGH);
      digitalWrite(SEGC, HIGH);
      digitalWrite(SEGD, LOW);
      digitalWrite(SEGE, LOW);
      digitalWrite(SEGF, LOW);
      digitalWrite(SEGG, LOW);
      digitalWrite(SEGP, LOW);
      break;
    case 8:
      digitalWrite(SEGA, HIGH);
      digitalWrite(SEGB, HIGH);
      digitalWrite(SEGC, HIGH);
      digitalWrite(SEGD, HIGH);
      digitalWrite(SEGE, HIGH);
      digitalWrite(SEGF, HIGH);
      digitalWrite(SEGG, HIGH);
      digitalWrite(SEGP, LOW);
      break;
    case 9:
      digitalWrite(SEGA, HIGH);
      digitalWrite(SEGB, HIGH);
      digitalWrite(SEGC, HIGH);
      digitalWrite(SEGD, HIGH);
      digitalWrite(SEGE, LOW);
      digitalWrite(SEGF, HIGH);
      digitalWrite(SEGG, HIGH);
      digitalWrite(SEGP, LOW);
      break;
  }


}

#line 1 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\fonctions.ino"

void affiche_temperature(NOMBRE nombre) {
  digitalWrite(DIGIT1, LOW);
  digitalWrite(DIGIT2, HIGH);
  digitalWrite(DIGIT3, HIGH);
  affiche_numero(nombre.decimale);
  digitalWrite(SEGP, LOW);
  delay(1);
  digitalWrite(DIGIT1, HIGH);
  digitalWrite(DIGIT2, LOW);
  digitalWrite(DIGIT3, HIGH);
  affiche_numero(nombre.unite);
  digitalWrite(SEGP, HIGH);
  delay(1);
  digitalWrite(DIGIT1, HIGH);
  digitalWrite(DIGIT2, HIGH);
  digitalWrite(DIGIT3, LOW);
  affiche_numero(nombre.dizaine);
  digitalWrite(SEGP, LOW);
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
    digitalWrite(Relay1Pin, LOW);
    digitalWrite(Relay2Pin, LOW);
  }
  else if (temp >= temperature_consigne - .5 && button1State == 0) {
    digitalWrite(Relay1Pin, HIGH);
    digitalWrite(Relay2Pin, HIGH);
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

  byte aktPattern = (aktS1 << 1) | aktS2;  //je ne comprend pas vraiment cette partie :/

  if (aktPattern != prevPattern) {
    prevPattern = aktPattern;
    if ( aktPattern != DEF ) {
      pattern = (pattern << 2) | aktPattern;
    } else {
      if (pattern == CW ) {
        tempSelect++;
        if (tempSelect >= maxtemp) {
          tempSelect = maxtemp;
        }
      } else if (pattern == CCW ) {
        tempSelect--;
        if (tempSelect <= mintemp) {
          tempSelect = mintemp;
        }
      }
      pattern = 0;
    }
  }
}

#line 1 "c:\\Users\\miles\\OneDrive\\HEIG\\Projet\\Projet Multi\\cidrerie_code\\setup.ino"
void setup() {
  //  setup serial
  Serial.begin(9600);
  // Set RelayPin as an output pin
  
  pinMode(Relay1Pin, OUTPUT);
  pinMode(Relay2Pin, OUTPUT);

  pinMode(KEY, INPUT);
  pinMode(S2, INPUT);
  pinMode(S1, INPUT);

  pinMode(SEGA, OUTPUT);
  pinMode(SEGB, OUTPUT);
  pinMode(SEGC, OUTPUT);
  pinMode(SEGD, OUTPUT);
  pinMode(SEGE, OUTPUT);
  pinMode(SEGF, OUTPUT);
  pinMode(SEGG, OUTPUT);
  pinMode(SEGP, OUTPUT);
  pinMode(DIGIT1, OUTPUT);
  pinMode(DIGIT2, OUTPUT);
  pinMode(DIGIT3, OUTPUT);
}

