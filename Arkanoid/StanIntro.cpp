#include "StanIntro.h"
#include <iostream>

///////////////		WORK IN PROGRESS	////////////////////


StanIntro::StanIntro(ElementySilnikaWskaznik elementy)
	: element(elementy), i(0), czasAnimacja(0.f), stopnie(250.f), promien(30.f), zamiennik(90.f), stalaSzybkosc(150.f), numerAnimacji(0),
	czasAnimacjaStatek(0.f)
{
	//this->szybkosc = sf::Vector2f(-this->stalaSzybkosc, this->stalaSzybkosc);
	this->szybkosc.x = -this->stalaSzybkosc * cos(this->radiany(30.f));
	this->szybkosc.y = -this->stalaSzybkosc * sin(this->radiany(30.f));
	
	std::cout << szybkosc.x << " " << szybkosc.y << std::endl;
}

void StanIntro::inicjalizuj()
{
	this->ustawTekstury();
	//std::cout << cos(0) << std::endl;
}


void StanIntro::ustawTekstury()
{
	this->element->zasoby.zaladujTeksture("tekstura_niebo", PLIK_TEKSTURA_NIEBO);
	this->element->zasoby.zaladujTeksture("tekstura_statek", PLIK_TEKSTURA_STATEK);

	this->animacje.push_back(sf::IntRect(37, 17, 44, 24));
	this->animacje.push_back(sf::IntRect(29, 70, 62, 41));
	this->animacje.push_back(sf::IntRect(20, 149, 64, 61));
	this->animacje.push_back(sf::IntRect(22, 238, 60, 58));
	this->animacje.push_back(sf::IntRect(175, 18, 54, 54));
	this->animacje.push_back(sf::IntRect(165, 101, 55, 55));
	this->animacje.push_back(sf::IntRect(167, 192, 53, 33));
	this->animacje.push_back(sf::IntRect(166, 254, 43, 43));

	this->malyStatek.setTexture(this->element->zasoby.zwrocTeksture("tekstura_statek"));
	this->malyStatek.setTextureRect(this->animacje[this->numerAnimacji]);
	this->malyStatek.setScale(0.0f, 0.0f);
	this->malyStatek.setPosition(sf::Vector2f(SZEROKOSC/2.f, 100.f));


	this->duzyStatek.setTexture(this->element->zasoby.zwrocTeksture("tekstura_statek_arkanoid"));
	this->duzyStatek.setTextureRect(sf::IntRect(1, 54, 192, 83));
	this->duzyStatek.setOrigin(sf::Vector2f(this->duzyStatek.getGlobalBounds().width / 2.f, this->duzyStatek.getGlobalBounds().height / 2.f));
	this->duzyStatek.setPosition(sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC - this->duzyStatek.getGlobalBounds().height/2.f - 15.f));

	this->tlo.setTexture(this->element->zasoby.zwrocTeksture("tekstura_niebo"));

	
}

void StanIntro::przechwytujWydarzenia(const float& dt)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (this->promien > 1)
			this->promien -= 60.f * dt;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		this->promien += 60.f * dt;
	}

	while (this->element->okno.pollEvent(this->element->wydarzenie))
	{
		switch (this->element->wydarzenie.type)
		{
		case sf::Event::Closed:
			this->element->okno.close();
			break;
		case sf::Event::Resized:
			this->element->okno.setPosition(sf::Vector2i(this->element->okno.getPosition().x - int(SZEROKOSC) / 2, this->element->okno.getPosition().y - int(WYSOKOSC) / 2));
			break;
		case sf::Event::KeyPressed:
			if (this->element->wydarzenie.key.code == sf::Keyboard::Escape)
				this->element->okno.close();
			if (this->element->wydarzenie.key.code == sf::Keyboard::Enter)
			{
				this->numerAnimacji++;
				this->malyStatek.setTextureRect(this->animacje[this->numerAnimacji]);
				std::cout << "animacja nr: " << this->numerAnimacji << " angle: " << this->stopnie << std::endl;
			}
			break;
		}
	}
}

