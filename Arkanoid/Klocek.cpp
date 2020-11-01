#include "Klocek.h"

int Klocek::ileZlotych;
std::vector<sf::Vector2f> Klocek::usunieteKlocki;


Klocek::Klocek(int rodzaj, float x, float y, ElementySilnikaWskaznik elementy, int ktoryPoziom, int hp)
    :rodzaj(rodzaj), klocekObok{}, animuj(false), hp(hp), element(elementy),
    Dol(Kierunek::Dol), Gora(Kierunek::Gora), Lewa(Kierunek::Lewa), Prawa(Kierunek::Prawa), Zaden(Kierunek::Zaden)
{
    this->ustawTeksture(ktoryPoziom); 
    this->setOrigin(sf::Vector2f(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f));
    this->setPosition(sf::Vector2f(x, y));

    this->ustawCien();   
}

void Klocek::ustawTeksture(int ktoryPoziom)
{
    this->setTexture(this->element->zasoby.zwrocTeksture("tekstura_klocek"));
    switch (this->rodzaj)
    {
    case 0:
        this->setTextureRect(sf::IntRect(0, 0, 16, 8));     ////    BIALY
        hp = 1;
        this->ilePunktow = 50;
        break;
    case 1:
        this->setTextureRect(sf::IntRect(16, 0, 16, 8));    ////    POMARANCZOWY
        hp = 1;
        this->ilePunktow = 60;
        break;
    case 2:
        this->setTextureRect(sf::IntRect(32, 0, 16, 8));     ////    BLEKITNY
        hp = 1;
        this->ilePunktow = 70;
        break;
    case 3:
        this->setTextureRect(sf::IntRect(48, 0, 16, 8));     ////    ZIELONY
        hp = 1;
        this->ilePunktow = 80;
        break;
    case 4:
        this->setTextureRect(sf::IntRect(0, 8, 16, 8));     ////    CZERWONY
        hp = 1;
        this->ilePunktow = 90;
        break;
    case 5:
        this->setTextureRect(sf::IntRect(16, 8, 16, 8));     ////    NIEBIESKI
        hp = 1;
        this->ilePunktow = 100;
        break;
    case 6:
        this->setTextureRect(sf::IntRect(32, 8, 16, 8));     ////    ROZOWY
        hp = 1;
        this->ilePunktow = 110;
        break;
    case 7:
        this->setTextureRect(sf::IntRect(48, 8, 16, 8));     ////    ZOLTY
        hp = 1;
        this->ilePunktow = 120;
        break;
    case 8:
        this->setTextureRect(sf::IntRect(0, 16, 16, 8));     ////    ZELAZNY
        if (hp == 0)
            hp = 2 + static_cast<int>(std::floor(ktoryPoziom / 8));
        this->ilePunktow = 50 * ktoryPoziom;
        break;
    case 9:
        this->setTextureRect(sf::IntRect(0, 24, 16, 8));     ////    ZLOTY
        hp = 100;
        this->ilePunktow = 0;
        break;
    default:
        this->setTextureRect(sf::IntRect(0, 0, 16, 8));
        hp = 1;
        this->ilePunktow = 50;
    }  
}

void Klocek::ustawCien()
{
    this->cien.setTexture(this->element->zasoby.zwrocTeksture("tekstura_klocek"));

    if (this->getPosition().x < 208) 
        this->cien.setTextureRect(sf::IntRect(0, 0, 16, 8));
    //Cien klockow dotykajacych prawej sciany jest uciety
    else 
        this->cien.setTextureRect(sf::IntRect(0, 0, 8, 8));

    this->cien.setColor(sf::Color(0, 0, 0, 100));
    this->cien.setPosition(sf::Vector2f(this->getPosition().x, this->getPosition().y + this->getGlobalBounds().height / 2.f));
}

void Klocek::aktualizuj(const float& dt)
{
    this->animacja(dt);
}

void Klocek::rysuj()
{
    this->element->okno.draw(this->cien);
    this->element->okno.draw(*this);  
}

