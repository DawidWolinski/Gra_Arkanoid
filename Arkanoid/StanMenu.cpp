#include "StanMenu.h"
#include "StanRozgrywki.h"


StanMenu::StanMenu(ElementySilnikaWskaznik elementy, bool start)
	:ktoryNapis(0), czasAnimacja(0.f), i(0), rysujTaito(false), czasSekwencja(0.f), wDol(Kierunek::Dol), wGore(Kierunek::Gora), 
	element(elementy)
{
	//Sprawdza, czy wyswietlac intro
	if (!start)
		this->rysujOpcje = true;
	else
		this->rysujOpcje = false;

	//Sprawdza, czy istnieje zapis gry
	if (this->element->daneJson[1] == nullptr)
		this->zapisGry = false;
	else 
		this->zapisGry = true;
}


void StanMenu::inicjalizuj()
{
	//Widok - powiekszenie x3
	this->widok.setSize(sf::Vector2f(SZEROKOSC, WYSOKOSC));
	this->widok.setCenter(sf::Vector2f(this->widok.getSize().x / 2.f, this->widok.getSize().y / 2.f));
	this->element->okno.setView(this->widok);

	this->element->skala = std::floor(this->element->okno.getSize().x / SZEROKOSC);
	
	//Czcionki
	this->ustawCzcionki();

	//Tekstury
	this->zaladujTekstury();
	
	//Dzwieki
	this->zaladujDzwieki();
}

void StanMenu::zaladujTekstury()
{
	//Logo "ARKANOID"
	this->element->zasoby.zaladujTeksture("tekstura_statek_arkanoid", PLIK_TEKSTURA_STATEK_ARKANOID);
	//Znak Copyright
	this->element->zasoby.zaladujTeksture("tekstura_copyright", PLIK_TEKSTURA_COPYRIGHT);

	//Tekstury do stanu rozgrywki
	this->element->zasoby.zaladujTeksture("tekstura_tlo", PLIK_TEKSTURA_TLO);
	this->element->zasoby.zaladujTeksture("tekstura_klocek", PLIK_TEKSTURA_KLOCEK);
	this->element->zasoby.zaladujTeksture("tekstura_paleta", PLIK_TEKSTURA_PALETA);
	this->element->zasoby.zaladujTeksture("tekstura_bonus", PLIK_TEKSTURA_BONUS);

	//Sprite - Logo gry "ARKANOID"
	this->logoGry.setTexture(this->element->zasoby.zwrocTeksture("tekstura_statek_arkanoid"));
	this->logoGry.setTextureRect(sf::IntRect(0, 0, 193, 42));
	this->logoGry.setOrigin(sf::Vector2f(this->logoGry.getGlobalBounds().width / 2.f, this->logoGry.getGlobalBounds().height / 2.f));
	this->logoGry.setPosition(sf::Vector2f(SZEROKOSC / 2.f, 60.f));

	//Sprite - Znak Copyright
	this->znakCopyright.setTexture(this->element->zasoby.zwrocTeksture("tekstura_copyright"));
	this->znakCopyright.setTextureRect(sf::IntRect(3, 2, 18, 18));
	this->znakCopyright.setOrigin(sf::Vector2f(this->znakCopyright.getGlobalBounds().width / 2.f, this->znakCopyright.getGlobalBounds().height / 2.f));
	this->znakCopyright.setPosition(sf::Vector2f(this->taito[1]->getGlobalBounds().left - 8.f, this->taito[1]->getPosition().y + 4.5f));
	this->znakCopyright.setScale(0.85f, 0.85f);
}

void StanMenu::ustawCzcionki()
{
	//Czcionka uzywana jedynie do logo firmy
	this->element->zasoby.zaladujCzcionke("czcionka_taito", PLIK_CZCIONKA_TAITO);

	//Napisy poczatkowe
	for (unsigned int i = 0; i < 3; i++)
		this->taito.push_back(std::make_unique<sf::Text>());

	this->ustawNapis(*this->taito[0], "TAITO", SZEROKOSC / 2.f, WYSOKOSC - 90.f, 27, sf::Color::Red, "czcionka_taito");
	this->ustawNapis(*this->taito[1], "1986 taito corp japan", SZEROKOSC / 2.f, this->taito[0]->getPosition().y + this->taito[0]->getGlobalBounds().height / 2.f + 25.f);
	this->ustawNapis(*this->taito[2], "all rights reserved", SZEROKOSC / 2.f, this->taito[1]->getPosition().y + this->taito[1]->getGlobalBounds().height / 2.f + 10.f);

	//Napisy menu
	for (unsigned int i = 0; i < 4; i++)
		this->napisy.push_back(std::make_unique<sf::Text>());
	
	this->ustawNapis(*this->napisy[0], "NEW GAME", SZEROKOSC / 2.f, 125.f, 10);
	this->ustawNapis(*this->napisy[1], "CONTINUE GAME", SZEROKOSC / 2.f, this->napisy[0]->getPosition().y + this->napisy[0]->getGlobalBounds().height + 20.f, 10);
	this->ustawNapis(*this->napisy[2], "HIGH SCORES", SZEROKOSC / 2.f, this->napisy[1]->getPosition().y + this->napisy[1]->getGlobalBounds().height + 20.f, 10);
	this->ustawNapis(*this->napisy[3], "EXIT", SZEROKOSC / 2.f, this->napisy[2]->getPosition().y + this->napisy[2]->getGlobalBounds().height + 20.f, 10);

	//Ustawia "continue game" na szaro jesli nie ma zapisu gry
	if (!this->zapisGry) 
		this->napisy[1]->setFillColor(sf::Color(75, 72, 72));
}

