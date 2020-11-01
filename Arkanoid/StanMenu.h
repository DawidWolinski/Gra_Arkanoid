#pragma once

#include "Silnik.h"
#include "Naglowki.h"
#include "TablicaWynikow.h"
#include "StanIntro.h"

class StanMenu : public Stan
{
public:

	//Konstruktor
	StanMenu(ElementySilnikaWskaznik elementy, bool start = false);

	//Funkcje
	void inicjalizuj() override;
	void przechwytujWydarzenia(const float& dt) override;
	void aktualizuj(const float& dt) override;
	void rysuj(const float& dt) override;

private:
	
	ElementySilnikaWskaznik element;
	sf::View widok;

	//Zmienne
	Kierunek wDol, wGore;
	bool zapisGry;
	bool rysujTaito;
	bool rysujOpcje;
	unsigned int ktoryNapis;
	unsigned int i;
	float czasAnimacja;
	float czasSekwencja;

	//Napisy
	std::vector <std::unique_ptr<sf::Text>> napisy;
	std::vector <std::unique_ptr<sf::Text>> taito;
	sf::Text allRights;
	sf::Sprite logoGry;
	sf::Sprite znakCopyright;

	//Funkcje
	void ustawCzcionki();
	void zaladujDzwieki();
	void zaladujTekstury();
	void sekwencja(float dt);
	void ustawNapis(sf::Text& tekst, std::string napis, float x, float y, unsigned int rozmiar = 8, sf::Color kolor = sf::Color::White, std::string czcionka = "czcionka");
	void animujNapis(const float& dt);
	void zmianaNapisu(Kierunek kierunek);
	void wyborOpcji();
	void aktualizujCzcionki();	
};

