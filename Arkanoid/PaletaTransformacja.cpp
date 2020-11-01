#include "PaletaTransformacja.h"
#include "Paleta.h"


PaletaTransformacja::PaletaTransformacja()
    : wyczyszczone(true)
{
    std::stack<sf::IntRect> tymczasowe;
    this->klatkiTransformacja.push_back(tymczasowe);
    this->klatkiTransformacja.push_back(tymczasowe);
    this->wytnijTekstury();
}

void PaletaTransformacja::animacja(Paleta& paleta, const float& dt)
{
    this->czasAnimacja += 10.f * dt;
    if (this->czasAnimacja > 0.4f)
    {
        this->czasAnimacja = 0.f;

        //Usuwa obecna transformacje
        if (!this->klatki.empty() && !this->wyczyszczone)
        {           
            paleta.setTextureRect(this->klatki.top());
            this->klatki.pop();
            paleta.setOrigin(sf::Vector2f(paleta.getGlobalBounds().width / 2.f, paleta.getGlobalBounds().height / 2.f));
            if (this->klatki.empty()) this->wyczyszczone = true;
        }
        //Transformacja
        else if (paleta.rodzajPrzejscia != 0)
        {      
            this->klatki.push(std::move(this->klatkiTransformacja[paleta.rodzajPrzejscia - 1].top()));
            this->klatkiTransformacja[paleta.rodzajPrzejscia - 1].pop();
            paleta.setTextureRect(this->klatki.top());
            paleta.setOrigin(sf::Vector2f(paleta.getGlobalBounds().width / 2.f, paleta.getGlobalBounds().height / 2.f));         
            if (this->klatkiTransformacja[paleta.rodzajPrzejscia - 1].empty())
            {
                paleta.stan = &paleta.standardowa;
                this->wyczyszczone = false;
                this->wytnijTekstury();
            }
        }
        else
            paleta.stan = &paleta.standardowa;
    }
}

void PaletaTransformacja::wytnijTekstury()
{

    //LASER
    if (this->klatkiTransformacja[0].empty())
    {
        this->klatkiTransformacja[0].push(sf::IntRect(144, 0, 32, 8));
        for (int i = 8; i >= 0; i--)
            this->klatkiTransformacja[0].push(sf::IntRect(112, i * 8, 32, 8));       

        this->klatkiTransformacja[0].push(sf::IntRect(32, 0, 32, 8));
    }

    //POWIEKSZENIE
    if (this->klatkiTransformacja[1].empty())
    {
        this->klatkiTransformacja[1].push(sf::IntRect(64, 0, 48, 8));
        this->klatkiTransformacja[1].push(sf::IntRect(208, 34, 44, 8));
        this->klatkiTransformacja[1].push(sf::IntRect(210, 42, 40, 8));
        this->klatkiTransformacja[1].push(sf::IntRect(212, 50, 36, 8));
        this->klatkiTransformacja[1].push(sf::IntRect(213, 58, 34, 8));
        this->klatkiTransformacja[1].push(sf::IntRect(32, 0, 32, 8));    
    }
}

void PaletaTransformacja::resetuj()
{
    while (!this->klatki.empty())
        this->klatki.pop();

    for (auto klatki : this->klatkiTransformacja)
        while (!klatki.empty())
            klatki.pop();

    this->wyczyszczone = true;

    this->wytnijTekstury();
}