void StanMenu::zaladujDzwieki()
{
	//Laduje wiekszosc dzwiekow do stanu rozgrywki
	this->element->zasoby.zaladujDzwiek("dzwiek_start", PLIK_DZWIEK_START);
	this->element->zasoby.zaladujDzwiek("dzwiek_smierc_paleta", PLIK_DZWIEK_SMIERC_PALETA);
	this->element->zasoby.zaladujDzwiek("dzwiek_odbicie_paleta", PLIK_DZWIEK_ODBICIE_PALETA);
	this->element->zasoby.zaladujDzwiek("dzwiek_odbicie_klocek", PLIK_DZWIEK_ODBICIE_KLOCEK);
	this->element->zasoby.zaladujDzwiek("dzwiek_odbicie_klocek_zloty", PLIK_DZWIEK_ODBICIE_KLOCEK_ZLOTY);
	this->element->zasoby.zaladujDzwiek("dzwiek_wydluzenie", PLIK_DZWIEK_WYDLUZENIE);
	this->element->zasoby.zaladujDzwiek("dzwiek_zycie", PLIK_DZWIEK_ZYCIE);
	this->element->zasoby.zaladujDzwiek("dzwiek_laser", PLIK_DZWIEK_LASER);
	this->element->zasoby.zaladujDzwiek("dzwiek_start_boss", PLIK_DZWIEK_START_BOSS);
}

void StanMenu::sekwencja(float dt)
{
	//Sekwencja wyswietlana po wlaczeniu gry (logo firmy --> glowne menu)

	this->czasSekwencja += dt;

	switch (this->i)
	{
	case 0:		//Logo firmy
		this->rysujTaito = true;
		this->i++;
		break;
	case 1:		//Znika po 2.5 sek
		if (this->czasSekwencja >= 2.5f)
		{
			i++;
			this->rysujTaito = false;
		}
		break;
	case 2:		//Glowne menu (opcje)
		if (this->czasSekwencja >= 4.f)
		{
			this->rysujOpcje = true;
			this->czasSekwencja = 0.f;
			this->i = 0;
		}
		break;
	}
}

void StanMenu::przechwytujWydarzenia(const float& dt)
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
				if(this->rysujOpcje)
					this->zmianaNapisu(this->wDol);
			if (this->element->wydarzenie.key.code == sf::Keyboard::W || this->element->wydarzenie.key.code == sf::Keyboard::Up)
				if (this->rysujOpcje)
					this->zmianaNapisu(this->wGore);
			if (this->element->wydarzenie.key.code == sf::Keyboard::Enter)
			{
				//Przerywa sekwencje intro i od razu otwiera menu glowne
				if (!this->rysujOpcje)
				{
					this->rysujTaito = false;
					this->rysujOpcje = true;
					this->i = 0;
				}
				else
					this->wyborOpcji();
			}
			//Zmniejsza rozdzielczosc ekranu
			if (this->element->wydarzenie.key.code == sf::Keyboard::K)
			{
				if (this->element->skala > 0.f)
				{
					if(this->element->skala > 1)
						this->element->skala--;
					this->element->okno.setSize(sf::Vector2u(static_cast<int>(SZEROKOSC) * static_cast<int>(this->element->skala), static_cast<int>(WYSOKOSC) * static_cast<int>(this->element->skala)));
					this->element->gornyInterface.aktualizujCzcionki(this->element->skala);
					this->aktualizujCzcionki();
					this->element->okno.setPosition(sf::Vector2i(this->element->okno.getPosition().x + static_cast<int>(SZEROKOSC / 2.f), this->element->okno.getPosition().y + static_cast<int>(WYSOKOSC / 2.f)));
				}
				
			}
			//Zwieksza rozdzielczosc ekranu
			if (this->element->wydarzenie.key.code == sf::Keyboard::L)
			{
				this->element->skala++;
				this->element->okno.setSize(sf::Vector2u(static_cast<int>(SZEROKOSC) * static_cast<int>(this->element->skala), static_cast<int>(WYSOKOSC) * static_cast<int>(this->element->skala)));
				this->element->gornyInterface.aktualizujCzcionki(this->element->skala);
				this->aktualizujCzcionki();
				this->element->okno.setPosition(sf::Vector2i(this->element->okno.getPosition().x + static_cast<int>(SZEROKOSC/2.f), this->element->okno.getPosition().y + static_cast<int>(WYSOKOSC / 2.f)));
			}
            break;
        }
    }
}

