#pragma once

#include <vector>
#include "Klocek.h"
#include "Silnik.h"
#include "Pilka.h"
#include "Paleta.h"

class Poziom
{
public:

    //Konstruktor
    Poziom(ElementySilnikaWskaznik elementy);

    //Zmienne
    unsigned int numer;     //Poziomy numerowane sa od 0 (wiec runda pierwsza to numer 0)
    unsigned int liczbaZwyklychPoziomow;
    bool start;
    std::vector <sf::IntRect> tla;
    std::vector <sf::IntRect> poziomy;

    //Funkcje
    void stworzPoziom(std::vector<std::unique_ptr<Klocek>>& klocki, std::unique_ptr<sf::Sprite>& tlo, std::vector<std::unique_ptr<sf::Sprite>>& hpGrafika, int ktoryPoziom = 0, bool wczytaj = false);
    void stworzPoziom(std::unique_ptr<sf::Sprite>& tlo, std::unique_ptr<sf::Sprite>& ramka, std::vector<std::unique_ptr<sf::Sprite>>& hpGrafika);
    void zacznijPoziom(std::unique_ptr<Paleta>& gracz, std::unique_ptr<Pilka>& pilki, const float& dt);
    void resetujStart();
    void rysuj();

private:

    //Zmienne
    ElementySilnikaWskaznik element;
    enum PusteMiejsce { _ = 100 };
    std::vector<sf::IntRect> hpGrafiki;
    sf::Text Runda;
    sf::Text Ready;
    float czasAnimacja;
    int i;
    bool rysujRunda;
    bool rysujReady;

    //Dzwieki rozpoczynajace poziom
    sf::Sound dzwiekStart;
    sf::Sound dzwiekStartBoss;

    //Funkcje
    void ustawCzcionki();
    void wytnijTla();
    void ustawTlo(std::unique_ptr<sf::Sprite>& tlo, std::vector<std::unique_ptr<sf::Sprite>>& hpGrafika);
    void ustawUklad(std::vector<std::unique_ptr<Klocek>>& klocki, bool wczytaj = false);
    std::vector<int> wybierzUklad();

};

