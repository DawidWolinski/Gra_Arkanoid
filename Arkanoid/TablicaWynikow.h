#pragma once
#include "Silnik.h"
#include "Naglowki.h"
#include "GameOver.h"

using json = nlohmann::json;

class TablicaWynikow : public Stan
{
public:

	//Konstruktor
	TablicaWynikow(int runda, int wynik, ElementySilnikaWskaznik elementy, bool czyWygrana = false);

	//Funkcje
	virtual void inicjalizuj() override;
	virtual void przechwytujWydarzenia(const float& dt) override;
	virtual void aktualizuj(const float& dt) override;
	virtual void rysuj(const float& dt) override;

private:

	ElementySilnikaWskaznik element;

	//Zmienne
	int miejsce;	
	int wynik;
	std::string napisRunda;
	int numerRunda;
	unsigned int limitWynikow;
	int ktoryInicjal;
	std::string literki[3];
	bool wpisany;
	bool czyWygrana;	
	bool nowyWynik;
	bool gameOver;
	float czasAnimacja;
	float czasPrzejscie;
	
	//Napisy
	sf::Text napisRunda1;
	sf::Text enterInitials;
	sf::Text napisImie;
	sf::Text napisWynik;
	sf::Text napisRunda2;
	sf::Text back;
	std::vector<std::unique_ptr<sf::Text>> brakNowegoWyniku;
	std::vector<std::unique_ptr<sf::Text>> naglowki1;
	std::vector<std::unique_ptr<sf::Text>> naglowki2;
	std::vector<std::unique_ptr<sf::Text>> inicjaly;
	std::vector<std::vector<sf::Text>> wyniki;

	//Funkcje
	void ustawNapisy();
	void ustawNapis(sf::Text& tekst, std::string napis, float x, float y, bool wysrodkuj = false, sf::Color kolor = sf::Color::White);
	void pobierzWyniki();
	void resetujWyniki();
	void wpiszWynik();
	void aktualizujInicjaly();
	void animujInicjal(const float& dt);
	void animujNapis(const float& dt);
	void przejscie(const float& dt);
	std::string zwrocKoncowke(int liczba);
};

