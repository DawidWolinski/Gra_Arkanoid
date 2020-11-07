#include "Bonus.h"

Bonus::Bonus(ElementySilnikaWskaznik elementy)
    :   rodzaj(10), aktywnyRodzaj(10), czyWRuchu(false), czyRysowac(true), czyPrzejscie(false),
        szybkosc(sf::Vector2f(0.f, 70.f)), szybkoscLasera(sf::Vector2f(0.f, -300.f)),
        zielonyCzas(0.f), zielonyLimit(5.f), losuj(0, 6), losujPrzejscie(0, 1), element(elementy)
{
    this->ustawTeksture();
    this->setPosition(sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC + this->getGlobalBounds().height/2.f + 1.f));
    this->cien.setPosition(this->getPosition().x + 4, this->getPosition().y + 4);
    this->przejscie.setPosition(SZEROKOSC - this->przejscie.getGlobalBounds().width, WYSOKOSC - this->przejscie.getGlobalBounds().height);
    this->odlegloscLaserow = std::abs(this->szybkoscLasera.y) / 5.f;
    this->dzwiekWydluzenie.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_wydluzenie"));
    this->dzwiekWydluzenie.setVolume(25.f);
    this->dzwiekLaser.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_laser"));
    this->dzwiekLaser.setVolume(25.f);

}

void Bonus::ustawTeksture()
{
    this->setTexture(this->element->zasoby.zwrocTeksture("tekstura_bonus"));
    this->setTextureRect(sf::IntRect(0, 0, 16, 7));
    this->setOrigin(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f);

    this->cien.setTexture(*this->getTexture());
    this->cien.setTextureRect(this->getTextureRect());
    this->cien.setColor(sf::Color(0, 0, 0, 250));
    this->cien.setOrigin(this->getOrigin());

    this->przejscie.setTexture(this->element->zasoby.zwrocTeksture("tekstura_tlo"));
    this->przejscie.setTextureRect(sf::IntRect(924, 256, 8, 40));
}

void Bonus::zrzuc(const sf::Vector2f& pozycja)
{
    if (!this->czyWRuchu)
    {
       //Losowanie bonusu
       //0 - Spowolnienie, 1 - Lapanie pilki, 2 - Laser, 3 - Wydluzenie, 4 - Trzy pilki, 5 - Pominiecie poziomu, 6 - Dodatkowe zycie

       int tymczasowa = 0;
        this->czyWRuchu = true;
        do
        {
             tymczasowa = this->losuj(this->element->generator);

             //Pominiecie poziomu (5) ma mniejsza szanse na wypadniecie
             if (tymczasowa == 5)
                 if (this->losujPrzejscie(this->element->generator) == 0)
                     tymczasowa = aktywnyRodzaj;
                     
        } while (tymczasowa == aktywnyRodzaj || tymczasowa == rodzaj || tymczasowa == 5 && this->przejscie.getTextureRect().left != 924);
        this->rodzaj = tymczasowa;
        this->setPosition(pozycja);
        this->setTextureRect(sf::IntRect(0, this->rodzaj * 8, 16, 7));
    }
}

void Bonus::aktualizuj(const float& dt)
{
    if (this->czyWRuchu)
    {
        if (this->getPosition().y < WYSOKOSC + this->getGlobalBounds().height / 2.f)
        {
            this->animacja(dt);
            this->move(this->szybkosc * dt);
            this->cien.setPosition(this->getPosition().x + 2, this->getPosition().y + 2);
        }
        else
            this->czyWRuchu = false;
    }
    
    this->animacjaPrzejscie(dt);
    this->aktualizujLasery(dt);
}

void Bonus::aktywujBonus(std::unique_ptr<Paleta>& gracz, std::unique_ptr<Pilka>& pilka)
{
    this->setPosition(sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC + this->getGlobalBounds().height / 2.f + 1.f));
    this->cien.setPosition(this->getPosition().x + 4, this->getPosition().y + 4);
    this->czyWRuchu = false;
    this->aktywnyRodzaj = this->rodzaj;
    pilka->czyWRuchu = true;

    if (this->rodzaj == 2 || this->rodzaj == 3)
        gracz->rodzajPrzejscia = this->rodzaj - 1;
    else gracz->rodzajPrzejscia = 0;

    gracz->stan = &gracz->transformacja;
    this->zielonyCzas = 0.f;
    this->element->gornyInterface.dodajPunkty(1000);
    if (this->rodzaj == 3) 
        this->dzwiekWydluzenie.play();
}

