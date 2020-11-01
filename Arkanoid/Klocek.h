#pragma once

#include "Naglowki.h"
#include "Silnik.h"
#include <array>


class Klocek : public sf::Sprite
{
public:

    //Konstruktor i destruktor
    Klocek(int rodzaj, float x, float y, ElementySilnikaWskaznik elementy, int ktoryPoziom = 1, int hp = 0);

    //Zmienne 
    int hp;
    int rodzaj;
    int ilePunktow;
    static int ileZlotych;
    static std::vector<sf::Vector2f> usunieteKlocki;

    //Funkcje
    void rysuj();
    void aktualizuj(const float& dt);
    void animacja(const float& dt);
    void sprawdzKlockiObok(const std::vector<std::unique_ptr<Klocek>>& klocki);
    void sprawdzKlockiObok();
    void uderzenie();
    Kierunek ktoraSciana(const sf::Vector2f& pozycjaPilki, const sf::Vector2f& szybkosc);

private:
    //Obiekty
    sf::Sprite cien;

    //Zmienne
    ElementySilnikaWskaznik element;
    Kierunek Prawa, Lewa, Dol, Gora, Zaden;
    std::array<bool, 4> klocekObok;
    float czasAnimacja;
    bool animuj;
   
    //Funkcje
    void ustawTeksture(int ktoryPoziom);
    void ustawCien();
};

