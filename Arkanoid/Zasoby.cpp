#include "Zasoby.h"

void Zasoby::zaladujTeksture(std::string nazwa, std::string nazwaPliku)
{
	if (this->tekstury.find(nazwa) == this->tekstury.end())
	{
		sf::Texture tekstura;

		if (tekstura.loadFromFile(nazwaPliku))
		{
			this->tekstury[nazwa] = tekstura;
		}
	}
}

sf::Texture& Zasoby::zwrocTeksture(std::string nazwa)
{
	return this->tekstury.at(nazwa);
}

void Zasoby::zaladujCzcionke(std::string nazwa, std::string nazwaPliku)
{
	if (this->czcionki.find(nazwa) == this->czcionki.end())
	{
		sf::Font czcionka;

		if (czcionka.loadFromFile(nazwaPliku))
		{
			this->czcionki[nazwa] = czcionka;
		}
	}
}

sf::Font& Zasoby::zwrocCzcionke(std::string nazwa)
{
	return this->czcionki.at(nazwa);
}

void Zasoby::zaladujDzwiek(std::string nazwa, std::string nazwaPliku)
{
	if (this->dzwieki.find(nazwa) == this->dzwieki.end())
	{
		sf::SoundBuffer dzwiek;

		if (dzwiek.loadFromFile(nazwaPliku))
		{
			this->dzwieki[nazwa] = dzwiek;
		}
	}
}

sf::SoundBuffer& Zasoby::zwrocDzwiek(std::string nazwa)
{
	return this->dzwieki.at(nazwa);
}

void Zasoby::zaladujUtwor(std::string nazwa, std::string nazwaPliku)
{
	if (this->utwory.find(nazwa) == this->utwory.end())
	{
		auto utwor = std::make_unique<sf::Music>();

		if (utwor->openFromFile(nazwaPliku))
		{
			this->utwory.emplace(nazwa, std::move(utwor));
		}
	}
}

std::unique_ptr<sf::Music>& Zasoby::zwrocUtwor(std::string nazwa)
{
	return this->utwory.at(nazwa);
}