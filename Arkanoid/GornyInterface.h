#pragma once
#include <SFML/Graphics.hpp>
#include "Naglowki.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

class GornyInterface
{
public:

	//Konstruktor
	GornyInterface();
	
	//Zmienne
	bool nowyWynik();
	int ilePunktow();
	int ileNajwiecejPunktow();

	//Funkcje
	void aktualizuj(const float& dt);
	void rysuj();
	void inicjalizuj(sf::RenderWindow* okno, sf::Font* czcionka, int minimalnyProg, int najwiecejPunktow);
	void dodajPunkty(int punkty);
	void resetuj();
	void aktualizujCzcionki(float skala);

private:

	//Zmienne
	int najwiecejPunktow;
	int punkty;
	int minimalnyProg;
	float czasAnimacja;
	bool czyRysowacUp;

	sf::RenderWindow* okno;
	sf::Font* czcionka;

	//Napisy
	sf::Text highscoreNapis;
	sf::Text up;
	sf::Text highscore;
	sf::Text score;

	//Funkcje
	void ustawNapisy();
	void ustawNapis(sf::Text& tekst, std::string napis, float x, float y, sf::Color kolor = sf::Color::White, bool wysrodkuj = true);
};

