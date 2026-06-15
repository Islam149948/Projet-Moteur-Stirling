const int NB_TROUS = 50;

// Volumes réels (cm³)
const float Vmax = 35.0;
const float Vmin = 22.4;

// Capteur de pression
const float VREF = 5.0;        // tension Arduino
const float PMAX = 100000.0;   // pression max du capteur (Pa) → à adapter

/************ VARIABLES ************/
volatile int trou = 0;
volatile int gtrou = 0;

float toursParSeconde = 0;
float toursParMinute = 0;
const int capteurPin = 3;
volatile unsigned long comptageImpuls = 0;
unsigned long tempsPrecedent = 0;
const unsigned long intervalle = 1000; // 1 seconde

void comptergTrou() {
  gtrou++;
  if (gtrou = 1) {
    trou = 37;
    gtrou = 0;
  }
}
void compterTrou() {
  trou++;
  if (trou >= 50) {
    trou = 0;
  }
}
/************ CALCUL DU VOLUME ************/
float calculVolume(int n) {
  float theta = 2.0 * PI * n / NB_TROUS;
  float V0 = 0.5 * (Vmax + Vmin);   // 1 → Vmax, 0 → Vmin
  return (Vmax -V0 )* cos(theta) + V0;      // cm³
}

/************ LECTURE PRESSION ************/
float lirePression() {
  int adc = analogRead(A0);
  float tension = adc * VREF / 1023.0;
  return (tension / VREF) * PMAX;          // Pa
}

/************ SETUP ************/
void setup() {
  pinMode(capteurPin, INPUT);
  Serial.begin(115200);
  pinMode(3, INPUT_PULLUP);   // D2
  attachInterrupt(digitalPinToInterrupt(3), comptergTrou, FALLING);
  pinMode(2, INPUT_PULLUP);   // D2
  attachInterrupt(digitalPinToInterrupt(2), compterTrou, FALLING);
  Serial.println("Mesure volume et pression");
  attachInterrupt(digitalPinToInterrupt(3), compter, FALLING);
}
/************ LOOP ************/
void loop() {

  unsigned long tempsActuel = millis();

  if (tempsActuel - tempsPrecedent >= intervalle) {
    noInterrupts();
    unsigned long impulsions = comptageImpuls;
    comptageImpuls = 0;
    interrupts();

    toursParSeconde = impulsions;
    toursParMinute = toursParSeconde * 60.0;

    tempsPrecedent = tempsActuel;
  }

  int n;
 
  n = trou;
 
  float volume = calculVolume(n);   // cm³
  float pression = lirePression();  // Pa

  //Serial.print("X:");
  Serial.print(volume);
  Serial.print(" - ");
  Serial.print(pression);  
  Serial.print(" _ ");
  Serial.println(toursParMinute);

  delay(13);
}

void compter() {
  comptageImpuls++;
}
