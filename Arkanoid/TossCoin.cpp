#include "TossCoin.h"

TossCoin::TossCoin(int ktoryPoziom, ElementySilnikaWskaznik elementy)
	: ktoryPoziom(ktoryPoziom), element(elementy), czas(0)
{
}

void TossCoin::inicjalizuj()
{
	this->ustawNapisy();
}

void TossCoin::ustawNapisy()
{
	for (unsigned int i = 0; i < 3; i++)
		this->instrukcja.push_back(std::make_unique<sf::Text>());

	for (unsigned int i = 0; i < 2; i++)
		this->beforeTimer.push_back(std::make_unique<sf::Text>());

	this->ustawNapis(this->toContinue, "to continue game", SZEROKOSC / 2.f, 50.f, true, sf::Color::Red);

	this->ustawNapis(*this->instrukcja[0], "insert additional coin(s),", SZEROKOSC / 2.f, this->toContinue.getPosition().y + this->toContinue.getGlobalBounds().height/2.f + 30.f, true);
	this->ustawNapis(*this->instrukcja[1], "hold fire button and push", this->instrukcja[0]->getGlobalBounds().left, this->instrukcja[0]->getPosition().y + this->instrukcja[0]->getGlobalBounds().height/2.f + 5.f);
	this->ustawNapis(*this->instrukcja[2], "1 or 2 player button", this->instrukcja[0]->getGlobalBounds().left, this->instrukcja[1]->getPosition().y + this->instrukcja[1]->getGlobalBounds().height + 5.f);

	this->ustawNapis(*this->beforeTimer[0], "before", this->instrukcja[0]->getGlobalBounds().left + 15.f, this->instrukcja[2]->getPosition().y + this->instrukcja[2]->getGlobalBounds().height + 25.f);
	this->ustawNapis(*this->beforeTimer[1], "timer reaches \"0\".", this->beforeTimer[0]->getGlobalBounds().left - 1.f, this->beforeTimer[0]->getPosition().y + this->beforeTimer[0]->getGlobalBounds().height + 5.f);

	
	this->ustawNapis(this->napisTime, "time 9", this->beforeTimer[0]->getGlobalBounds().left + 35.f, this->beforeTimer[1]->getPosition().y + this->beforeTimer[1]->getGlobalBounds().height + 15.f, false, sf::Color::Yellow);
	this->ustawNapis(this->credit, "credit 0", SZEROKOSC - 70.f, WYSOKOSC - 10.f);

}

void TossCoin::aktualizujTimer()
{
	//Odlicza czas do konca

	if (std::floor(this->zegar.getElapsedTime().asSeconds()) > this->czas)
	{
		this->czas += 1.f;

		if (this->czas == 9.f)
		{
			if (this->element->gornyInterface.nowyWynik())
				this->element->narzedzie.dodajStan(StanWskaznik(new TablicaWynikow(this->ktoryPoziom, this->element->gornyInterface.ilePunktow(), this->element)));
			else
				this->element->narzedzie.dodajStan(StanWskaznik(new GameOver(this->element)));
		}		
		else
			this->napisTime.setString("time " + std::to_string(static_cast<int>(9.f - this->czas)));
	}
}

void TossCoin::przechwytujWydarzenia(const float& dt)
{
	while (this->element->okno.pollEvent(this->element->wydarzenie))
	{
		switch (this->element->wydarzenie.type)
		{
		case sf::Event::Closed:
			this->element->okno.close();
			break;
		case sf::Event::Resized:
			this->element->okno.setPosition(sf::Vector2i(this->element->okno.getPosition().x - int(SZEROKOSC) / 2, this->element->okno.getPosition().y - int(WYSOKOSC) / 2));
			break;
		case sf::Event::KeyPressed:
			if (this->element->wydarzenie.key.code == sf::Keyboard::Enter || this->element->wydarzenie.key.code == sf::Keyboard::Space || this->element->wydarzenie.key.code == sf::Keyboard::Escape)
			{
				if (this->element->gornyInterface.nowyWynik())
					this->element->narzedzie.dodajStan(StanWskaznik(new TablicaWynikow(this->ktoryPoziom, this->element->gornyInterface.ilePunktow(), this->element)));
				else
					this->element->narzedzie.dodajStan(StanWskaznik(new GameOver(this->element)));
			}
			break;
		}
	}
}

void TossCoin::aktualizuj(const float& dt)
{
	this->element->gornyInterface.aktualizuj(dt);

	this->aktualizujTimer();
}

void TossCoin::rysuj(const float& dt)
{
	this->element->okno.clear();

	this->element->gornyInterface.rysuj();

	this->element->okno.draw(this->toContinue);

	for(auto& napis : this->instrukcja)
		this->element->okno.draw(*napis);

	for (auto& napis : this->beforeTimer)
		this->element->okno.draw(*napis);

	this->element->okno.draw(this->napisTime);

	this->element->okno.draw(this->credit);

	this->element->okno.display();
}

void TossCoin::ustawNapis(sf::Text& tekst, std::string napis, float x, float y, bool wysrodkuj, sf::Color kolor)
{
	tekst.setFont(this->element->zasoby.zwrocCzcionke("czcionka"));
	tekst.setCharacterSize(8 * static_cast<int>(this->element->skala));
	tekst.setString(napis);
	tekst.setScale(sf::Vector2f(1 / this->element->skala, 1 / this->element->skala));
	if(wysrodkuj)
		tekst.setOrigin(sf::Vector2f(std::round(tekst.getLocalBounds().width / 2.f), std::round(tekst.getLocalBounds().height / 2.f)));
	tekst.setFillColor(kolor);
	tekst.setPosition(sf::Vector2f(std::round(x), std::round(y)));
}