#include "MenuRozgrywka.h"


MenuRozgrywka::MenuRozgrywka(ElementySilnikaWskaznik elementy, bool boss)
	: boss(boss), aktywne(false), czasAnimacja(0.f), ktoryNapis(0), wGore(false), element(elementy)
{
	this->ustawNapisy();
	this->ustawZaslone();
}

void MenuRozgrywka::ustawNapisy()
{
	if (!this->boss)
	{
		for (int i = 0; i < 3; i++)
			this->napisy.push_back(std::make_unique<sf::Text>());

		this->ustawNapis(*this->napisy[0], "resume", SZEROKOSC / 2.f, 100.f, 10, sf::Color::White);
		this->ustawNapis(*this->napisy[1], "save and exit", SZEROKOSC / 2.f, this->napisy[0]->getPosition().y + this->napisy[0]->getGlobalBounds().height + 20.f, 10, sf::Color::White);
		this->ustawNapis(*this->napisy[2], "exit", SZEROKOSC / 2.f, this->napisy[1]->getPosition().y + this->napisy[1]->getGlobalBounds().height + 20.f, 10, sf::Color::White);
	}
	else
	{
		for (int i = 0; i < 2; i++)
			this->napisy.push_back(std::make_unique<sf::Text>());

		this->ustawNapis(*this->napisy[0], "resume", SZEROKOSC / 2.f, 115.f, 10, sf::Color::White);
		this->ustawNapis(*this->napisy[1], "exit", SZEROKOSC / 2.f, this->napisy[0]->getPosition().y + this->napisy[0]->getGlobalBounds().height + 20.f, 10, sf::Color::White);
	}


	for (int i = 0; i < 2; i++)
		this->komunikat.push_back(std::make_unique<sf::Text>());

	this->ustawNapis(*this->komunikat[0], "progress can only be saved when", SZEROKOSC / 2.f, 220.f, 4, sf::Color::White);
	this->ustawNapis(*this->komunikat[1], "the ball flies upwards", SZEROKOSC / 2.f, this->komunikat[0]->getPosition().y + this->napisy[0]->getGlobalBounds().height / 2.f + 2.f, 4, sf::Color::White);
}

void MenuRozgrywka::ustawZaslone()
{
	this->zaslona.setSize(sf::Vector2f(SZEROKOSC, WYSOKOSC));
	this->zaslona.setPosition(sf::Vector2f(0.f, 0.f));
	this->zaslona.setFillColor(sf::Color(0, 0, 0, 125));
}


void MenuRozgrywka::animujNapis(const float& dt)
{
	this->czasAnimacja += dt;

	if (this->czasAnimacja >= 0.5f)
	{
		this->czasAnimacja = 0.f;

		if (this->napisy[this->ktoryNapis]->getFillColor().Yellow == this->napisy[this->ktoryNapis]->getFillColor()) 
			this->napisy[this->ktoryNapis]->setFillColor(sf::Color::White);
		else 
			this->napisy[this->ktoryNapis]->setFillColor(sf::Color::Yellow);
	}
}

void MenuRozgrywka::zmianaNapisu(int kierunek)
{
	if (kierunek == 0)
	{
		if (this->ktoryNapis == this->napisy.size() - 1) 
			this->ktoryNapis = 0;
		else 
			this->ktoryNapis++;

		if (this->ktoryNapis == 1 && !this->wGore && !this->boss)
			this->ktoryNapis++;
	}

	else
	{
		if (this->ktoryNapis == 0) 
			this->ktoryNapis = this->napisy.size() - 1;
		else 
			this->ktoryNapis--;

		if (this->ktoryNapis == 1 && !this->wGore && !this->boss)
			this->ktoryNapis--;
	}

	for (unsigned int i = 0; i < this->napisy.size(); i++)
	{
		if (i == 1 && this->wGore || i != this->ktoryNapis && i != 1 || this->boss && i != this->ktoryNapis)
			this->napisy[i]->setFillColor(sf::Color::White);
	}

	this->napisy[this->ktoryNapis]->setFillColor(sf::Color::Yellow);
	this->czasAnimacja = 0.f;
}

void MenuRozgrywka::aktualizuj(const float& dt)
{
	this->animujNapis(dt);
}

void MenuRozgrywka::rysuj()
{
	if (this->aktywne)
	{
		this->element->okno.draw(this->zaslona);

		for (auto& napis : this->napisy)
			this->element->okno.draw(*napis);

		if(!this->wGore && !this->boss)
			for (auto& komunikatNapis : this->komunikat)
				this->element->okno.draw(*komunikatNapis);
	}	
}

void MenuRozgrywka::wyszarzNapis()
{
	if (this->wGore)
		this->napisy[1]->setFillColor(sf::Color::White);
	else
		this->napisy[1]->setFillColor(sf::Color(75, 72, 72));		
}

void MenuRozgrywka::ustawNapis(sf::Text& tekst, std::string napis, float x, float y, unsigned int size, sf::Color kolor)
{
	tekst.setFont(this->element->zasoby.zwrocCzcionke("czcionka"));
	tekst.setCharacterSize(size * static_cast<int>(this->element->skala));
	tekst.setString(napis);
	tekst.setScale(sf::Vector2f(1 / this->element->skala, 1 / this->element->skala));
	tekst.setOrigin(sf::Vector2f(std::round(tekst.getLocalBounds().width / 2.f), std::round(tekst.getLocalBounds().height / 2.f)));
	tekst.setFillColor(kolor);
	tekst.setPosition(sf::Vector2f(std::round(x), std::round(y)));
}