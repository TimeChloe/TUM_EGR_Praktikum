/**
 * EGR Praktikum
 * Termin 1 - Aufgabe 2
 */

#include <stdint.h>

int main(void)
{
    // place code to run once here

    // Aktivierung des GPIO A Moduls
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + 0x4C);
    *adresse |=(1 << 0);

    //Taster1 Pin als Input konfigurieren
    adresse = (uint32_t *)(0x48000000 + 0x00);
    *adresse &= ~(1 << 4);
    *adresse &= ~(1 << 5);

    //Taster2 Pin als Input konfigurieren
    adresse = (uint32_t *)(0x48000000 + 0x00);
    *adresse &= ~(1 << 6);
    *adresse &= ~(1 << 7);

    //Taster3 Pin als Input konfigurieren
    adresse = (uint32_t *)(0x48000000 + 0x00);
    *adresse &= ~(1 << 8);
    *adresse &= ~(1 << 9);

    //Taster1 Pin als pull-up einstellen
    adresse = (uint32_t *)(0x48000000 + 0x0C);
    *adresse &= ~(1 << 5);
    *adresse |= (1 << 4);

    //Taster2 Pin als pull-up einstellen
    adresse = (uint32_t *)(0x48000000 + 0x0C);
    *adresse &= ~(1 << 7);
    *adresse |= (1 << 6);

    //Taster3 Pin als pull-up einstellen
    adresse = (uint32_t *)(0x48000000 + 0x0C);
    *adresse &= ~(1 << 9);
    *adresse |= (1 << 8);  

    // LED Blau Pin als Output konfigurieren
    adresse = (uint32_t *)(0x48000000 + 0x00);
    *adresse &= ~(1 << 17);
    *adresse |= (1 << 16);

    // LED Rot Pin als Output konfigurieren
    adresse = (uint32_t *)(0x48000000 + 0x00);
    *adresse &= ~(1 << 19);
    *adresse |= (1 << 18);

    // LED Gruen Pin als Output konfigurieren
    adresse = (uint32_t *)(0x48000000 + 0x00);
    *adresse &= ~(1 << 21);
    *adresse |= (1 << 20);

    // LED Blau Pin als push-pull einstellen
    adresse = (uint32_t *)(0x48000000 + 0x04);
    *adresse &= ~(1 << 8);

    // LED Rot Pin als push-pull einstellen
    adresse = (uint32_t *)(0x48000000 + 0x04);
    *adresse &= ~(1 << 9);

    // LED Gruen Pin als push-pull einstellen
    adresse = (uint32_t *)(0x48000000 + 0x04);
    *adresse &= ~(1 << 10);

    while (1)
    {
    // place your loop code here

    //Alle LEDs ausschalten
    adresse = (uint32_t *)(0x48000000 + 0x14);
    *adresse &= ~((1 << 8) | (1 << 9) | (1 << 10));  

    //Taster auslesen
    adresse = (uint32_t *)(0x48000000 + 0x10);

     //LED Blau steuern
    if (!(*adresse & (1 << 2))) {  // Taster 1 betätigt
        adresse = (uint32_t *)(0x48000000 + 0x14);
        *adresse |= (1 << 8); // LED Blau einschalten
        *adresse &= ~((1 << 9) | (1 << 10));  // LED Rot und Gruen ausschalten
     } 

    // // LED Rot steuern
    else if (!(*adresse & (1 << 3))) {  // Taster 2 betätigt
        adresse = (uint32_t *)(0x48000000 + 0x14);
        *adresse |= (1 << 9);  // LED Rot einschalten
        *adresse &= ~((1 << 8) | (1 << 10));  // LED Blau und Gruen ausschalten
    } 

    // LED Gruen steuern
    else if (!(*adresse & (1 << 4))) {  // Taster 3 betätigt
        adresse = (uint32_t *)(0x48000000 + 0x14);
        *adresse |= (1 << 10);  // LED Gruen einschalten
        *adresse &= ~((1 << 8) | (1 << 9));  // LED Blau und Rot ausschalten
    } 
    }

    return 0;
}
