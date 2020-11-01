#pragma once
#include "Silnik.h"
#include "Naglowki.h"

class MenuRozgrywka
{
public:

	//Konstruktor
	MenuRozgrywka(ElementySilnikaWskaznik elementy, bool boss = false);
	
	//Funkcje
	void aktualizuj(const float& dt);
	void rysuj();
	void wyszarzNapis();
	void zmianaNapisu(int kierunek);
	int ktoryNapis;

	//Zmienne
	bool wGore;
	bool aktywne;

private:

	ElementySilnikaWskaznik element;

	//Funkcje
	void animujNapis(const float& dt);	
	void ustawZaslone();
	void ustawNapisy();
	void ustawNapis(sf::Text& tekst, std::string napis, float x, float y, unsigned int size, sf::Color kolor);

	//Zmienne
	bool boss;
	float czasAnimacja;
	
	sf::RectangleShape zaslona;

	//Napisy
	std::vector <std::unique_ptr<sf::Text>> napisy;
	std::vector <std::unique_ptr<sf::Text>> komunikat;	
};
