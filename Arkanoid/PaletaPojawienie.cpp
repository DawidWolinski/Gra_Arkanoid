#include "PaletaPojawienie.h"
#include "Paleta.h"

void PaletaPojawienie::animacja(Paleta& paleta, const float& dt)
{
    this->czasAnimacja += 10.f * dt;
    if (this->czasAnimacja > 0.5f)
    {
        this->czasAnimacja = 0.f;

        if (paleta.getTextureRect().left != 0)
            paleta.setTextureRect(sf::IntRect(0, 0, 32, 8));

        if (paleta.getTextureRect().top == 32)
        {
            paleta.setTextureRect(sf::IntRect(32, 0, 32, 8));
            paleta.stan = &paleta.standardowa;
        }
        else
            paleta.setTextureRect(sf::IntRect(paleta.getTextureRect().left, paleta.getTextureRect().top + 8, 32, 8));
    }
}
