#include "Boss.h"

Boss::Boss(ElementySilnikaWskaznik elementy, sf::Vector2f pozycjaRamki)
	: czyRysowac(true), hp(5), czasUderzenie(0.f), czasUmieranie(0.f), czasPocisk(0.f), mnoznik(1), top(144), element(elementy), czyOutro(false),
    Dol(Kierunek::Dol), Gora(Kierunek::Gora), Lewa(Kierunek::Lewa), Prawa(Kierunek::Prawa), Zaden(Kierunek::Zaden)
{
    this->dzwiekSmiercBoss.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_smierc_boss"));
    this->dzwiekSmiercBoss.setVolume(25.f);

	this->tlo.setSize(sf::Vector2f(64.f, 96.f));
	this->tlo.setFillColor(sf::Color::Black);
	this->tlo.setOrigin(sf::Vector2f(this->tlo.getGlobalBounds().width / 2.f, this->tlo.getGlobalBounds().height / 2.f));
	this->tlo.setPosition(sf::Vector2f(pozycjaRamki.x + 8.f + this->tlo.getGlobalBounds().width / 2.f, pozycjaRamki.y + 23.f + this->tlo.getGlobalBounds().height / 2.f));

	this->setTexture(this->element->zasoby.zwrocTeksture("tekstura_boss"));
	this->setTextureRect(sf::IntRect(0, 144, 62, 96));
	this->setOrigin(sf::Vector2f(this->getGlobalBounds().width / 2.f, this->getGlobalBounds().height / 2.f));
	this->setPosition(this->tlo.getPosition());

    //Hitbox bossa
	this->hitBox.setSize(sf::Vector2f(46.f, 90.f));
	this->hitBox.setOrigin(sf::Vector2f(this->hitBox.getGlobalBounds().width / 2.f, this->hitBox.getGlobalBounds().height / 2.f));
	this->hitBox.setPosition(sf::Vector2f(this->getPosition().x, this->getPosition().y));

    //Pociski
    this->pocisk.setTexture(this->element->zasoby.zwrocTeksture("tekstura_pocisk"));
    this->pocisk.setTextureRect(sf::IntRect(7, 63, 7, 8));

    this->animacjePociskow.push_back(sf::IntRect(7, 63, 7, 8));
    this->animacjePociskow.push_back(sf::IntRect(26, 62, 7, 8));
    this->animacjePociskow.push_back(sf::IntRect(45, 62, 7, 8));
    this->animacjePociskow.push_back(sf::IntRect(67, 61, 7, 8));
    this->animacjePociskow.push_back(sf::IntRect(88, 61, 7, 8));
    this->animacjePociskow.push_back(sf::IntRect(105, 61, 7, 8));
    this->animacjePociskow.push_back(sf::IntRect(7, 83, 7, 8));
    this->animacjePociskow.push_back(sf::IntRect(26, 83, 7, 8));
    this->animacjePociskow.push_back(sf::IntRect(48, 83, 8, 7));
    this->animacjePociskow.push_back(sf::IntRect(68, 83, 7, 8));
    this->animacjePociskow.push_back(sf::IntRect(87, 82, 8, 7));


    this->stan = &this->otwarcie;
}

Kierunek Boss::ktoraSciana(const sf::Sprite& obiekt, const sf::Vector2f& szybkosc)
{
    //Sprawdza od ktorej strony pilka uderzyla bossa

    float dx = std::abs(obiekt.getPosition().x - this->hitBox.getPosition().x) - this->hitBox.getGlobalBounds().width / 2.f;
    float dy = std::abs(obiekt.getPosition().y - this->hitBox.getPosition().y) - this->hitBox.getGlobalBounds().height / 2.f;

        if (dy < dx)
        {
            if (obiekt.getPosition().x > this->hitBox.getPosition().x && szybkosc.x < 0) 
                return Prawa;
            else if (obiekt.getPosition().x < this->hitBox.getPosition().x && szybkosc.x > 0) 
                return Lewa;
        }
        else
        {
            if (obiekt.getPosition().y > this->hitBox.getPosition().y && szybkosc.y < 0) 
                return Dol;
            else if (obiekt.getPosition().y < this->hitBox.getPosition().y && szybkosc.y > 0) 
                return Gora;
        }

    return Zaden;
}

void Boss::aktualizuj(const float& dt, std::unique_ptr<Paleta>& gracz)
{
    this->animacja(dt, gracz);

    //Ruch pociskow
    for (auto& pocisk : this->pociski)
        std::get<0>(pocisk)->move(*std::get<1>(pocisk) * dt * 100.f);
}

void Boss::rysuj()
{
    this->element->okno.draw(this->tlo);
	if (this->czyRysowac)
	    this->element->okno.draw(*this);

    for (auto& pocisk : this->pociski)
        this->element->okno.draw(*std::get<0>(pocisk));
}

void Boss::animacja(const float& dt, std::unique_ptr<Paleta>& gracz)
{
    this->stan->animacja(*this, *gracz, dt);
    if (!this->pociski.empty())
        this->animacjaPociski(dt);

    if (this->hp > 0)
    {
        this->animacjaUderzenie(dt);
        this->animacjaUmieranie(dt);
    }
}

void Boss::uderzenie()
{
    this->hp--;
    
    //Przy 3 hp boss zaczyna migac
    if (this->hp == 3)
        this->setTextureRect(sf::IntRect(this->getTextureRect().left, 336, 62, 96));

    if(this->top != 240) 
        this->top = this->getTextureRect().top;

    //Animacja uderzenia
    if(this->hp > 0)
        this->setTextureRect(sf::IntRect(this->getTextureRect().left, 240, 62, 96));
    else
    {
        while (!this->pociski.empty()) this->pociski.pop_back();
        this->stan = &this->smierc;
        this->dzwiekSmiercBoss.play();
    }

    this->czasUderzenie = 0.f;
}

void Boss::animacjaUderzenie(const float& dt)
{
    if (this->getTextureRect().top == 240)
    {
        this->czasUderzenie += 10.f * dt;

        if (this->czasUderzenie > 0.8f)
        {
            this->czasUderzenie = 0.f;
            this->setTextureRect(sf::IntRect(this->getTextureRect().left, this->top, 62, 96));
        }
    }
}

void Boss::animacjaUmieranie(const float& dt)
{
    if (this->getTextureRect().top != 240 && this->hp < 4)
    {
        this->czasUmieranie += 10.f * dt;
        if (this->czasUmieranie > 1.5f)
        {
            this->czasUmieranie = 0.f;

            this->setTextureRect(sf::IntRect(this->getTextureRect().left, this->getTextureRect().top + 96 * this->mnoznik, 62, 96));

            if (this->getTextureRect().top == 528)
                this->mnoznik = -1;
            else if (this->getTextureRect().top == 336)
                this->mnoznik = 1;
        }
    }
}

void Boss::animacjaPociski(const float& dt)
{
    this->czasPocisk += 10.f * dt;

    if (this->czasPocisk >= 0.5f)
    {
        for (auto& pocisk : this->pociski)
        {
            std::get<2>(pocisk)++;
            if (std::get<2>(pocisk) == this->animacjePociskow.size())
                std::get<2>(pocisk) = 0; 

            std::get<0>(pocisk)->setTextureRect(this->animacjePociskow[std::get<2>(pocisk)]);
            std::get<0>(pocisk)->setOrigin(sf::Vector2f(std::get<0>(pocisk)->getGlobalBounds().width / 2.f, std::get<0>(pocisk)->getGlobalBounds().height / 2.f));
        }

        this->czasPocisk = 0.f;
    }
}