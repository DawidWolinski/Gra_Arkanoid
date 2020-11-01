#include "Pilka.h"



Pilka::Pilka(ElementySilnikaWskaznik elementy)
    :czyWRuchu(false), stalaSzybkosc(150.f), pi(atan(1.f) * 4.f), odlegloscOdPalety(0.f), czyRysowac(false), odbita(true),
    Dol(Kierunek::Dol), Gora(Kierunek::Gora), Lewa(Kierunek::Lewa), Prawa(Kierunek::Prawa),
    losuj(0, 2), element(elementy)
{
    this->szybkosc = sf::Vector2f(-this->stalaSzybkosc, this->stalaSzybkosc);
    this->ustawTeksture();
    
    this->setPosition(sf::Vector2f(SZEROKOSC, WYSOKOSC));
    this->zmienKat(140.f);
    
}

void  Pilka::ustawTeksture()
{
    this->setTexture(this->element->zasoby.zwrocTeksture("tekstura_paleta"));
    this->setTextureRect(sf::IntRect(0, 40, 5, 4));
    this->setOrigin(sf::Vector2f(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f));
    this->cien.setTexture(*this->getTexture());
    this->cien.setTextureRect(sf::IntRect(4, 44, 5, 4));
    this->cien.setOrigin(this->getOrigin());
}

void Pilka::aktualizuj(const float& dt, const sf::Vector2f& rozmiarOkna)
{
    this->ruch(dt, rozmiarOkna);  
}

void Pilka::ruch(const float& dt, const sf::Vector2f& rozmiarOkna)
{
    if (this->czyWRuchu)
    {
        this->move(szybkosc * dt);
        if (this->getPosition().x + this->getGlobalBounds().width / 2.f >= rozmiarOkna.x - 8.f) this->odbij(Lewa);
        if (this->getPosition().x - this->getGlobalBounds().width / 2.f <= 8.f) this->odbij(Prawa);
        if (this->getPosition().y - this->getGlobalBounds().height / 2.f <= 24.f) this->odbij(Dol);
    }

    this->cien.setPosition(this->getPosition().x + 4, this->getPosition().y + 4);
}

void Pilka::rysuj()
{
    if (this->czyRysowac) 
    { 
        this->element->okno.draw(*this); 
    }
}

void Pilka::rysujCien()
{
    if(this->czyRysowac)
        this->element->okno.draw(this->cien);
}

void Pilka::zmienKat(const float& stopnie)
{
    this->stopnie = stopnie;
    float radiany = 2 * pi * (this->stopnie / 360);

    if (this->szybkosc.x >= 0)
        this->szybkosc.x = this->stalaSzybkosc * cos(radiany);
    else
        this->szybkosc.x = -this->stalaSzybkosc * cos(radiany);
    if (this->szybkosc.y >= 0)
        this->szybkosc.y = this->stalaSzybkosc * sin(radiany);  
    else
        this->szybkosc.y = -this->stalaSzybkosc * sin(radiany);
}

void Pilka::odbij(Kierunek kierunekOdbicia, const float& stopnie)
{
    if (stopnie != 0)
        this->zmienKat(stopnie);
 
    switch (kierunekOdbicia)
    {
    case Kierunek::Prawa:
        this->szybkosc.x = std::abs(this->szybkosc.x);
        break;
    case Kierunek::Lewa:
        this->szybkosc.x = -std::abs(this->szybkosc.x);
        break;
    case Kierunek::Dol:
        this->szybkosc.y = std::abs(this->szybkosc.y);
        break;
    case Kierunek::Gora:
        this->szybkosc.y = -std::abs(this->szybkosc.y);
        break;
    }
}

void Pilka::odbijPaleta(const int& miejsceUderzenia)
{
    switch (miejsceUderzenia)
    {
    case 1:     //POMARANCZOWY LEWY
        this->odbij(Lewa, 30);
        this->odbij(Gora);
        break;
    case 2:     //POMARANCZOWY PRAWY
        this->odbij(Prawa, 30);
        this->odbij(Gora);
        break;
    case 3:     //CZERWONY LEWY
        this->odbij(Lewa, 45);
        this->odbij(Gora);
        break;
    case 4:     //CZERWONY PRAWY
        this->odbij(Prawa, 45);
        this->odbij(Gora);
        break;
    case 5:     //SREBRNY LEWY
        this->odbij(Lewa, 65);
        this->odbij(Gora);
        break;
    case 6:     //SREBRNY PRAWY
        this->odbij(Prawa, 65);
        this->odbij(Gora);
        break;
    case 7:     //SRODEK
        this->zmienKat(65);
        this->odbij(Gora);
        break;
    default:
        break;
    }
}

void Pilka::ustawNaPalecie(const sf::Sprite& paleta)
{
    this->setPosition(paleta.getPosition().x, paleta.getPosition().y - paleta.getGlobalBounds().height / 2.f - this->getGlobalBounds().height / 2.f);
}

void Pilka::zwiekszSzybkosc()
{
    if (this->stalaSzybkosc <= 300.f && this->losuj(this->element->generator) == 1)
    {
        this->stalaSzybkosc += 5.f;
        this->szybkosc.x = this->stalaSzybkosc * cos(std::atan2(this->szybkosc.y, this->szybkosc.x));
        this->szybkosc.y = this->stalaSzybkosc * sin(std::atan2(this->szybkosc.y, this->szybkosc.x));
    }
        
}

void Pilka::zmniejszSzybkosc(float ile)
{
    this->stalaSzybkosc -= ile;
    if (this->stalaSzybkosc < 150.f) 
        this->stalaSzybkosc = 150.f;

    this->szybkosc.x = this->stalaSzybkosc * cos(std::atan2(this->szybkosc.y, this->szybkosc.x));
    this->szybkosc.y = this->stalaSzybkosc * sin(std::atan2(this->szybkosc.y, this->szybkosc.x));
}