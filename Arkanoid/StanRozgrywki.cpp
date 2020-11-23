#include "StanRozgrywki.h"

StanRozgrywki::StanRozgrywki(ElementySilnikaWskaznik elementy, bool wczytaj, const int& ktoryPoziom)
    : czasOdczekanie(0.f), hp(2), wczytaj(wczytaj), ktoryPoziom(ktoryPoziom), przyznaneHP(0), Zaden(Kierunek::Zaden),
    losuj(0, 2), element(elementy)   
{
 
}

void StanRozgrywki::inicjalizuj()
{
    //Dzwieki
    this->ustawDzwieki();
    
    //Obiekty
    this->poziom = std::make_unique<Poziom>(this->element);
    this->gracz = std::make_unique<Paleta>(this->element);
    this->tlo = std::make_unique<sf::Sprite>();
    this->pilki.push_back(std::make_unique<Pilka>(this->element));
    this->bonus = std::make_unique<Bonus>(this->element);
    this->menu = std::make_unique<MenuRozgrywka>(this->element);

    //Tworzy obecny poziom
    if (this->wczytaj)
        this->wczytajZapis();
    else
        this->poziom->stworzPoziom(this->klocki, this->tlo, this->hpGrafika, this->ktoryPoziom);
}

void StanRozgrywki::ustawDzwieki()
{
    //Dzwieki odbicia pilki posiadaja timery, ktore ograniczaja ilosc ich uzyc
    this->dzwiekOdbiciePaleta = std::make_pair(sf::Sound(), sf::Clock());
    this->dzwiekOdbicieKlocek = std::make_pair(sf::Sound(), sf::Clock());
    this->dzwiekOdbicieKlocekZloty = std::make_pair(sf::Sound(), sf::Clock());

    this->dzwiekSmierc.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_smierc_paleta"));
    this->dzwiekSmierc.setVolume(25.f);

    this->dzwiekOdbiciePaleta.first.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_odbicie_paleta"));
    this->dzwiekOdbiciePaleta.first.setVolume(25.f);

    this->dzwiekOdbicieKlocek.first.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_odbicie_klocek"));
    this->dzwiekOdbicieKlocek.first.setVolume(25.f);

    this->dzwiekOdbicieKlocekZloty.first.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_odbicie_klocek_zloty"));
    this->dzwiekOdbicieKlocekZloty.first.setVolume(25.f);

    this->dzwiekZycie.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_zycie"));
    this->dzwiekZycie.setVolume(25.f);
}

