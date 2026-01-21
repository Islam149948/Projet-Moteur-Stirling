const int NB_TROUS = 50;            // Nombre total de petits trous sur la roue (1 tour = 50 impulsions)

// Volumes réels (cm³)
const float Vmax = 35.0;            // Volume maximal du système mécanique
const float Vmin = 22.4;            // Volume minimal du système mécanique

// Capteur de pression
const float VREF = 5.0;             // Tension de référence de l’Arduino (5 V)
const float PMAX = 100000.0;        // Pression maximale mesurable par le capteur (en Pa)

/************ VARIABLES ************/

volatile int trou = 0;              // Compteur des petits trous → position angulaire de la roue
volatile int gtrou = 0;             // Détection du grand trou → repère de position absolue

/************ INTERRUPTION ************/

void comptergTrou() {               // Fonction appelée lors du passage du grand trou
  gtrou++;                           // Incrémentation du compteur du grand trou
  if (gtrou = 1) {                   // Test (affectation) exécuté lors de la détection
    trou = 37;                       // Recalage de la position : le grand trou correspond au trou n°37
    gtrou = 0;                       // Remise à zéro du compteur du grand trou
  }
}

void compterTrou() {                // Fonction appelée à chaque passage d’un petit trou
  trou++;                            // Incrémentation de la position angulaire
  if (trou >= 50) {                  // Si on dépasse le nombre total de petits trous
    trou = 0;                        // Retour à zéro pour recommencer un nouveau tour
  }
}

/************ CALCUL DU VOLUME ************/

float calculVolume(int n) {          // Fonction qui calcule le volume en fonction de la position
  float theta = 2.0 * PI * n / NB_TROUS; // Conversion du numéro de trou en angle (en radians)
  float V0 = 0.5 * (Vmax + Vmin);    // Calcul du volume moyen entre Vmax et Vmin
  return (Vmax - V0) * cos(theta) + V0; // Calcul du volume suivant une loi cosinus
}

/************ LECTURE PRESSION ************/

float lirePression() {               // Fonction de lecture de la pression
  int adc = analogRead(A0);          // Lecture de la valeur analogique du capteur (0 à 1023)
  float tension = adc * VREF / 1023.0; // Conversion de la valeur numérique en tension
  return (tension / VREF) * PMAX;    // Conversion de la tension en pression (Pa)
}

/************ SETUP ************/

void setup() {                       // Fonction exécutée une seule fois au démarrage
  Serial.begin(9600);                // Initialisation de la communication série à 9600 bauds

  pinMode(3, INPUT_PULLUP);          // Broche 3 configurée en entrée avec résistance pull-up
  attachInterrupt(digitalPinToInterrupt(3), comptergTrou, FALLING); // Interruption sur le grand trou (front descendant)

  pinMode(2, INPUT_PULLUP);          // Broche 2 configurée en entrée avec résistance pull-up
  attachInterrupt(digitalPinToInterrupt(2), compterTrou, FALLING); // Interruption sur les petits trous (front descendant)

  Serial.println("Mesure volume et pression"); // Message affiché au démarrage
}

/************ LOOP ************/

void loop() {                        // Boucle principale exécutée en continu
  int n;                             // Variable locale pour stocker la position de la roue

  n = trou;                          // Copie de la position angulaire actuelle (petits trous)

  float volume = calculVolume(n);    // Calcul du volume théorique correspondant à cette position
  float pression = lirePression();   // Lecture de la pression réelle mesurée par le capteur

  Serial.print(volume);              // Envoi du volume vers le moniteur série
  Serial.print(" - ");               // Envoi d’un séparateur pour la lisibilité
  Serial.println(pression);          // Envoi de la pression vers le moniteur série

  delay(100);                        // Pause de 100 ms entre deux mesures
}
