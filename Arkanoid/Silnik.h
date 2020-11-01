#pragma once

#include "NarzedzieDoStanow.h"
#include "Zasoby.h"
#include "GornyInterface.h"
#include "json.hpp"
#include <random>
#include <fstream>
#include <memory>

struct ElementySilnika
{
    ElementySilnika()
        : skala(3.f), wydarzenie()
    {
        std::random_device rd;
        this->generator.seed(rd());
    }

    NarzedzieDoStanow narzedzie;
    Zasoby zasoby;
    
    sf::RenderWindow okno;
    sf::Event wydarzenie;
    nlohmann::json daneJson;
    std::mt19937 generator;
    float skala;    

    GornyInterface gornyInterface;
};

typedef std::shared_ptr<ElementySilnika> ElementySilnikaWskaznik;

class Silnik
{
public:
    Silnik(int szerokosc, int wysokosc, std::string tytul);

private:
    ElementySilnikaWskaznik element = std::make_shared<ElementySilnika>();
    sf::Clock zegarDelta;

    void dzialanie();
};



