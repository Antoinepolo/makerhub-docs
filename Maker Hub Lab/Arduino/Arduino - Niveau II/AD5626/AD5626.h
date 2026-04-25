#ifndef AD5626_h
#define AD5626_h

#include "Arduino.h"
#include <SPI.h>

// Classe qui définit l'objet AD5626
class AD5626 {
  public:
    // Constructeur, prend en paramètre la pin CS (Chip Select), et optionnelement la broche CLR et LDAC
    AD5626(int csPin, int clrPin = -1, int ldacPin = -1);
    
    // Initialise la communication SPI et définit la broche CS en sortie
    void begin();
    
    // Envoie une valeur brute de 12 bits (0-4095)
    void setValue(int value);
    
    // Envoie une tension en millivolts (aussi sur 12 bits) (0-4095 mV)
    void setVoltage(int mv);

    // Fonction pour reset l'ADC (broche CLR)
    void reset();

    // Fonction pour bloquer la sortie (broche LDAC)
    void lock();
    void release();

  private:
    // Variable interne pour le numéro de la broche Chip Select, on ne peut pas le changer
    int _csPin;
    int _clrPin;
    int _ldacPin;
};

#endif