void StanRozgrywki::przechwytujWydarzenia(const float& dt)
{
    //STEROWANIE PALETA
    if (this->gracz->czyWRuchu && !this->menu->aktywne)
    {
        //RUCH W LEWO
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) && !this->gracz->ruchWPrawo)
        {
            if (this->gracz->getPosition().x - this->gracz->getGlobalBounds().width / 2.f > 9.f)
                this->gracz->move(-this->gracz->szybkosc * dt);
            else
                this->gracz->setPosition(sf::Vector2f(8.f + this->gracz->getGlobalBounds().width / 2.f, this->gracz->getPosition().y));
        }
        //RUCH W PRAWO
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            if (this->gracz->getPosition().x + this->gracz->getGlobalBounds().width / 2.f < SZEROKOSC - 9.f)
                this->gracz->move(this->gracz->szybkosc * dt);
            else if (this->bonus->czyPrzejscie)
            {
                this->gracz->move(this->gracz->szybkosc * dt);
                if(this->gracz->getPosition().x + this->gracz->getGlobalBounds().width / 2.f > SZEROKOSC - 6.f)
                    this->animacjaPrzejscia();
            }              
            else
                this->gracz->setPosition(sf::Vector2f(SZEROKOSC - 8.f - this->gracz->getGlobalBounds().width / 2.f, this->gracz->getPosition().y));

            this->gracz->ruchWPrawo = true;
        }
    }

    //POZOSTALE
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
            if (this->element->wydarzenie.key.code == sf::Keyboard::Escape || this->element->wydarzenie.key.code == sf::Keyboard::P)
            {
                this->menu->aktywne = !this->menu->aktywne;
                if (this->menu->aktywne)
                {
                    for (auto& pilka : this->pilki)
                        if (pilka->szybkosc.y < 0.f) this->menu->wGore = true;
                    this->menu->wyszarzNapis();
                }
                else
                    this->menu->wGore = false;       
            }                
            if (this->element->wydarzenie.key.code == sf::Keyboard::S || this->element->wydarzenie.key.code == sf::Keyboard::Down)
                if(this->menu->aktywne)
                    this->menu->zmianaNapisu(0);
            if (this->element->wydarzenie.key.code == sf::Keyboard::W || this->element->wydarzenie.key.code == sf::Keyboard::Up)
                if(this->menu->aktywne)
                    this->menu->zmianaNapisu(1);
            if (this->element->wydarzenie.key.code == sf::Keyboard::Enter && this->menu->aktywne)
            {
                switch (this->menu->ktoryNapis)
                {
                case 0:
                    this->menu->aktywne = false;
                    this->menu->wGore = false;
                    break;
                case 1:
                    this->zapiszGre();
                    this->element->gornyInterface.resetuj();
                    this->element->narzedzie.dodajStan(StanWskaznik(new StanMenu(this->element)));
                    break;
                case 2:
                    this->element->gornyInterface.resetuj();
                    this->element->narzedzie.dodajStan(StanWskaznik(new StanMenu(this->element)));
                    break;
                }                            
            }                              
            if (this->element->wydarzenie.key.code == sf::Keyboard::Space)
                this->uzycieSpacji();   
                
            break;

        case sf::Event::KeyReleased:
            if (this->element->wydarzenie.key.code == sf::Keyboard::Right || this->element->wydarzenie.key.code == sf::Keyboard::D)
                this->gracz->ruchWPrawo = false;
            break;
        }
    } 
}

void StanRozgrywki::uzycieSpacji()
{
    //Gdy lapanie pilki jest aktywne
    if (this->bonus->aktywnyRodzaj == 1 && !this->pilki.empty() && !this->pilki.back()->czyWRuchu)
    {
        this->pilki.back()->czyWRuchu = true;
        this->bonus->zielonyCzas = 0.f;
    }
    //Gdy strzelanie laserami jest aktywne
    else if (this->bonus->aktywnyRodzaj == 2)
    {
        this->bonus->strzelLaserem(this->gracz->getPosition());
    }    
}

void StanRozgrywki::animacjaPrzejscia()
{
    //Animacja przejscia przez portal do nastepnego poziomu
    this->gracz->czyPrzechodzi = true;
    this->dodajPunkty(10000);
    for (auto& pilka : this->pilki)
        pilka->czyWRuchu = false;
    this->bonus->czyWRuchu = false;
}

void StanRozgrywki::dodajPunkty(int punkty)
{
    this->element->gornyInterface.dodajPunkty(punkty);

    //Przyznaje HP po zdobyciu 20000 punktow
    if(this->przyznaneHP == 0 && this->element->gornyInterface.ilePunktow() >= 20000)
    {
        if (this->hp < 6) 
            this->hp++;
        this->przyznaneHP++;
        this->dzwiekZycie.play();
    }
    //Przyznaje HP po zdobyciu 60000 punktow i nastepnie za kazde kolejne zdobyte 60000 punktow
    else if(this->przyznaneHP > 0 && this->element->gornyInterface.ilePunktow() >= 60000 * this->przyznaneHP)
    {
        if (this->hp < 6) 
            this->hp++;
        this->przyznaneHP++;
        this->dzwiekZycie.play();
    }
}

void StanRozgrywki::dzwiekOdbicie(std::pair<sf::Sound, sf::Clock>& dzwiek)
{
    //Ogranicza liczbe dzwiekow odbicia pilek od palety i klockow
    if (dzwiek.second.getElapsedTime().asSeconds() >= 0.05f)
    {
        dzwiek.first.play();
        dzwiek.second.restart();
    }
}

