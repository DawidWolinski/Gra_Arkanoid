#include "PaletaStandardowa.h"
#include "Paleta.h"

PaletaStandardowa::PaletaStandardowa()
    : czasAnimacja(0.f), szerokoscPalety(0)
{

}

void PaletaStandardowa::animacja(Paleta& paleta, const float& dt)
{
    this->czasAnimacja += 5.f * dt;
    if (this->czasAnimacja > 0.6f)
    {
        this->czasAnimacja = 0.f;

        if (paleta.getTextureRect().left == 64)
            this->szerokoscPalety = 48;
        else
            this->szerokoscPalety = 32;

        if (paleta.getTextureRect().top == 40)
            paleta.setTextureRect(sf::IntRect(paleta.getTextureRect().left, 0, this->szerokoscPalety, 8));
        else
            paleta.setTextureRect(sf::IntRect(paleta.getTextureRect().left, paleta.getTextureRect().top + 8, this->szerokoscPalety, 8));
    }
}