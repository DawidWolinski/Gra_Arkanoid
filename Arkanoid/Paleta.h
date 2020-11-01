#pragma once
#include "Silnik.h"
#include "Naglowki.h"
#include "PaletaPojawienie.h"
#include "PaletaTransformacja.h"
#include "PaletaSmierc.h"
#include "PaletaStandardowa.h"
#include "Animacja.h"

class Paleta : public sf::Sprite
{
public:

    //Konstruktor
    Paleta(ElementySilnikaWskaznik elementy);

    //Zmienne
    bool czyRysowac;
    bool czyWRuchu;
    bool ruchWPrawo;
    bool czyPrzechodzi;
    bool smierc;
    int rodzajPrzejscia;
    const sf::Vector2f szybkosc;
      
    //Funkcje
    void aktualizuj(const float& dt);
    void aktualizujCien();
    void rysuj();
    int miejsceKolizji(const sf::Sprite& obiekt);
    void zlapPilke(sf::Sprite& pilka);

    //Animacje
    Animacja* stan;
    PaletaPojawienie pojawienie;
    PaletaStandardowa standardowa;
    PaletaTransformacja transformacja;
    PaletaSmierc smiercAnimacja;

private:

    //Obiekty
    sf::Sprite cien;

    //Zmienne
    ElementySilnikaWskaznik element;
    float czasAnimacja;

    //Funkcje
    void animacja(const float& dt);
    void ustawTeksture();
};