void StanRozgrywki::sprawdzKolizje()
{
    //SPRAWDZA KOLIZJE PILEK
    for (unsigned int k = 0; k < this->pilki.size(); k++)
    {
        //Z CEGLAMI
        for (unsigned int i = 0; i < this->klocki.size(); i++)
        {
            if (this->pilki[k]->getGlobalBounds().intersects(this->klocki[i]->getGlobalBounds()))
            {
                Kierunek kierunekOdbicia = this->klocki[i]->ktoraSciana(this->pilki[k]->getPosition(), this->pilki[k]->szybkosc);
                
                if (kierunekOdbicia != Zaden)
                {
                    this->pilki[k]->odbij(kierunekOdbicia);
                    this->klocki[i]->uderzenie();
                   
                    if (this->klocki[i]->rodzaj < 8)
                        this->dzwiekOdbicie(this->dzwiekOdbicieKlocek);
                    else
                        this->dzwiekOdbicie(this->dzwiekOdbicieKlocekZloty);

                    if (this->klocki[i]->hp < 1)
                    {
                        if (this->pilki.size() == 1 && this->klocki[i]->rodzaj != 8)
                            //25% szans na zrzucenie bonusu
                            if(this->losuj(this->element->generator) == 0)
                                this->bonus->zrzuc(this->klocki[i]->getPosition());

                        this->pilki[k]->zwiekszSzybkosc();
                        this->dodajPunkty(klocki[i]->ilePunktow);

                        this->klocki.erase(this->klocki.begin() + i);
                    }   
                }
            }            
        }

        //Z PALETA
        if (this->pilki[k]->getGlobalBounds().intersects(this->gracz->getGlobalBounds()))
        {
            int miejsceKolizji = this->gracz->miejsceKolizji(*this->pilki[k]);
            if (miejsceKolizji != 0)
            {
                this->pilki[k]->odbijPaleta(miejsceKolizji);
                this->pilki[k]->odbita = false;

                //GDY JEST WLACZONE LAPANIE
                if (this->bonus->aktywnyRodzaj == 1 && this->pilki[k]->czyWRuchu)
                {
                        this->gracz->zlapPilke(*this->pilki[k]);
                        this->pilki[k]->odlegloscOdPalety = this->pilki[k]->getPosition().x - this->gracz->getPosition().x;
                        this->pilki[k]->czyWRuchu = false;                  
                }
            }
        }
        else if (!this->pilki[k]->odbita && this->pilki[k]->czyWRuchu)
        {
            this->dzwiekOdbicie(this->dzwiekOdbiciePaleta);
            this->pilki[k]->odbita = true;
        }

        //Z PODLOGA
        if (this->pilki[k]->getPosition().y > WYSOKOSC + this->pilki[k]->getGlobalBounds().height)
        {
            this->pilki.erase(this->pilki.begin() + k);
            if (this->pilki.empty())
            {
                this->hp--;
                this->bonus->setPosition(0.f - this->bonus->getGlobalBounds().width, this->bonus->getPosition().y);
                this->bonus->aktywnyRodzaj = 10;
                this->gracz->stan = &this->gracz->smiercAnimacja;
                this->dzwiekSmierc.play();
            }
        }
    }

    //KOLIZJE BONUSOW Z PALETA
    if (this->gracz->getGlobalBounds().intersects(this->bonus->getGlobalBounds()))
        this->bonus->aktywujBonus(this->gracz, this->pilki.back());

    //KOLIZJE LASEROW Z CEGLAMI
    for (unsigned int i = 0; i < this->bonus->lasery.size(); i++)
    {
        bool trafiony = false;
        for (int j = this->klocki.size() - 1; j >= 0; j--)
        {
            if (this->bonus->lasery[i]->getGlobalBounds().intersects(this->klocki[j]->getGlobalBounds()) && !this->bonus->lasery[i]->czyKolizja)
            {
                trafiony = true;
                this->klocki[j]->uderzenie();
                if (this->klocki[j]->hp < 1)
                {
                    this->dodajPunkty(klocki[j]->ilePunktow);
                    this->klocki.erase(this->klocki.begin() + j);
                } 
            }
        }
        if (trafiony)
            this->bonus->lasery[i]->czyKolizja = true;
                              
    }

    if (!Klocek::usunieteKlocki.empty())
    {
        for (auto& klocek : this->klocki)
            klocek->sprawdzKlockiObok();
        Klocek::usunieteKlocki.clear();
    }
}

