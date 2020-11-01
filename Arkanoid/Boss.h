#pragma once
#include "Silnik.h"
#include "Naglowki.h"
#include "Paleta.h"
#include "BossAnimacja.h"
#include "BossSpoczynek.h"
#include "BossSmierc.h"
#include "BossStrzelanie.h"
#include "BossOtwarcie.h"
#include "BossZamkniecie.h"

class Boss : public sf::Sprite
{
public:

	Boss(ElementySilnikaWskaznik elementy, sf::Vector2f pozycjaRamki);
	Kierunek ktoraSciana(const sf::Sprite& obiekt, const sf::Vector2f& szybkosc);
	
	void aktualizuj(const float& dt, std::unique_ptr<Paleta>& gracz);
	void uderzenie();
	void rysuj();

	int hp;
	bool czyRysowac;
	bool czyOutro;
	sf::RectangleShape hitBox;
	sf::Sound dzwiekSmiercBoss;


	BossAnimacja* stan;

	BossOtwarcie otwarcie;
	BossStrzelanie strzelanie;
	BossZamkniecie zamkniecie;
	BossSpoczynek spoczynek;
	BossSmierc smierc;

	sf::Sprite pocisk;
	std::vector<std::tuple<std::unique_ptr<sf::Sprite>, std::unique_ptr<sf::Vector2f>, unsigned int>> pociski{};
	std::vector<sf::IntRect> animacjePociskow;
	

private:
	void animacja(const float& dt, std::unique_ptr<Paleta>& gracz);
	void animacjaUderzenie(const float& dt);
	void animacjaUmieranie(const float& dt);
	void animacjaPociski(const float& dt);

	ElementySilnikaWskaznik element;
	Kierunek Prawa, Lewa, Dol, Gora, Zaden;

	float czasUderzenie;
	float czasUmieranie;
	float czasPocisk;
	int top;
	int mnoznik;
	

	sf::RectangleShape tlo;
};

