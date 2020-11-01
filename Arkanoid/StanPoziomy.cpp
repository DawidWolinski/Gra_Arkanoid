#include "StanPoziomy.h"

StanPoziomy::StanPoziomy(ElementySilnikaWskaznik elementy)
	:ktoryNapis(0), czasAnimacja(0.f), i(0), element(elementy)
{
	this->widok.setSize(sf::Vector2f(SZEROKOSC, WYSOKOSC));
	this->widok.setCenter(sf::Vector2f(this->widok.getSize().x / 2.f, this->widok.getSize().y / 2.f));
	this->element->okno.setView(this->widok);

}

void StanPoziomy::inicjalizuj()
{
	this->poziom = std::make_unique<Poziom>(this->element);
	this->tlo = std::make_unique<sf::Sprite>();
}


void StanPoziomy::przechwytujWydarzenia(const float& dt)
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
			if (this->element->wydarzenie.key.code == sf::Keyboard::S || this->element->wydarzenie.key.code == sf::Keyboard::Down)
			{
				
			}
			if (this->element->wydarzenie.key.code == sf::Keyboard::W || this->element->wydarzenie.key.code == sf::Keyboard::Up)
			{
				
			}

			if (this->element->wydarzenie.key.code == sf::Keyboard::Enter)
			{
				this->element->narzedzie.dodajStan(StanWskaznik(new StanRozgrywki(this->element, this->ktoryNapis)), true);
			}
			break;
		}
	}

}

void StanPoziomy::animujNapis(const float& dt)
{
	this->czasAnimacja += dt;

	if (this->czasAnimacja >= 0.5f)
	{
		if (this->napisy[this->ktoryNapis]->getFillColor().Yellow == this->napisy[this->ktoryNapis]->getFillColor()) this->napisy[this->ktoryNapis]->setFillColor(sf::Color::White);
		else this->napisy[this->ktoryNapis]->setFillColor(sf::Color::Yellow);

		this->czasAnimacja = 0.f;
	}
}

void StanPoziomy::poziomy()
{
	if (this->i < this->poziom->tla.size())
	{

		this->poziom->stworzPoziom(this->klocki, this->tlo, this->hpGrafika, false, i);

		this->element->okno.draw(*this->tlo);
		for (auto& klocek : this->klocki) klocek->rysuj();

		this->teksturyZdjec.push_back(std::make_unique<sf::Texture>());
		this->teksturyZdjec.back()->create(this->element->okno.getSize().x, this->element->okno.getSize().y);
		this->teksturyZdjec.back()->update(this->element->okno);

		this->zdjeciaPoziomow.push_back(std::make_unique<sf::Sprite>(*this->teksturyZdjec.back()));
		this->zdjeciaPoziomow.back()->setScale(sf::Vector2f(0.09f, 0.09f));

		this->element->okno.clear();

		if (this->i == 0) this->zdjeciaPoziomow[0]->setPosition(this->zdjeciaPoziomow[0]->getGlobalBounds().width + 10.f, 0.f);
		if (this->i == 1) this->zdjeciaPoziomow[1]->setPosition(2*(this->zdjeciaPoziomow[1]->getGlobalBounds().width + 10.f), 0.f);

		if (this->i == 2) this->zdjeciaPoziomow[2]->setPosition( (this->zdjeciaPoziomow[2]->getGlobalBounds().width + 10.f), this->zdjeciaPoziomow[2]->getGlobalBounds().height + 10.f);
		if (this->i == 3) this->zdjeciaPoziomow[3]->setPosition(2 * (this->zdjeciaPoziomow[3]->getGlobalBounds().width + 10.f), this->zdjeciaPoziomow[3]->getGlobalBounds().height + 10.f);
		if (this->i == 4) this->zdjeciaPoziomow[4]->setPosition(0.f, 2*(this->zdjeciaPoziomow[4]->getGlobalBounds().height + 10.f));

		i++;
	}
}

void StanPoziomy::aktualizuj(const float& dt)
{
	this->poziomy();
}

void StanPoziomy::rysuj(const float& dt)
{

	this->element->okno.clear();

	
	
	if(this->i == this->poziom->tla.size()) for (auto& zdjecie : this->zdjeciaPoziomow) this->element->okno.draw(*zdjecie);
	//if (this->i == this->poziom->tla.size()) this->element->okno.draw(*this->zdjeciaPoziomow[0]);

	this->element->okno.display();


}