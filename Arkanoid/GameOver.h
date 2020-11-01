#pragma once
#include "Silnik.h"
#include "Naglowki.h"
#include "StanMenu.h"

class GameOver : public Stan
{
public:

	//Konstruktor
	GameOver(ElementySilnikaWskaznik elementy);

	//Funkcje
	virtual void inicjalizuj() override;
	virtual void przechwytujWydarzenia(const float& dt) override;
	virtual void aktualizuj(const float& dt) override;
	virtual void rysuj(const float& dt) override;

private:

	ElementySilnikaWskaznik element;

	//Zmienne
	sf::Sound dzwiekGameOver;
	sf::Text gameOver;
	float czasPrzejscie;
};