float StanIntro::radiany(const float& stopnie)
{
	return (stopnie * this->pi) / 180.f;
}

void StanIntro::sekwencja(const float& dt)
{
	switch (this->i)
	{
	case 0:
		this->ruchStatku(dt);

		break;
	}
}

void StanIntro::ruchStatku(const float& dt)
{
	this->czasAnimacja += 15 * dt;

	if (this->stopnie > 90 && this->stopnie < 200)
		this->malyStatek.move(this->szybkosc * dt);

	else if(this->czasAnimacja > 0.1f)
	{
		this->stopnie += 1.f;
		if (this->stopnie > 360.f) 
			this->stopnie = 0.f;

		float x, y;
		x = SZEROKOSC/2.f + cos(this->radiany(this->stopnie)) * this->promien;
		y = 100.f + sin(this->radiany(this->stopnie)) * this->promien;

		this->malyStatek.setPosition(sf::Vector2f(x, y));

		this->czasAnimacja = 0.f;

		std::cout << this->stopnie << std::endl;
	}
}

void StanIntro::animacjaStatku(const float& dt)
{
	this->czasAnimacjaStatek += 10 * dt;

	if (this->czasAnimacjaStatek > 0.5)
	{
		if (this->duzyStatek.getTextureRect().left == 1)
			this->duzyStatek.setTextureRect(sf::IntRect(201, this->duzyStatek.getTextureRect().top, this->duzyStatek.getTextureRect().width, this->duzyStatek.getTextureRect().height));
		else
			this->duzyStatek.setTextureRect(sf::IntRect(1, this->duzyStatek.getTextureRect().top, this->duzyStatek.getTextureRect().width, this->duzyStatek.getTextureRect().height));

		this->czasAnimacjaStatek = 0.f;
	}
}

void StanIntro::aktualizuj(const float& dt)
{


	if (this->stopnie > 300 || this->stopnie <= 10)
	{
		this->promien += this->zamiennik * dt;
		if (this->stopnie == 310)
			this->malyStatek.setScale(0.02f, 0.02f);
		else if (this->stopnie == 315)
			this->malyStatek.setScale(0.04f, 0.04f);
		else if (this->stopnie == 320)
			this->malyStatek.setScale(0.06f, 0.06f);
		else if (this->stopnie == 325)
			this->malyStatek.setScale(0.08f, 0.08f);
		else if (this->stopnie == 330)
			this->malyStatek.setScale(0.1f, 0.1f);
		else if (this->stopnie == 335)
			this->malyStatek.setScale(0.12f, 0.12f);
		else if (this->stopnie == 340)
			this->malyStatek.setScale(0.14f, 0.14f);
		else if (this->stopnie == 345)
			this->malyStatek.setScale(0.16f, 0.16f);
		else if (this->stopnie == 350)
			this->malyStatek.setScale(0.18f, 0.18f);
		else if (this->stopnie == 355)
			this->malyStatek.setScale(0.2f, 0.2f);
	}
		
	else if (this->stopnie > 20 && this->stopnie < 60)
		this->promien -= this->zamiennik * dt;
	else if (this->stopnie >= 60 && this->stopnie < 90)
	{
		if (this->zamiennik > 30) this->zamiennik-=0.75f;
		this->promien -= this->zamiennik * dt;
	}


	if (this->stopnie == 340 || this->stopnie == 359 || this->stopnie == 10 || this->stopnie == 20 || 
		this->stopnie == 40 || this->stopnie == 60 || this->stopnie == 85)
	{
		this->numerAnimacji++;
		this->malyStatek.setTextureRect(this->animacje[this->numerAnimacji]);
	}

	this->animacjaStatku(dt);
	this->ruchStatku(dt);
}

void StanIntro::rysuj(const float& dt)
{
	this->element->okno.clear();

	this->element->okno.draw(this->tlo);

	this->element->okno.draw(this->duzyStatek);

	this->element->okno.draw(this->malyStatek);

	this->element->gornyInterface.rysuj();

	this->element->okno.display();
}
