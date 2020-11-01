#pragma once
#include "Stan.h"
#include "Silnik.h"
#include "Naglowki.h"
#include "Poziom.h"
#include "Klocek.h"
#include <memory>
#include "StanRozgrywki.h"

class StanPoziomy : public Stan
{
public:

	//Konstruktor
	StanPoziomy(ElementySilnikaWskaznik elementy);

	//Funkcje
	void inicjalizuj() override;
	void przechwytujWydarzenia(const float& dt) override;
	void aktualizuj(const float& dt) override;
	void rysuj(const float& dt) override;
	void ustawCzcionki();

private:
	ElementySilnikaWskaznik element;
	sf::View widok;


	unsigned int ktoryNapis;
	unsigned int i;

	float czasAnimacja;

	//Napisy
	std::vector <std::unique_ptr<sf::Text>> napisy;


	//Funkcje
	void animujNapis(const float& dt);
	void poziomy();

	std::unique_ptr<Poziom> poziom;
	std::unique_ptr<sf::Sprite> tlo;
	std::vector <std::unique_ptr<Klocek>> klocki;
	std::vector<std::unique_ptr<sf::Sprite>> hpGrafika;
	std::vector<std::unique_ptr<sf::Sprite>> zdjeciaPoziomow;
	std::vector<std::unique_ptr<sf::Texture>> teksturyZdjec;


};

