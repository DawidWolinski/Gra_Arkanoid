#pragma once
#include "Naglowki.h"
#include "Silnik.h"
#include <math.h>

class Pilka : public sf::Sprite
{
public:

    //Konstruktor
    Pilka(ElementySilnikaWskaznik elementy);

    //Zmienne
    bool czyWRuchu;
    bool czyRysowac;
    float odlegloscOdPalety;
    float stopnie;
    float stalaSzybkosc;
    sf::Vector2f szybkosc;
    bool odbita;

    //Funkcje
    void aktualizuj(const float& dt, const sf::Vector2f& rozmiarOkna);
    void rysuj();
    void rysujCien();
    void zmienKat(const float& stopnie);
    void odbij(Kierunek kierunekOdbicia, const float& stopnie = 0);
    void odbijPaleta(const int& miejsceUderzenia);
    void ustawNaPalecie(const sf::Sprite& paleta);
    void zwiekszSzybkosc();
    void zmniejszSzybkosc(float ile = 50.f);



private:
    //Obiekty
    sf::Sprite cien;

    //Zmienne
    ElementySilnikaWskaznik element;
    Kierunek Prawa, Lewa, Dol, Gora;
    const float pi;
    std::uniform_int_distribution<int> losuj;
   
    //Funkcje
    void ustawTeksture();
    void ruch(const float& dt, const sf::Vector2f& rozmiarOkna);
    
};

