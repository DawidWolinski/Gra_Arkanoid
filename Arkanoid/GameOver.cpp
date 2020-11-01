#include "GameOver.h"

GameOver::GameOver(ElementySilnikaWskaznik elementy)
	: czasPrzejscie(0.f), element(elementy)
{

}

void GameOver::inicjalizuj()
{
	//Ustawienie napisu "GAME OVER"
    this->gameOver.setFont(this->element->zasoby.zwrocCzcionke("czcionka"));
    this->gameOver.setCharacterSize(8 * static_cast<int>(this->element->skala));
    this->gameOver.setString("game over");
    this->gameOver.setScale(sf::Vector2f(1 / this->element->skala, 1 / this->element->skala));
    this->gameOver.setOrigin(sf::Vector2f(std::round(this->gameOver.getLocalBounds().width / 2.f), std::round(this->gameOver.getLocalBounds().height / 2.f)));
    this->gameOver.setFillColor(sf::Color::White);
    this->gameOver.setPosition(sf::Vector2f(SZEROKOSC / 2.f, WYSOKOSC - 80.f));

	//Dzwieki
	this->element->zasoby.zaladujDzwiek("dzwiek_koniec_gry", PLIK_DZWIEK_KONIEC_GRY);
	this->dzwiekGameOver.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_koniec_gry"));
	this->dzwiekGameOver.setVolume(25.f);

	this->dzwiekGameOver.play();
}

void GameOver::przechwytujWydarzenia(const float& dt)
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
			if (this->element->wydarzenie.key.code == sf::Keyboard::Escape)
				this->element->okno.close();
			if (this->element->wydarzenie.key.code == sf::Keyboard::P || this->element->wydarzenie.key.code == sf::Keyboard::Enter || this->element->wydarzenie.key.code == sf::Keyboard::Space)
				this->czasPrzejscie += 5.f;
			break;
		}
	}
}

void GameOver::aktualizuj(const float& dt)
{
	this->element->gornyInterface.aktualizuj(dt);

	this->czasPrzejscie += dt;
	
	if (this->czasPrzejscie >= 5.f)
	{
		this->element->gornyInterface.resetuj();
		this->element->narzedzie.dodajStan(StanWskaznik(new StanMenu(this->element)));
	}
}

void GameOver::rysuj(const float& dt)
{
    this->element->okno.clear();

    this->element->okno.draw(this->gameOver);
	this->element->gornyInterface.rysuj();

    this->element->okno.display();
}