void StanRozgrywki::zarzadzajBonusami(const float& dt)
{
    switch (this->bonus->aktywnyRodzaj)
    {
    case 0:     //SPOWOLNIENIE
        for (auto& pilka : this->pilki)
            pilka->zmniejszSzybkosc(75.f);

        this->bonus->aktywnyRodzaj = 10;
        break;

    case 1:     //LAPANIE PILKI
        if (!this->pilki.empty() && !this->pilki.back()->czyWRuchu)
        {
            this->pilki.back()->setPosition(sf::Vector2f(this->gracz->getPosition().x + this->pilki.back()->odlegloscOdPalety, this->pilki.back()->getPosition().y));
            this->bonus->zielonyCzas += dt;
            if (this->bonus->zielonyCzas >= this->bonus->zielonyLimit) this->uzycieSpacji();
        }            
        break;

    case 4:     //3 PILKI
        for (float i = -1; i < 1; i++)
        {
            this->pilki.push_back(std::make_unique<Pilka>(*this->pilki.front()));
            this->pilki.back()->czyWRuchu = true;
            this->pilki.back()->zmienKat(i * 5.f + this->pilki.front()->stopnie + (i + 1.f) * 5.f);
        }
        this->bonus->aktywnyRodzaj = 10;
        break;

    case 5:     //POMINIECIE POZIOMU
        this->bonus->czyPrzejscie = true;
        this->bonus->aktywnyRodzaj = 10;
        break;

    case 6:     //DODATKOWE ZYCIE
        if (this->hp < 6) this->hp++;
        this->dzwiekZycie.play();
        this->bonus->aktywnyRodzaj = 10;
        break;

    default:
        break;
    }
}

void StanRozgrywki::sprawdzajStanGry(const float& dt)
{
        //JESLI ZNISZCZONO WSZYSTKIE KLOCKI       ||    JESLI UZYTO PRZEJSCIA LUB DOSZLO DO SMIERCI
    if (this->klocki.size() == Klocek::ileZlotych || this->gracz->getGlobalBounds().left >= SZEROKOSC)
    {
        if (this->czasOdczekanie == 0.f)
        {
            while (!this->pilki.empty()) this->pilki.pop_back();
            this->gracz->czyRysowac = false;
            this->bonus->czyRysowac = false;
        }

        this->czasOdczekanie += dt;
        if (this->czasOdczekanie > 1.f)
        {  
            //Koniec gry
            if (this->hp == 0)
                this->element->narzedzie.dodajStan(StanWskaznik(new TossCoin(this->poziom->numer + 1, this->element)));
            //Respawn
            else if (this->gracz->smierc)
                this->poziom->start = true;
            //Nastepny poziom
            else if (this->poziom->numer < this->poziom->liczbaZwyklychPoziomow - 1)
                this->poziom->stworzPoziom(this->klocki, this->tlo, this->hpGrafika);
            //Ostatni poziom
            else if (this->poziom->numer == this->poziom->liczbaZwyklychPoziomow - 1)
                this->element->narzedzie.dodajStan(StanWskaznik(new StanBoss(this->element, this->hp)), true);

            this->resetuj();   
        }
    }
}

void StanRozgrywki::resetuj()
{
    //Pilki
    this->pilki.push_back(std::make_unique<Pilka>(this->element));
    //Gracz
    this->gracz.reset();
    this->gracz = std::make_unique<Paleta>(this->element);
    //Bonus
    this->bonus.reset();
    this->bonus = std::make_unique<Bonus>(this->element);
    //Animacje transformacji palety
    this->gracz->transformacja.resetuj();

    this->czasOdczekanie = 0.f;
}

