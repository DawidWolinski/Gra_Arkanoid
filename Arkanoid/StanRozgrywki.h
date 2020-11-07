#pragma once

#include "Silnik.h"
#include "Naglowki.h"
#include "Paleta.h"
#include "Pilka.h"
#include "Klocek.h"
#include "Poziom.h"
#include "Bonus.h"
#include "StanBoss.h"
#include "MenuRozgrywka.h"
#include "TossCoin.h"

using json = nlohmann::json;

class StanRozgrywki : public Stan
{
public:

	//Konstruktor
	StanRozgrywki(ElementySilnikaWskaznik elementy, bool wczytaj, const int& ktoryPoziom = 0);

	//Funkcje
	void inicjalizuj() override;
	void przechwytujWydarzenia(const float& dt) override;
	void aktualizuj(const float& dt) override;
	void rysuj(const float& dt) override;

private:

	ElementySilnikaWskaznik element;
	Kierunek Zaden;

	//Zmienne
	bool wczytaj;
	int hp;
	int przyznaneHP;
	float czasOdczekanie;
	int ktoryPoziom;
	sf::Time czasDzwiek;
	sf::Clock dzwiekTimer;
	std::uniform_int_distribution<int> losuj;

	//Funkcje
	void sprawdzKolizje();
	void zarzadzajBonusami(const float& dt);
	void sprawdzajStanGry(const float& dt);
	void uzycieSpacji();
	void resetuj();
	void zapiszGre();
	void wczytajZapis();
	void ustawDzwieki();
	void dodajPunkty(int punkty);
	void animacjaPrzejscia();
	void dzwiekOdbicie(std::pair<sf::Sound, sf::Clock>& dzwiek);

	//Dzwieki
	sf::Sound dzwiekSmierc;
	sf::Sound dzwiekZycie;
	std::pair<sf::Sound, sf::Clock> dzwiekOdbiciePaleta;
	std::pair<sf::Sound, sf::Clock> dzwiekOdbicieKlocek;
	std::pair<sf::Sound, sf::Clock> dzwiekOdbicieKlocekZloty;

	//Obiekty
	std::unique_ptr<Poziom> poziom;
	std::unique_ptr<Paleta> gracz;
	std::unique_ptr<sf::Sprite> tlo;
	std::vector <std::unique_ptr<Klocek>> klocki;
	std::vector <std::unique_ptr<Pilka>> pilki;
	std::unique_ptr<Bonus> bonus;
	std::vector<std::unique_ptr<sf::Sprite>> hpGrafika;
	std::unique_ptr<MenuRozgrywka> menu;
};