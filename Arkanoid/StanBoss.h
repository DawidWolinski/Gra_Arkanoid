#pragma once

#include "Silnik.h"
#include "MenuRozgrywka.h"
#include "Paleta.h"
#include "Pilka.h"
#include "Poziom.h"
#include "Boss.h"
#include "TossCoin.h"

class StanBoss : public Stan
{
public:

	//Konstruktor
	StanBoss(ElementySilnikaWskaznik elementy, int hp);

	//Funkcje
	void inicjalizuj() override;
	void przechwytujWydarzenia(const float& dt) override;
	void aktualizuj(const float& dt) override;
	void rysuj(const float& dt) override;

private:

	//Zmienne
	ElementySilnikaWskaznik element;
	Kierunek Zaden;

	int punkty;
	int hp;
	float czasOdczekanie;

	//Funkcje
	void zaladujDzwieki();
	void sprawdzKolizje();
	void sprawdzajStanGry(const float& dt);
	void uzycieSpacji();
	void resetuj();

	//Dzwieki
	sf::Sound dzwiekSmierc;
	sf::Sound dzwiekOdbiciePaleta;
	sf::Sound dzwiekOdbicieBoss;
	//sf::Music dzwiekOutro;

	//Obiekty
	std::unique_ptr<Poziom> poziom;
	std::unique_ptr<Paleta> gracz;
	std::unique_ptr<sf::Sprite> tlo;
	std::unique_ptr<sf::Sprite> ramka;
	std::unique_ptr<Boss> boss;
	std::vector <std::unique_ptr<Pilka>> pilki;
	std::vector<std::unique_ptr<sf::Sprite>> hpGrafika;
	std::unique_ptr<MenuRozgrywka> menu;
};
