#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Zasoby
{
public:
	//AssetManager();
	//~AssetManager();

	void zaladujTeksture(std::string nazwa, std::string nazwaPliku);
	sf::Texture& zwrocTeksture(std::string nazwa);

	void zaladujCzcionke(std::string nazwa, std::string nazwaPliku);
	sf::Font& zwrocCzcionke(std::string nazwa);

	void zaladujDzwiek(std::string nazwa, std::string nazwaPliku);
	sf::SoundBuffer& zwrocDzwiek(std::string nazwa);

	void zaladujUtwor(std::string nazwa, std::string nazwaPliku);
	std::unique_ptr<sf::Music>& zwrocUtwor(std::string nazwa);

private:
	std::map<std::string, sf::Texture> tekstury;
	std::map<std::string, sf::Font> czcionki;
	std::map<std::string, sf::SoundBuffer> dzwieki;
	std::map<std::string, std::unique_ptr<sf::Music>> utwory;
};
