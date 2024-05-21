/**
 * EGR Praktikum
 * Termin 1 - Aufgabe 1
 */

#include <stdint.h>

int main(void)
{
    // place code to run once here
    // Aktivierung des GPIO A Moduls
    uint32_t volatile *adresse = (uint32_t *)(0x40021000 + 0x4C);
    *adresse |=(1 << 0);

    // LED Pin als Output konfigurieren
    adresse = (uint32_t *)(0x48000000 + 0x00);
    *adresse &= ~(1 << 17);
    *adresse |= (1 << 16);

    // LED Pin als push-pull einstellen
    adresse = (uint32_t *)(0x48000000 + 0x04);
    *adresse &= ~(1 << 8);

    // LED anschalten
    adresse = (uint32_t *)(0x48000000 + 0x14);
    *adresse |= (1 << 8);

    

    while (1)
    {
        // place your loop code here
    }

    return 0;
}