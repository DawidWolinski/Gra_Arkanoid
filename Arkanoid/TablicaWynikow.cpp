#include "TablicaWynikow.h"

TablicaWynikow::TablicaWynikow(int runda, int wynik, ElementySilnikaWskaznik elementy, bool czyWygrana)
	:   numerRunda(runda), wynik(wynik), element(elementy), czyWygrana(czyWygrana), miejsce(999), 
        nowyWynik(true), limitWynikow(5), ktoryInicjal(0), literki{ "a", "", "" }, wpisany(false), 
        gameOver(false), czasAnimacja(0.f), czasPrzejscie(0.f)
{
    if (this->czyWygrana)
        this->napisRunda = "ALL";
    else
        this->napisRunda = std::to_string(runda);
}

void TablicaWynikow::inicjalizuj()
{
    //this->resetujWyniki(); //uzyj ten funkcji zamiast pobierzWyniki(), aby zresetowac wyniki
    this->ustawNapisy();
    this->pobierzWyniki();
    
}

void TablicaWynikow::przechwytujWydarzenia(const float& dt)
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
            if (this->element->wydarzenie.key.code == sf::Keyboard::Escape && !this->nowyWynik)
            {
                this->element->narzedzie.usunStan();
            }               
            if (this->element->wydarzenie.key.code == sf::Keyboard::Space && !this->nowyWynik)
            { 
                this->element->narzedzie.usunStan();
            }
            if (this->element->wydarzenie.key.code == sf::Keyboard::Enter)
            {
                if (this->nowyWynik)
                {
                    this->inicjaly[this->ktoryInicjal]->setFillColor(sf::Color::Yellow);

                    if (this->ktoryInicjal < 2)
                    {
                        this->ktoryInicjal++;
                        this->literki[this->ktoryInicjal] = "a";
                        this->napisImie.setString(this->napisImie.getString() + this->literki[this->ktoryInicjal]);
                    }
                    else if (!this->wpisany)
                        this->wpiszWynik();
                }
                else
                    this->element->narzedzie.usunStan();
            }
            if (this->element->wydarzenie.key.code == sf::Keyboard::BackSpace)
            {
                if (this->nowyWynik)
                {
                    if (this->ktoryInicjal > 0)
                    {
                        this->literki[this->ktoryInicjal] = "";
                        this->ktoryInicjal--;

                        std::string temp = this->napisImie.getString();
                        temp.erase(temp.length() - 1);
                        this->napisImie.setString(temp);
                    }
                }
                else
                    this->element->narzedzie.usunStan();    
            }           
            this->aktualizujInicjaly();
            break;
        case sf::Event::TextEntered:
            
                if (
                    (this->element->wydarzenie.text.unicode >= (char)'0' && this->element->wydarzenie.text.unicode <= (char)'9')
                    ||
                    (this->element->wydarzenie.text.unicode >= (char)'a' && this->element->wydarzenie.text.unicode <= (char)'z')
                    ||
                    (this->element->wydarzenie.text.unicode >= (char)'A' && this->element->wydarzenie.text.unicode <= (char)'Z')
                    ||
                    this->element->wydarzenie.text.unicode == (char)' '
                    )
                {
                    this->literki[this->ktoryInicjal] = (char)this->element->wydarzenie.text.unicode;

                    std::string temp = this->napisImie.getString();
                    temp.erase(temp.length() - 1);
                    this->napisImie.setString(temp);
                    this->napisImie.setString(this->napisImie.getString() + this->literki[this->ktoryInicjal]);        
                }
                this->aktualizujInicjaly();
            break;
		}
	}
}

