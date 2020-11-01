#include "BossStrzelanie.h"
#include "Boss.h"
#include "Paleta.h"


BossStrzelanie::BossStrzelanie()
	: czasAnimacja(0.f), czasStrzal(0.f)
{
}

void BossStrzelanie::animacja(Boss& boss, Paleta& paleta, const float& dt)
{
	this->czasAnimacja += 10.f * dt;
	this->czasStrzal += 10.f * dt;


	if (this->czasAnimacja > 30.f)
	{
		this->czasAnimacja = 0.f;

		boss.stan = &boss.zamkniecie;
	}
	else if(this->czasStrzal > 5.f)
	{
		this->czasStrzal = 0.f;

		boss.pociski.push_back(std::make_tuple<std::unique_ptr<sf::Sprite>, std::unique_ptr<sf::Vector2f>>(std::make_unique<sf::Sprite>(boss.pocisk), std::make_unique<sf::Vector2f>(this->zwrocKierunek(boss.getPosition(), paleta)), 0));
		std::get<0>(boss.pociski.back())->setPosition(boss.getPosition());
	}	
}

sf::Vector2f BossStrzelanie::zwrocKierunek(sf::Vector2f pociskPozycja, Paleta& paleta)
{
	float dlugosc = std::sqrt(pociskPozycja.x * pociskPozycja.x + pociskPozycja.y * pociskPozycja.y);

	sf::Vector2f dystans = paleta.getPosition() - pociskPozycja;

	dystans.x /= dlugosc;
	dystans.y /= dlugosc;

	return dystans;
}

void BossStrzelanie::resetuj()
{
	this->czasAnimacja = 0.f;
	this->czasStrzal = 0.f;
}