void Bonus::rysuj()
{
    if (this->czyRysowac)
    {
        this->element->okno.draw(this->cien);
        this->element->okno.draw(*this);
    }
 
    this->element->okno.draw(this->przejscie);

    for (auto& laser : this->lasery)
        this->element->okno.draw(*laser);
}

void Bonus::animacja(const float& dt)
{
    this->czasAnimacja += 10.f * dt;
    if (this->czasAnimacja > 1.f)
    {
        this->czasAnimacja = 0.f;

        if (this->getTextureRect().left == 112)
            this->setTextureRect(sf::IntRect(0, this->getTextureRect().top, this->getTextureRect().width, this->getTextureRect().height));
        else
            this->setTextureRect(sf::IntRect(this->getTextureRect().left + 16, this->rodzaj * 8, 16, 7));
    }
}

void Bonus::strzelLaserem(const sf::Vector2f& pozycjaPalety)
{
    if (this->lasery.empty() || pozycjaPalety.y > this->odlegloscLaserow + this->lasery.back()->getPosition().y)
    {
        this->lasery.push_back(std::make_unique<Laser>(this->element, pozycjaPalety));
        this->dzwiekLaser.play();
    }   
}

void Bonus::aktualizujLasery(const float& dt)
{
    for (unsigned int i = 0; i < this->lasery.size(); i++)
    {
        this->lasery[i]->aktualizuj(dt);
        if(this->lasery[i]->usun)
            this->lasery.erase(this->lasery.begin() + i);
    }
        
}

void Bonus::animacjaPrzejscie(const float& dt)
{
    //Animacja portalu do naastepnego poziomu
    if (this->czyPrzejscie)
    {
        this->czasAnimacjaPrzejscie += 10.f * dt;
        if (this->czasAnimacjaPrzejscie > 0.5f)
        {
            this->czasAnimacjaPrzejscie = 0.f;

            if (this->przejscie.getTextureRect().left == 980)   //Gdy jest otwarty
                this->przejscie.setTextureRect(sf::IntRect(this->przejscie.getTextureRect().left - 16, this->przejscie.getTextureRect().top, this->przejscie.getTextureRect().width, this->przejscie.getTextureRect().height));
            else                                                //Otwieranie oraz gdy jest otwarty
                this->przejscie.setTextureRect(sf::IntRect(this->przejscie.getTextureRect().left + 8, this->przejscie.getTextureRect().top, this->przejscie.getTextureRect().width, this->przejscie.getTextureRect().height));
        }
    }   
}


//LASER
Laser::Laser(ElementySilnikaWskaznik elementy, sf::Vector2f pozycjaPalety)
    :czyKolizja(false), usun(false), czasAnimacja(0.f), szybkosc(sf::Vector2f(0.f, -300.f)), element(elementy)
{
    this->setTexture(this->element->zasoby.zwrocTeksture("tekstura_paleta"));
    this->setTextureRect(sf::IntRect(152, 48, 16, 8));
    this->setOrigin(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f);
    this->setPosition(pozycjaPalety);
}

void Laser::aktualizuj(const float& dt)
{
    if (!this->czyKolizja)                              //Ruch lasera (dopoki nie dojdzie do kolizji z cegla lub sufitem)
    {
        this->move(this->szybkosc * dt);
        if (this->getPosition().y <= 30.f)
            this->czyKolizja = true;
    }
    else if (this->getTextureRect().top == 48)          //Pierwsza klatka animacji wybuchu lasera
    {
        this->setTextureRect(sf::IntRect(153, 56, 14, 3));
        this->setPosition(sf::Vector2f(this->getPosition().x, this->getPosition().y - this->getGlobalBounds().height - 2.f));
        this->setOrigin(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f);
    }
    else
    {
        this->czasAnimacja += dt * 10.f;

        if (this->czasAnimacja >= 0.5f)
        {
            if (this->getTextureRect().top == 56)       //Druga klatka animacji wybuchu lasera
                this->setTextureRect(sf::IntRect(152, 64, 16, 3));
            else                                        //Usuniecie (z poziomu bonusu, w funkcji aktualizacji)
                this->usun = true;          

            this->setOrigin(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f);
            this->czasAnimacja = 0.f;
        }
    }
}