void TablicaWynikow::pobierzWyniki()
{

    json obecny = {
        {"place", ""},
        {"score", this->wynik},
        {"round", this->napisRunda},
        {"name", ""}
    };

    //Jesli nie ma zadnych wynikow w pliku to z automatu idzie na pierwsze miejsce
    if (this->element->daneJson[0] == nullptr)
    {
        obecny["place"] = "1ST";
        this->element->daneJson[0].push_back(obecny);
        this->miejsce = 0;
    }
    else
    {
        //Sprawdza ktore miejsce
        for (unsigned int i = 0; i < this->element->daneJson[0].size(); i++)
        {
            if (this->wynik > this->element->daneJson[0][i]["score"])
            {
                this->miejsce = i;
                break;
            }
        }

        //Jesli ostatnie miejsce
        if (this->element->daneJson[0].size() < this->limitWynikow)
            this->miejsce = this->element->daneJson[0].size();

        //Sprawdza czy zajeto ktores miejsce. Jesli tak to dodaje nowy wynik
        if (this->miejsce == 999 || this->wynik == 0)
            nowyWynik = false;
        else
            this->element->daneJson[0].insert(this->element->daneJson[0].begin() + this->miejsce, obecny);
            
        //Usuwa ostatni element jesli wykroczono poza limit miejsc
        if (this->element->daneJson[0].size() > this->limitWynikow) this->element->daneJson[0].erase(this->element->daneJson[0].begin() + this->limitWynikow);
    }

    //Uaktualnia numery miejsc
    for (unsigned int i = 0; i < this->element->daneJson[0].size(); i++)
        this->element->daneJson[0][i]["place"] = zwrocKoncowke(i + 1);

    //TWORZY WYKAZ WYNIKOW NA PODSTAWIE DANYCH Z PLIKU
    sf::Text tekst;
    std::string napis;

    int i = 1;
    for (auto& pierwszy : this->element->daneJson[0])
    {
        std::vector<sf::Text> tymczasowy;
        int a = 0;
        for (auto& drugi : pierwszy)
        {           
            if (a != 3)
                napis = drugi;
            else
                napis = std::to_string(static_cast<int>(drugi));

            this->ustawNapis(tekst, napis, 0.f, this->naglowki2[0]->getPosition().y + i * (this->naglowki2[0]->getGlobalBounds().height + 10.f));
            tymczasowy.push_back(tekst);
            a++;
        }
        this->wyniki.push_back(tymczasowy);
        i++;
    }

    //USTAWIA ICH POZYCJE W POZIOMIE
    for (unsigned int a = 0; a < this->wyniki.size(); a++)
    {
        this->wyniki[a][0].setPosition(sf::Vector2f(this->naglowki2[2]->getPosition().x, this->wyniki[a][0].getPosition().y));
        this->wyniki[a][1].setPosition(sf::Vector2f(39.f, this->wyniki[a][0].getPosition().y));
        this->wyniki[a][2].setPosition(sf::Vector2f(this->naglowki2[1]->getPosition().x, this->wyniki[a][0].getPosition().y));
        this->wyniki[a][3].setPosition(sf::Vector2f(this->naglowki2[0]->getPosition().x, this->wyniki[a][0].getPosition().y));
    }

    //OBCENY WYNIK ZAZNACZA NA ZOLTO
    if (this->nowyWynik)
        for (auto& napis : this->wyniki[this->miejsce])
            napis.setFillColor(sf::Color::Yellow);
}

std::string TablicaWynikow::zwrocKoncowke(int liczba)
{
    std::string koncowki[] = { "ST", "ND", "RD", "TH", "TH", "TH", "TH", "TH", "TH", "TH" };

    if ((liczba % 100 >= 11 && liczba % 100 <= 13) || (liczba % 10 == 0))
        return std::to_string(liczba) + "TH";
    else
        return std::to_string(liczba) + koncowki[(liczba % 10) - 1];
}

void TablicaWynikow::wpiszWynik()
{
    //Wpisuje nowy wynik do pliku json

    this->wpisany = true;

    this->element->daneJson[0][this->miejsce]["name"] = this->napisImie.getString();
    this->wyniki[this->miejsce][0].setString(this->napisImie.getString());
    this->wyniki[this->miejsce][0].setOrigin(sf::Vector2f(this->wyniki[this->miejsce][0].getLocalBounds().width, this->wyniki[this->miejsce][0].getLocalBounds().height));        
 
    std::ofstream plik(PLIK_DANE);

    this->element->daneJson[1] = nullptr;
    this->element->daneJson[2] = nullptr;

    plik << this->element->daneJson << std::endl;
    
    this->gameOver = true;
}

