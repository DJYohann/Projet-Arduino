/* Code de Maxime DESGRANGES et Yohann BREUIL
   Protototype de système de surveillance
   Un bouton poussoir permet de changer de mode
   La température, l'intensité lumineuse, l'heure et le mode sont affichés sur l'afficheur LCD
   La lumière s'allume proportionnelement à l'intensité lumineuse
   Le systèle d'alarme s'active quand la température depasse le seuil bas ou haut
*/

#include <LiquidCrystal.h>

// Initialisation de l'afficheur LCD

LiquidCrystal lcd (12, 11, 5, 4, 3, 2);

// Déclarations des constantes

#define bouton 7
#define buzzer 8
#define lumiere 9
#define captTemp A0
#define captLum A1

// Déclaration du signe soleil

byte Sun[] = {
  B00000,
  B00100,
  B10101,
  B01110,
  B01110,
  B01110,
  B10101,
  B00100
};

// Déclaration des variables globales

bool mode = 0;
int seuilBas = 20;
int seuilHaut = 35;
int etatLedCarte = LOW;
long previousMillis = 0;

int heure = 1;
int minute = 23;
int seconde = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  lcd.createChar(0, Sun); // Création du signe soleil
  lcd.clear();
  lcd.print(" ");

  pinMode(bouton, INPUT_PULLUP);
  pinMode(lumiere, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  clignotementLedCarte();
  horloge();
  char mode = modeSurveillance();
  int temp = lireTemperature();
  int lumi = lireLuminosite();
  affichage(mode, temp, lumi, heure, minute);
  if (mode == 'S') {
    alarme(temp);
    lumiereAmbiante(lumi);
  }
  delay(1000);
  seconde++;
}

/* Fonction affichage
   Rôle : afficher toutes les valeurs récupérées sur l'afficheur LCD
   Paramètres : char mode - Mode en cours (normal ou surveillance)
                int temp - température récupérée par le capteur de température
                int lux - intensité lumineuse récupérée par la photorésistance
                int heure - heure récupérée sur le moniteur série
                int minute - minute récupérée sur le moniteur série
*/

void affichage (char mode, int temp, int lux, int heure, int minute) {
  // Affichage du mode
  lcd.setCursor(0, 0);
  lcd.print(mode);

  // Affichage de l'heure
  lcd.setCursor(6, 0);
  if (heure < 10) {
    lcd.print(0);
  }
  lcd.print(heure);

  lcd.setCursor(8, 0);
  lcd.print(":");
  if (minute < 10) {
    lcd.print(0);
  }
  lcd.print(minute);

  // Affichage de l'intensité lumineuse
  if (lux >= 0 && lux < 10) {
    lcd.setCursor(3, 1);
    lcd.print(lux);
  }
  else if (lux >= 10 && lux < 100) {
    lcd.setCursor(2, 1);
    lcd.print(lux);
  }
  else {
    lcd.setCursor(1, 1);
    lcd.print(lux);
  }
  lcd.setCursor(4, 1);
  lcd.write(byte(0));

  // Affichage de la température
  if (temp >= 0 && temp < 10) {
    lcd.setCursor(13, 1);
    lcd.print(temp);
  }
  else if (temp >= 10 && temp < 100) {
    lcd.setCursor(12, 1);
    lcd.print(temp);
  }
  else {
    lcd.setCursor(11, 1);
    lcd.print(temp);
  }
  lcd.setCursor(14, 1);
  lcd.print("\262C");
}

/* Fonction horloge
   Rôle : Défiler l'heure et les minutes 
*/

void horloge (void) {
  if (seconde == 60) {
    minute++;
  }

  if (minute == 60) {
    heure++;
    minute = 0;
  }

  if (heure == 24) {
    heure = 0;
    minute = 0;
  }
}

/* Fonction modeSurveillance
   Rôle : Renvoyer le caractère associé (S pour Surveillance et N pour Normal) au mode grâce à l'appui du bouton poussoir
*/

char modeSurveillance (void) {
  bool valBouton = digitalRead(bouton);

  // Changement du mode grâ à l'appui du bouton
  if (!valBouton) {
    if (mode) {
      mode = 0;
    } else {
      mode = 1;
    }
  }

  // Renvoi du caractère associé au mode
  if (mode) {
    return 'S';
  } else {
    return 'N';
  }
}

/* Fonction lireTemperature
   Rôle : Renvoyer la température donnée par le capteur de température
   Valeur de retour : int temperature - température lue
*/

int lireTemperature (void) {
  int val = analogRead(captTemp);
  float tension = (val * 5.0) / 1024.0;
  int temperature = (((tension * 1000) - 500) / 10) + 0.9;

  return temperature;
}

/* Fonction lireLuminosite
   Rôle : Renvoyer l'intensité lumineuse donnée par la photorésistance
   Valeur de retour : int luminosite - intensité lumineuse lue
*/

int lireLuminosite (void) {
  int luminosite = analogRead(captLum);

  return luminosite;
}

/* Fonction alarme
   Rôle : Émettre un son aigu si la température passe le seuil bas et émettre un son grave si elle dépasse le seuil haut
   Paramètre : int temp - température renvoyée par le capteur de température
*/

void alarme (int temp) {
  if (temp < seuilBas) {
    tone(buzzer, 1200);
    delay(100);
  }
  if (temp > seuilHaut) {
    tone(buzzer, 200);
    delay(100);
  }
  noTone(buzzer);
}

/* Fonction lumiereAmbiante
   Rôle : Allumer la lumière proportionnellement à l'intensité lumineuse
   Paramètre : int lum - intensité lumineuse renvoyée par la photorésistance
*/

void lumiereAmbiante (int lum) {
  int val = map(lum, 0, 1023, 0, 255);
  val = (val - 255) * -1;

  analogWrite(lumiere, val);
}

/* Fonction clignotementLedCarte
   Rôle : Faire cligonter la led de la carte toutes les secondes
*/

void clignotementLedCarte (void) {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > 1000) {
    previousMillis = currentMillis;

    // Inversion de l'état de la led
    if (etatLedCarte == LOW) {
      etatLedCarte = HIGH;
    }
    else {
      etatLedCarte = LOW;
    }
    
    digitalWrite(LED_BUILTIN, etatLedCarte);
  }
}