void StanRozgrywki::zapiszGre()
{
    //Jesli sa 3 pilki w grze to zapisuje predkosc tej najwolniejszej
    float szybkosc = 302.5f;
    for (auto& pilka : this->pilki)
        if (pilka->stalaSzybkosc < szybkosc) 
            szybkosc = pilka->stalaSzybkosc;

    json dane = {
        {"hp", this->hp},
        {"score", this->element->gornyInterface.ilePunktow()},
        {"level", this->poziom->numer},
        {"speed", szybkosc}
    };

    //Dane na temat zniszczonych klockow
    json ukladKlockow;

    for (unsigned int i = 0; i < this->klocki.size(); i++)
        ukladKlockow.push_back({ this->klocki[i]->getPosition().x, this->klocki[i]->getPosition().y, this->klocki[i]->rodzaj, this->klocki[i]->hp });

    this->element->daneJson[1] = dane;
    this->element->daneJson[2] = ukladKlockow;

    std::ofstream file(PLIK_DANE);
    file << this->element->daneJson << std::endl;
}

void StanRozgrywki::wczytajZapis()
{
    //Odczytuje dane dotyczace: hp, punktow, poziomu oraz szybkosci pilki
    this->hp = this->element->daneJson[1]["hp"];
    if(this->element->daneJson[1]["score"] != 0)
        this->element->gornyInterface.dodajPunkty(this->element->daneJson[1]["score"]);
    this->ktoryPoziom = this->element->daneJson[1]["level"];
    this->pilki.back()->stalaSzybkosc = this->element->daneJson[1]["speed"];


    //Odczytuje uklad obecnego poziomu (ktore klocki zostaly zniszczone, a ktore nie)
    this->poziom->stworzPoziom(this->klocki, this->tlo, this->hpGrafika, this->ktoryPoziom, true);


    //Na podstawie ilosci punktow sprawdza ile razy przyznano hp
    if (this->element->gornyInterface.ilePunktow() >= 20000 && this->element->gornyInterface.ilePunktow() < 60000)
        this->przyznaneHP = 1;
    else if(this->element->gornyInterface.ilePunktow() >= 60000)
        this->przyznaneHP = 1 + static_cast<int>(this->element->gornyInterface.ilePunktow() / 60000);

    //Usuwa obecny zapis
    this->element->daneJson[1] = nullptr;
    this->element->daneJson[2] = nullptr;

    //Uwzglednia zmiany w pliku (usuniety zapis)
    std::ofstream file(PLIK_DANE);
    file << this->element->daneJson;

    this->wczytaj = false;
}

void StanRozgrywki::aktualizuj(const float& dt)
{
    if (!this->menu->aktywne)
    {
        if (this->poziom->start) 
            this->poziom->zacznijPoziom(this->gracz, this->pilki.back(), dt);

        this->gracz->aktualizuj(dt);

        for (auto& pilka : this->pilki) 
            pilka->aktualizuj(dt, sf::Vector2f(SZEROKOSC, WYSOKOSC));

        this->sprawdzKolizje();

        for (auto& klocek : this->klocki)   
            klocek->aktualizuj(dt);

        this->bonus->aktualizuj(dt);

        this->zarzadzajBonusami(dt);

        this->sprawdzajStanGry(dt);
    }
    else
        this->menu->aktualizuj(dt);
 
    this->element->gornyInterface.aktualizuj(dt);

}

void StanRozgrywki::rysuj(const float& dt)
{
    
    this->element->okno.clear();

    this->element->okno.draw(*this->tlo);

    for (auto& klocek : this->klocki) klocek->rysuj();

    for (auto& pilka : this->pilki) pilka->rysujCien();

    this->bonus->rysuj();

    this->gracz->rysuj();

    for (auto& pilka : this->pilki) pilka->rysuj();

    for (int i = 0; i < this->hp - 1; i++)
        this->element->okno.draw(*this->hpGrafika[i]);  

    this->poziom->rysuj();

    this->element->gornyInterface.rysuj();

    this->menu->rysuj();

    this->element->okno.display();
}