void TablicaWynikow::aktualizujInicjaly()
{
    this->inicjaly[0]->setString(this->literki[0]);
    this->inicjaly[1]->setString(this->literki[1]);
    this->inicjaly[2]->setString(this->literki[2]);

    float odstep = this->napisImie.getGlobalBounds().width - (this->inicjaly[0]->getGlobalBounds().width + this->inicjaly[1]->getGlobalBounds().width + this->inicjaly[2]->getGlobalBounds().width);
    
    if(this->napisImie.getString().getSize() == 3) 
        odstep /= 2;

    this->inicjaly[1]->setPosition(sf::Vector2f(std::round(this->inicjaly[0]->getPosition().x + this->inicjaly[1]->getGlobalBounds().width + odstep), this->inicjaly[1]->getPosition().y));
    this->inicjaly[2]->setPosition(sf::Vector2f(std::round(this->inicjaly[1]->getPosition().x + this->inicjaly[2]->getGlobalBounds().width + odstep), this->inicjaly[2]->getPosition().y));   
}

void TablicaWynikow::animujInicjal(const float& dt)
{
    this->czasAnimacja += dt;

    if (this->czasAnimacja >= 0.5f)
    {
        this->czasAnimacja = 0.f;

        if (this->inicjaly[this->ktoryInicjal]->getFillColor().Red == this->inicjaly[this->ktoryInicjal]->getFillColor()) 
            this->inicjaly[this->ktoryInicjal]->setFillColor(sf::Color::White);
        else 
            this->inicjaly[this->ktoryInicjal]->setFillColor(sf::Color::Red);   
    }
}

void TablicaWynikow::animujNapis(const float& dt)
{
    this->czasAnimacja += dt;

    if (this->czasAnimacja >= 0.5f)
    {
        this->czasAnimacja = 0.f;

        if (this->back.getFillColor().Yellow == this->back.getFillColor()) 
            this->back.setFillColor(sf::Color::White);
        else 
            this->back.setFillColor(sf::Color::Yellow);
    }
}

void TablicaWynikow::przejscie(const float& dt)
{
    this->czasPrzejscie += dt;

    if (this->czasPrzejscie >= 5.f)
        this->element->narzedzie.dodajStan(StanWskaznik(new GameOver(this->element)));     
}

void TablicaWynikow::ustawNapisy()
{
    this->ustawNapis(this->enterInitials, "enter your initials!", SZEROKOSC / 2.f, 60.f, true, sf::Color::Red);

    for(unsigned int i = 0; i < 3; i++)
        this->naglowki1.push_back(std::make_unique<sf::Text>());

    this->ustawNapis(*this->naglowki1[0], "score", 80.f, 90.f, false, sf::Color::Yellow);
    this->ustawNapis(*this->naglowki1[1], "round", this->naglowki1[0]->getPosition().x + 50.f, 90.f, false, sf::Color::Yellow);
    this->ustawNapis(*this->naglowki1[2], "name", this->naglowki1[1]->getPosition().x + 45.f, 90.f, false, sf::Color::Yellow);


    //NAPISY POD NAGLOWKAMI GORNYMI
    this->ustawNapis(this->napisWynik, std::to_string(this->wynik), this->naglowki1[0]->getPosition().x, this->naglowki1[0]->getPosition().y + 20.f);
    this->ustawNapis(this->napisRunda1, this->napisRunda, this->naglowki1[1]->getPosition().x, this->naglowki1[1]->getPosition().y + 20.f, false, sf::Color::Yellow);
    this->ustawNapis(this->napisImie, "aaa", this->naglowki1[2]->getPosition().x, this->naglowki1[2]->getPosition().y + 20.f, false, sf::Color::Yellow);
    this->napisImie.setString("a");

    for (unsigned int i = 0; i < 3; i++)
        this->inicjaly.push_back(std::make_unique<sf::Text>(this->napisImie));

    this->inicjaly[1]->setString("");
    this->inicjaly[2]->setString("");

    //NAGLOWKI DOLNE (W TYM JEDEN NIEWIDOCZNY)
    for (unsigned int i = 0; i < this->naglowki1.size(); i++)
        this->naglowki2.push_back(std::make_unique<sf::Text>(*this->naglowki1[i]));

    this->naglowki2[0]->setPosition(sf::Vector2f(104.f, 140.f));
    this->naglowki2[1]->setPosition(sf::Vector2f(this->naglowki2[0]->getPosition().x + this->naglowki2[1]->getGlobalBounds().width + 15.f, 140.f));
    this->naglowki2[2]->setPosition(sf::Vector2f(this->naglowki2[1]->getPosition().x + this->naglowki2[2]->getGlobalBounds().width + 15.f, 140.f));

    //NAPIS RUNDA
    this->ustawNapis(this->napisRunda2, "round " + std::to_string(this->numerRunda), SZEROKOSC - 1.f, WYSOKOSC - 3.f);

    //NAPIS JESLI NIE MA NOWEGO WYNIKU
    for (unsigned int i = 0; i < 3; i++)
        this->brakNowegoWyniku.push_back(std::make_unique<sf::Text>());

    this->ustawNapis(*this->brakNowegoWyniku[0], "the following are", SZEROKOSC / 2.f, 65.f, true, sf::Color::Yellow);
    this->ustawNapis(*this->brakNowegoWyniku[1], "the records of the bravest", SZEROKOSC / 2.f, this->brakNowegoWyniku[0]->getPosition().y + this->brakNowegoWyniku[0]->getGlobalBounds().height/2.f + 10.f, true, sf::Color::Yellow);
    this->ustawNapis(*this->brakNowegoWyniku[2], "fighters of arkanoid", SZEROKOSC / 2.f, this->brakNowegoWyniku[1]->getPosition().y + this->brakNowegoWyniku[1]->getGlobalBounds().height/2.f + 10.f, true, sf::Color::Yellow);

    //NAPIS BACK
    this->ustawNapis(this->back, "back", SZEROKOSC/2.f, WYSOKOSC - 10.f, true);
}

