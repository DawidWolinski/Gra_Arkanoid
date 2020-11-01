#include "Silnik.h"
#include "Naglowki.h"
#include "StanMenu.h"


Silnik::Silnik(int szerokosc, int wysokosc, std::string tytul)
{
	 
	this->element->okno.create(sf::VideoMode(szerokosc, wysokosc), tytul, sf::Style::Close | sf::Style::Titlebar);
	this->element->okno.setFramerateLimit(LIMIT_FPS);
	this->element->okno.setPosition(sf::Vector2i(static_cast<int>(sf::VideoMode::getDesktopMode().width/2) - static_cast<int>(this->element->okno.getSize().x/2), static_cast<int>(sf::VideoMode::getDesktopMode().height/2) - static_cast<int>(this->element->okno.getSize().y/2) - 30));

	std::ifstream plik(PLIK_DANE);
	plik >> this->element->daneJson;

	this->element->zasoby.zaladujCzcionke("czcionka", PLIK_CZCIONKA);
	this->element->gornyInterface.inicjalizuj(&this->element->okno, &this->element->zasoby.zwrocCzcionke("czcionka"), this->element->daneJson[0].back()["score"], this->element->daneJson[0][0]["score"]);
	this->element->narzedzie.dodajStan(StanWskaznik(new StanMenu(this->element, true)));

	this->dzialanie();

	//Przy zamknieciu usuwa ostatni stan
	this->element->narzedzie.usunWszystkieStany();
}

void Silnik::dzialanie()
{
	while (this->element->okno.isOpen())
	{
		float dt = zegarDelta.restart().asSeconds();

		//Na wypadek gdy gra sie zatnie to uniemozliwi to "teleportacje" pilki i palety
		if(dt > 0.0175f)
			dt = 0.0175f;

		this->element->narzedzie.zarzadzajZmianaStanu();

		this->element->narzedzie.zwrocObecnyStan()->przechwytujWydarzenia(dt);
		this->element->narzedzie.zwrocObecnyStan()->aktualizuj(dt);
		this->element->narzedzie.zwrocObecnyStan()->rysuj(dt);
	}
}