void Klocek::sprawdzKlockiObok(const std::vector<std::unique_ptr<Klocek>>& klocki)
{
    //Sprawdza, czy dany klocek sasiaduje z innymi
    for (auto& klocek : klocki)
    { 
        if (this->getPosition().y == klocek->getPosition().y)
        {
            if (this->getPosition().x == klocek->getPosition().x - klocek->getGlobalBounds().width) 
                this->klocekObok[static_cast<int>(Prawa)] = true;
            else if (this->getPosition().x == klocek->getPosition().x + klocek->getGlobalBounds().width) 
                this->klocekObok[static_cast<int>(Lewa)] = true;
        }
        else if (this->getPosition().x == klocek->getPosition().x)
        {
            if (this->getPosition().y == klocek->getPosition().y - klocek->getGlobalBounds().height) 
                this->klocekObok[static_cast<int>(Dol)] = true;
            else if (this->getPosition().y == klocek->getPosition().y + klocek->getGlobalBounds().height) 
                this->klocekObok[static_cast<int>(Gora)] = true;
        }      
    }

    //Sciana (lewa i prawa) rowniez traktowana jest jako klocek 
    if (this->getPosition().x - this->getGlobalBounds().width/2.f == 8.f)
        this->klocekObok[static_cast<int>(Lewa)] = true;
    else if (this->getPosition().x + this->getGlobalBounds().width/2.f == 216.f)
        this->klocekObok[static_cast<int>(Prawa)] = true;

}

void Klocek::sprawdzKlockiObok()
{
    //Sprawdza, czy zniszczony klocek sasiadowal z tym klockiem
    for (auto& pozycja : this->usunieteKlocki)
    {
        if (this->getPosition().y == pozycja.y)
        {
            if (this->getPosition().x + this->getGlobalBounds().width == pozycja.x)
                this->klocekObok[static_cast<int>(Prawa)] = false;
            else if (this->getPosition().x - this->getGlobalBounds().width == pozycja.x)
                this->klocekObok[static_cast<int>(Lewa)] = false;
        }
        else if (this->getPosition().x == pozycja.x)
        {
            if (this->getPosition().y + this->getGlobalBounds().height == pozycja.y)
                this->klocekObok[static_cast<int>(Dol)] = false;
            else if (this->getPosition().y - this->getGlobalBounds().height == pozycja.y)
                this->klocekObok[static_cast<int>(Gora)] = false;
        }
    }
}

Kierunek Klocek::ktoraSciana(const sf::Vector2f& pozycjaPilki, const sf::Vector2f& szybkosc)
{
    //Sprawdza od ktorej strony pilka uderzyla w klocek

    float dx = std::abs(pozycjaPilki.x - this->getPosition().x) - this->getGlobalBounds().width / 2.f; //- obiekt.getGlobalBounds().width/2.f;
    float dy = std::abs(pozycjaPilki.y - this->getPosition().y) - this->getGlobalBounds().height / 2.f; //- obiekt.getGlobalBounds().height / 2.f;

        if (dy < dx)
        {
            if (pozycjaPilki.x > this->getPosition().x && !this->klocekObok[static_cast<int>(Prawa)] && szybkosc.x < 0)
                return Prawa;
            else if (pozycjaPilki.x < this->getPosition().x && !this->klocekObok[static_cast<int>(Lewa)] && szybkosc.x > 0)
                return Lewa;
        }
        else
        {
            if (pozycjaPilki.y > this->getPosition().y && !this->klocekObok[static_cast<int>(Dol)] && szybkosc.y < 0)
                return Dol;
            else if (pozycjaPilki.y < this->getPosition().y && !this->klocekObok[static_cast<int>(Gora)] && szybkosc.y > 0)
                return Gora;
        }  
    
    return Zaden;
}

void Klocek::uderzenie()
{
    if (this->hp != 100) this->hp--;

    if (this->hp == 0) 
        this->usunieteKlocki.push_back(this->getPosition());
    else 
        this->animuj = true;
}

void Klocek::animacja(const float& dt)
{
    //Animacja uderzenia dla srebrnych i zlotych klockow
    if (this->animuj)
    {
        this->czasAnimacja += 6.f * dt;
        if (this->czasAnimacja > 0.5f)
        {
            if (this->getTextureRect().left == 80)
            {
                this->setTextureRect(sf::IntRect(0, this->getTextureRect().top, 16, 8));
                this->animuj = false;
            }  
            else
                this->setTextureRect(sf::IntRect(this->getTextureRect().left + 16, this->getTextureRect().top, 16, 8));
            this->czasAnimacja -= this->czasAnimacja;
        }
    }
}