void TablicaWynikow::ustawNapis(sf::Text& tekst, std::string napis, float x, float y, bool wysrodkuj, sf::Color kolor)
{
    tekst.setFont(this->element->zasoby.zwrocCzcionke("czcionka"));
    tekst.setCharacterSize(7 * static_cast<int>(this->element->skala));
    tekst.setString(napis);
    tekst.setScale(sf::Vector2f(1 / this->element->skala, 1 / this->element->skala));
    if(wysrodkuj) 
        tekst.setOrigin(sf::Vector2f(std::round(tekst.getLocalBounds().width / 2.f), std::round(tekst.getLocalBounds().height / 2.f)));
    else 
        tekst.setOrigin(sf::Vector2f(tekst.getLocalBounds().width, tekst.getLocalBounds().height));
    tekst.setFillColor(kolor);
    tekst.setPosition(sf::Vector2f(std::round(x), std::round(y)));
}

void TablicaWynikow::resetujWyniki()
{
    //Resetuje wyniki w pliku json do defaultowych

    this->element->daneJson = {
        {
            { {"name", "ssb"}, {"place", "1ST"}, {"round", "5"}, {"score", 50000} },
            { {"name", "snd"}, {"place", "2ND"}, {"round", "4"}, {"score", 45000} },
            { {"name", "tor"}, {"place", "3RD"}, {"round", "3"}, {"score", 40000} },
            { {"name", "onj"}, {"place", "4TH"}, {"round", "2"}, {"score", 35000} },
            { {"name", "gmd"}, {"place", "5TH"}, {"round", "1"}, {"score", 30000} }
        },
        nullptr, nullptr };

    std::ofstream file(PLIK_DANE);
    file << this->element->daneJson << std::endl;
}

void TablicaWynikow::aktualizuj(const float& dt)
{

    if (this->nowyWynik && !this->wpisany)
    {
        this->animujInicjal(dt);
    }
    else
        this->animujNapis(dt);

    if (this->gameOver)
    {
        this->przejscie(dt);
    }

    this->element->gornyInterface.aktualizuj(dt);

}

void TablicaWynikow::rysuj(const float& dt)
{
    this->element->okno.clear();


    if (this->nowyWynik)
    {
        this->element->okno.draw(this->enterInitials);

        this->element->okno.draw(this->napisWynik);

        this->element->okno.draw(this->napisRunda1);

        for (auto& inicjal : this->inicjaly)
            this->element->okno.draw(*inicjal);

        for (auto& naglowek : this->naglowki1)
            this->element->okno.draw(*naglowek);

        this->element->okno.draw(this->napisRunda2);
    }
    else
    {
        for (auto& napis : this->brakNowegoWyniku)
            this->element->okno.draw(*napis);

        this->element->okno.draw(this->back);
    }

    for (auto& naglowek : this->naglowki2)
        this->element->okno.draw(*naglowek);

    for (auto& pierwszy : this->wyniki)
        for (auto& drugi : pierwszy)
            this->element->okno.draw(drugi);

    this->element->gornyInterface.rysuj();

    this->element->okno.display();
}