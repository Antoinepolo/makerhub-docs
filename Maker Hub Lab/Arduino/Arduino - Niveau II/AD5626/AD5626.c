#include "AD5626.h"

// Constructeur, il ne fait que de donner les pins passée à la variable privée
AD5626::AD5626(int csPin, int clrPin, int ldacPin) {
  _csPin = csPin;
  _clrPin = clrPin;
  _ldacPin = ldacPin;
}

void AD5626::begin() {
  // On définit la broche Chip Select en sortie
  pinMode(_csPin, OUTPUT);
  // On passe l'état de la broche Chip Select à haut pour signier au DAC d'arrêter d'écouter
  digitalWrite(_csPin, HIGH);

  // Initialisation de la pin de Reset (CLR)
  if (_clrPin != -1) {
    // On définie la broche RESET en sortie
    pinMode(_clrPin, OUTPUT);
     // On la met à HIGH pour ne pas être en reset permanent
    digitalWrite(_clrPin, HIGH);
  }

  // Initialisation de la pin de Verrou (LDAC)
  if (_ldacPin != -1) {
    // On définie la broche LDAC en sortie
    pinMode(_ldacPin, OUTPUT);
    // On la met à LOW pour que la sortie réagisse direct
    digitalWrite(_ldacPin, LOW);
  }
  
  // On initialise le SPI de l'Arduino
  SPI.begin();
}

void AD5626::setValue(int value) {
  // On empêche la valeur de dépasser la limite de 12 bits
  value = constrain(value, 0, 4095);

  // Configuration du SPI
  // 1MHz : Fréquence
  // MSBFIRST : On envoie le bit de poids fort en premier
  // SPI_MODE1 : Réglage spécifique du signal d'horloge pour l'AD5626
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
  
  // On passe l'état de la broche Chip Select à bas pour signier au DAC de commencer à écouter
  digitalWrite(_csPin, LOW);
  
  // Envoi des 12 bits alignés à droite (en 2 octets)
  // Envoie la partie haute (les 8 bits de gauche)
  SPI.transfer(highByte(value));
  // Envoie la partie basse (les 8 bits de droite)
  SPI.transfer(lowByte(value));
  
  // On passe l'état de la broche Chip Select à haut pour signier au DAC d'arrêter d'écouter
  digitalWrite(_csPin, HIGH);

  // On libère le bus SPI
  SPI.endTransaction();
}

// Alias plus intuitif
void AD5626::setVoltage(int mv) {
  // On appelle la fonction du haut
  setValue(mv);
}

// Fonction pour reset le DAC
void AD5626::reset() {
  if (_clrPin != -1) {
    // On force la sortie à l'état bas physiquement
    digitalWrite(_clrPin, LOW);
    
    // On vide le registre d'entrée via SPI pour pas qu'il ne se recharge
    setValue(0); 
    
    // Petit temps de maintien
    delayMicroseconds(2);
    
    // On libère le reset on le repassant à l'état haut
    digitalWrite(_clrPin, HIGH);
  }
}

// Fonction pour bloquer la sortie du DAC
void AD5626::lock() {
  // On vérifie que la broche est bien spécifiée
  if (_ldacPin != -1) {
    // On met la broche à l'état haut (bloque les données)
    digitalWrite(_ldacPin, HIGH);
  }
}

// Fonction pour débloquer la sortie du DAC
void AD5626::release() {
  // On vérifie que la broche est bien spécifiée
  if (_ldacPin != -1) {
    // On met la broche à l'état bas (débloque les données)
    digitalWrite(_ldacPin, LOW); // On ouvre le verrou
  }
}
