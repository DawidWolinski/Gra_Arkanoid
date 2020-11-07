#pragma once

#include "Silnik.h"
#include "Naglowki.h"
#include "Paleta.h"
#include "Pilka.h"


class Laser;

class Bonus : public sf::Sprite
{
public:

	//Konstruktor
	Bonus(ElementySilnikaWskaznik elementy);

	//Zmienne
	bool czyRysowac;
	bool czyWRuchu;
	bool czyPrzejscie;
	int rodzaj;
	int aktywnyRodzaj;
	float zielonyCzas;
	const float zielonyLimit;
	std::vector<std::unique_ptr<Laser>> lasery;

	//Funkcje
	void aktualizuj(const float& dt);
	void rysuj();
	void zrzuc(const sf::Vector2f& pozycja);
	void aktywujBonus(std::unique_ptr<Paleta>& gracz, std::unique_ptr<Pilka>& pilka);
	void strzelLaserem(const sf::Vector2f& pozycjaPalety);

private:

	//Zmienne
	ElementySilnikaWskaznik element;
	float czasAnimacja;	
	float czasAnimacjaPrzejscie;	
	float odlegloscLaserow;
	sf::Sound dzwiekWydluzenie;
	sf::Sound dzwiekLaser;
	sf::Sprite cien;
	sf::Sprite przejscie;
	const sf::Vector2f szybkosc;
	const sf::Vector2f szybkoscLasera;
	std::uniform_int_distribution<int> losuj;
	std::uniform_int_distribution<int> losujPrzejscie;

	//Funkcje
	void ustawTeksture();
	void animacja(const float& dt);
	void aktualizujLasery(const float& dt);
	void animacjaPrzejscie(const float& dt);
};


class Laser : public sf::Sprite
{
public:
	friend class Bonus;

	//Konstruktor
	Laser(ElementySilnikaWskaznik elementy, sf::Vector2f pozycjaPalety);

	//Zmienne
	bool czyKolizja;

private:

	//Zmienne
	ElementySilnikaWskaznik element;
	bool usun;
	float czasAnimacja;
	sf::Vector2f szybkosc;

	//Funkcje
	void aktualizuj(const float& dt);
};
