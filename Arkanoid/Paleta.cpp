#include "Paleta.h"

Paleta::Paleta(ElementySilnikaWskaznik elementy)
    :czyWRuchu(false), czyRysowac(false), ruchWPrawo(false), szybkosc(300.f, 0.f), rodzajPrzejscia(3), czyPrzechodzi(false), smierc(false), 
    element(elementy)
{
    this->ustawTeksture();

    this->stan = &this->pojawienie;

    this->setPosition(sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC - 19.f)); 
    this->cien.setPosition(this->getPosition().x + 4, this->getPosition().y + 4);
}

void Paleta::ustawTeksture()
{
    this->setTexture(this->element->zasoby.zwrocTeksture("tekstura_paleta"));
    this->setTextureRect(sf::IntRect(208, 0, 32, 8));
    this->setOrigin(sf::Vector2f(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f));

    this->cien.setTexture(*this->getTexture());
    this->setTextureRect(this->getTextureRect());
    this->cien.setColor(sf::Color(0, 0, 0, 250));
    this->cien.setOrigin(this->getOrigin());
}

void Paleta::aktualizuj(const float& dt)
{
    if (this->czyRysowac)
    {
        this->animacja(dt);
        this->aktualizujCien();

        if (this->czyPrzechodzi)
        {
            this->czyWRuchu = false;
            if (this->getGlobalBounds().left < SZEROKOSC)
                this->move(sf::Vector2f(50.f, 0.f) * dt);
            else
            {
                this->czyPrzechodzi = false;
                this->czyRysowac = false;
            }         
        }
    }
}

void Paleta::aktualizujCien()
{
    this->cien.setTextureRect(this->getTextureRect());
    this->cien.setOrigin(this->getOrigin());
    this->cien.setPosition(this->getPosition().x + 4, this->getPosition().y + 4);
}

void Paleta::rysuj()
{
   if (this->czyRysowac)
    {
        this->element->okno.draw(this->cien);
        this->element->okno.draw(*this);
    }
}

void Paleta::animacja(const float& dt)
{
    this->stan->animacja(*this, dt);
}

int Paleta::miejsceKolizji(const sf::Sprite& obiekt)
{
    //Sprawdza, w ktore miejsce na palecie uderzyla pilka

    if (obiekt.getPosition().y < this->getPosition().y + this->getGlobalBounds().height/4.f)
    {
        if (obiekt.getPosition().x <= this->getGlobalBounds().left + 4.f)                                           
            return 1;           //POMARANCZOWY LEWY
        else if (obiekt.getPosition().x >= this->getGlobalBounds().left + this->getGlobalBounds().width - 4.f)      
            return 2;           //POMARANCZOWY PRAWY
        else
        {
            if (obiekt.getPosition().x <= this->getGlobalBounds().left + 9.f)                                       
                return 3;       //CZERWONY LEWY
            else if (obiekt.getPosition().x >= this->getGlobalBounds().left + this->getGlobalBounds().width - 9.f)  
                return 4;       //CZERWONY PRAWY
            else
            {
                if (obiekt.getPosition().x < this->getPosition().x)         
                    return 5;   //SREBRNY LEWY
                else if (obiekt.getPosition().x > this->getPosition().x)    
                    return 6;   //SREBRNY PRAWY
                else                                                        
                    return 7;   //SRODEK
            }
        }
    }
    return 0;
}

void Paleta::zlapPilke(sf::Sprite& pilka)
{
    if (pilka.getGlobalBounds().left < this->getGlobalBounds().left)
        pilka.setPosition(sf::Vector2f(pilka.getPosition().x - 2.f + (this->getGlobalBounds().left - pilka.getGlobalBounds().left), this->getPosition().y - this->getGlobalBounds().height / 2.f - pilka.getGlobalBounds().height / 2.f));
    else if (pilka.getGlobalBounds().left + pilka.getGlobalBounds().width > this->getGlobalBounds().left + this->getGlobalBounds().width)
        pilka.setPosition(sf::Vector2f(pilka.getPosition().x + 2.f - ((pilka.getGlobalBounds().left + pilka.getGlobalBounds().width) - (this->getGlobalBounds().left + this->getGlobalBounds().width)), this->getPosition().y - this->getGlobalBounds().height / 2.f - pilka.getGlobalBounds().height / 2.f));
    else
        pilka.setPosition(sf::Vector2f(pilka.getPosition().x, this->getPosition().y - this->getGlobalBounds().height / 2.f - pilka.getGlobalBounds().height / 2.f));

}
