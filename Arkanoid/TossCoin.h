#pragma once
#include "Silnik.h"
#include "Naglowki.h"
#include "GameOver.h"

class TossCoin : public Stan
{
public:

	//Konstruktor
	TossCoin(int ktoryPoziom, ElementySilnikaWskaznik elementy);

	//Funckje
	virtual void inicjalizuj() override;
	virtual void przechwytujWydarzenia(const float& dt) override;
	virtual void aktualizuj(const float& dt) override;
	virtual void rysuj(const float& dt) override;

private:

	ElementySilnikaWskaznik element;

	//Zmienne
	int ktoryPoziom;
	sf::Text toContinue;
	std::vector<std::unique_ptr<sf::Text>> instrukcja;
	std::vector<std::unique_ptr<sf::Text>> beforeTimer;
	sf::Text napisTime;
	sf::Text credit;
	sf::Clock zegar;
	float czas;

	//Funkcje
	void ustawNapisy();
	void ustawNapis(sf::Text& tekst, std::string napis, float x, float y, bool wysrodkuj = false, sf::Color kolor = sf::Color::White);
	void aktualizujTimer();
};