void StanMenu::animujNapis(const float& dt)
{
	//Animacja aktywnego napisu (miga na zolto)
	this->czasAnimacja += dt;

	if (this->czasAnimacja >= 0.5f)
	{
		this->czasAnimacja = 0.f;

		if (this->napisy[this->ktoryNapis]->getFillColor().Yellow == this->napisy[this->ktoryNapis]->getFillColor()) this->napisy[this->ktoryNapis]->setFillColor(sf::Color::White);
		else this->napisy[this->ktoryNapis]->setFillColor(sf::Color::Yellow);	
	}
}

void StanMenu::zmianaNapisu(Kierunek kierunek)
{
	if (kierunek == this->wDol)
	{
		if (this->ktoryNapis == this->napisy.size() - 1) this->ktoryNapis = 0;
		else this->ktoryNapis++;

		if (this->ktoryNapis == 1 && !this->zapisGry)
			this->ktoryNapis++;
	}
	else
	{
		if (this->ktoryNapis == 0) this->ktoryNapis = this->napisy.size() - 1;
		else this->ktoryNapis--;

		if (this->ktoryNapis == 1 && !this->zapisGry)
			this->ktoryNapis--;
	}

	//Usuwa podswietlenie z poprzedniego napisu
	for (unsigned int i = 0; i < this->napisy.size(); i++)
	{
		if (i == 1 && this->zapisGry || i != this->ktoryNapis && i != 1)
			this->napisy[i]->setFillColor(sf::Color::White);
	}
		
	//Podswietla wybrany napis na zolto
	this->napisy[this->ktoryNapis]->setFillColor(sf::Color::Yellow);
	this->czasAnimacja = 0.f;
}

void StanMenu::wyborOpcji()
{
	switch (this->ktoryNapis)
	{
	case 0:			//Nowa gra
		this->element->narzedzie.dodajStan(StanWskaznik(new StanRozgrywki(this->element, false)));
		//this->element->narzedzie.dodajStan(StanWskaznik(new StanBoss(this->element, 5)), true);
		//this->element->narzedzie.dodajStan(StanWskaznik(new StanIntro(this->element)), true);
		break;
	case 1:			//Kontynuacja zapisanej gry
		this->element->narzedzie.dodajStan(StanWskaznik(new StanRozgrywki(this->element, true)));
		break;
	case 2:			//Tablica wynikow
		this->element->narzedzie.dodajStan(StanWskaznik(new TablicaWynikow(0, 0, this->element, false)), false);
		break;
	case 3:			//Wyjscie
		this->element->okno.close();
		break;
	default:
		break;
	}
}

void StanMenu::aktualizuj(const float& dt)
{
	if (!this->rysujOpcje) 
		this->sekwencja(dt);

	this->animujNapis(dt);

	this->element->gornyInterface.aktualizuj(dt);
}


void StanMenu::rysuj(const float& dt)
{
	this->element->okno.clear();

	if(this->rysujOpcje) 
		for (auto& napis : this->napisy) 
			this->element->okno.draw(*napis);
	else if (this->rysujTaito)
	{
		for (auto& napis : this->taito)
			this->element->okno.draw(*napis);

		this->element->okno.draw(this->znakCopyright);
	}
		
	this->element->okno.draw(this->logoGry);

	this->element->gornyInterface.rysuj();

	this->element->okno.display();
}

void StanMenu::ustawNapis(sf::Text& tekst, std::string napis, float x, float y, unsigned int rozmiar, sf::Color kolor, std::string czcionka)
{
	tekst.setFont(this->element->zasoby.zwrocCzcionke(czcionka));
	tekst.setCharacterSize(rozmiar * static_cast<int>(this->element->skala));
	tekst.setString(napis);
	tekst.setScale(sf::Vector2f(1 / this->element->skala, 1 / this->element->skala));
	tekst.setOrigin(sf::Vector2f(std::round(tekst.getLocalBounds().width / 2.f), std::round(tekst.getLocalBounds().height / 2.f)));
	tekst.setFillColor(kolor);
	tekst.setPosition(sf::Vector2f(std::round(x), std::round(y)));
}

void StanMenu::aktualizujCzcionki()
{
	//Aktualizuje rozmiary napisow po zmianie rozdzielczosci okna

	for (auto& napis : this->napisy)
	{
		napis->setCharacterSize(10 * static_cast<int>(this->element->skala));
		napis->setScale(sf::Vector2f(1 / this->element->skala, 1 / this->element->skala));
		napis->setOrigin(sf::Vector2f(std::round(napis->getLocalBounds().width / 2.f), std::round(napis->getLocalBounds().height / 2.f)));
	}

	/*
	this->taito[0]->setCharacterSize(27 * static_cast<int>(this->element->skala));
	this->taito[1]->setCharacterSize(8 * static_cast<int>(this->element->skala));
	this->taito[2]->setCharacterSize(8 * static_cast<int>(this->element->skala));

	for (auto& napis : this->taito)
	{
		napis->setScale(sf::Vector2f(1 / this->element->skala, 1 / this->element->skala));
		napis->setOrigin(sf::Vector2f(std::round(napis->getLocalBounds().width / 2.f), std::round(napis->getLocalBounds().height / 2.f)));
	}
	*/
}