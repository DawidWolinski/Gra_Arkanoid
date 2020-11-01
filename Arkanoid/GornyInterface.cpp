#include "GornyInterface.h"

GornyInterface::GornyInterface()
    : punkty(0), czyRysowacUp(true), czasAnimacja(0.f), okno(nullptr), czcionka(nullptr), minimalnyProg(30000), najwiecejPunktow(50000)
{ 
}

void GornyInterface::inicjalizuj(sf::RenderWindow* okno, sf::Font* czcionka, int minimalnyProg, int najwiecejPunktow)
{
    this->okno = okno;
    this->czcionka = czcionka;

    this->minimalnyProg = minimalnyProg;
    this->najwiecejPunktow = najwiecejPunktow;

    this->ustawNapisy();
}

void GornyInterface::ustawNapisy()
{
    this->ustawNapis(this->highscoreNapis, "high score", SZEROKOSC / 2.f, 6.f, sf::Color::Red);
    this->ustawNapis(this->highscore, std::to_string(this->najwiecejPunktow), SZEROKOSC / 2.f, 14.f);
    this->ustawNapis(this->up, "1up", 37.f, this->highscoreNapis.getPosition().y, sf::Color::Red);
    this->ustawNapis(this->score, "00", 55.f, this->highscore.getPosition().y, sf::Color::White, false);
}

void GornyInterface::dodajPunkty(int punkty)
{
    this->punkty += punkty;
    this->score.setString(std::to_string(this->punkty));
    this->score.setOrigin(sf::Vector2f(this->score.getLocalBounds().width, this->score.getLocalBounds().height));

    if (this->punkty > this->najwiecejPunktow)
    {
        this->najwiecejPunktow = this->punkty;
        this->highscore.setString(std::to_string(this->najwiecejPunktow));
        this->highscore.setOrigin(sf::Vector2f(std::round(this->highscore.getLocalBounds().width / 2.f), this->highscore.getLocalBounds().height));
    }
}

bool GornyInterface::nowyWynik()
{
    return this->punkty > this->minimalnyProg;
}

int GornyInterface::ilePunktow()
{
    return this->punkty;
}

int GornyInterface::ileNajwiecejPunktow()
{
    return this->najwiecejPunktow;
}

void GornyInterface::resetuj()
{
    std::ifstream plik(PLIK_DANE);
    json tablica;
    plik >> tablica;

    this->najwiecejPunktow = tablica[0][0]["score"];
    this->minimalnyProg = tablica[0].back()["score"];

    this->highscore.setString(std::to_string(this->najwiecejPunktow));
    this->highscore.setOrigin(sf::Vector2f(std::round(this->highscore.getLocalBounds().width / 2.f), this->highscore.getLocalBounds().height));

    this->punkty = 0;
    this->score.setString("00");
    this->score.setOrigin(sf::Vector2f(this->score.getLocalBounds().width, this->score.getLocalBounds().height));
}

void GornyInterface::ustawNapis(sf::Text& tekst, std::string napis, float x, float y, sf::Color kolor, bool wysrodkuj)
{
    tekst.setFont(*this->czcionka);
    tekst.setCharacterSize(8*static_cast<int>(3));
    tekst.setString(napis);
    tekst.setScale(sf::Vector2f(1 / 3.f, 1 / 3.f));

    if(wysrodkuj)
        tekst.setOrigin(sf::Vector2f(std::round(tekst.getLocalBounds().width / 2.f), tekst.getLocalBounds().height));
    else
        tekst.setOrigin(sf::Vector2f(tekst.getLocalBounds().width, tekst.getLocalBounds().height));

    tekst.setFillColor(kolor);
    tekst.setPosition(sf::Vector2f(std::round(x), std::round(y)));
}

void GornyInterface::aktualizujCzcionki(float skala)
{
    this->highscoreNapis.setCharacterSize(8 * static_cast<int>(skala));
    this->highscoreNapis.setScale(sf::Vector2f(1 / skala, 1 / skala));
    this->highscoreNapis.setOrigin(sf::Vector2f(std::round(this->highscoreNapis.getLocalBounds().width / 2.f), this->highscoreNapis.getLocalBounds().height));

    this->highscore.setCharacterSize(8 * static_cast<int>(skala));
    this->highscore.setScale(sf::Vector2f(1 / skala, 1 / skala));
    this->highscore.setOrigin(sf::Vector2f(std::round(this->highscore.getLocalBounds().width / 2.f), this->highscore.getLocalBounds().height));

    this->up.setCharacterSize(8 * static_cast<int>(skala));
    this->up.setScale(sf::Vector2f(1 / skala, 1 / skala));
    this->up.setOrigin(sf::Vector2f(std::round(this->up.getLocalBounds().width / 2.f), this->up.getLocalBounds().height));

    this->score.setCharacterSize(8 * static_cast<int>(skala));
    this->score.setScale(sf::Vector2f(1 / skala, 1 / skala));
    this->score.setOrigin(sf::Vector2f(this->score.getLocalBounds().width, this->score.getLocalBounds().height));
}

void GornyInterface::aktualizuj(const float& dt)
{
    this->czasAnimacja += dt;

    if (this->czasAnimacja >= 0.5f)
    {
        this->czasAnimacja = 0.f;
        this->czyRysowacUp = !this->czyRysowacUp;
    }
}

void GornyInterface::rysuj()
{

    this->okno->draw(this->highscoreNapis);

    this->okno->draw(this->highscore);

    if (this->czyRysowacUp)
        this->okno->draw(this->up);

    this->okno->draw(this->score);
}