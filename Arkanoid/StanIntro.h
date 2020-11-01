#pragma once
#include "Silnik.h"
#include "Naglowki.h"


///////////////		WORK IN PROGRESS	////////////////////


class StanIntro : public Stan
{
public:

	//Konstruktor
	StanIntro(ElementySilnikaWskaznik elementy);

	//Funkcje
	void inicjalizuj() override;
	void przechwytujWydarzenia(const float& dt) override;
	void aktualizuj(const float& dt) override;
	void rysuj(const float& dt) override;

private:

	ElementySilnikaWskaznik element;
	int i;
	sf::Sprite malyStatek;
	sf::Sprite duzyStatek;
	sf::Sprite tlo;

	float zamiennik;

	float stalaSzybkosc;
	sf::Vector2f szybkosc;
	float promien;
	float czasAnimacja;
	float czasAnimacjaStatek;
	float stopnie;
	const double pi = atan(1) * 4;
	std::vector<sf::IntRect> animacje;
	int numerAnimacji;


	void ustawTekstury();
	float radiany(const float& stopnie);
	void sekwencja(const float& dt);
	void ruchStatku(const float& dt);
	void animacjaStatku(const float& dt);
};

