#include "PaletaSmierc.h"
#include "Paleta.h"

PaletaSmierc::PaletaSmierc()
    : i(0)
{
    this->wytnijTekstury();
}

void PaletaSmierc::animacja(Paleta& paleta, const float& dt)
{
    this->czasAnimacja += 10.f * dt;
    if (this->czasAnimacja > 0.9f && !paleta.smierc)
    {
        this->czasAnimacja = 0.f;
        paleta.czyWRuchu = false;

        paleta.setTextureRect(this->klatki[i]);
        paleta.setOrigin(sf::Vector2f(paleta.getGlobalBounds().width / 2.f, paleta.getGlobalBounds().height / 2.f));

        i++;
        if (i >= this->klatki.size())
        {
            i = 0;
            paleta.smierc = true;
            paleta.czyRysowac = false;
            paleta.setPosition(SZEROKOSC + paleta.getGlobalBounds().width, paleta.getPosition().y);       
        }
            
    }
}

void PaletaSmierc::wytnijTekstury()
{
    if (this->klatki.empty())
    {
        this->klatki.push_back(sf::IntRect(176, 0, 32, 8));
        this->klatki.push_back(sf::IntRect(176, 8, 32, 8));
        this->klatki.push_back(sf::IntRect(176, 16, 32, 8));
        this->klatki.push_back(sf::IntRect(24, 76, 32, 24));
        this->klatki.push_back(sf::IntRect(69, 76, 37, 24));        
        this->klatki.push_back(sf::IntRect(116, 76, 41, 24));
        this->klatki.push_back(sf::IntRect(163, 79, 44, 21)); 
    } 
}