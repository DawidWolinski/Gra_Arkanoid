#include "StanBoss.h"
#include "Animacja.h"

StanBoss::StanBoss(ElementySilnikaWskaznik elementy, int hp)
    : czasOdczekanie(0.f), hp(hp), punkty(this->element->gornyInterface.ilePunktow()), Zaden(Kierunek::Zaden), element(elementy)
{

}

void StanBoss::inicjalizuj()
{
    //Tekstury  
    this->element->zasoby.zaladujTeksture("tekstura_boss", PLIK_TEKSTURA_BOSS);
    this->element->zasoby.zaladujTeksture("tekstura_paleta", PLIK_TEKSTURA_PALETA);
    this->element->zasoby.zaladujTeksture("tekstura_bonus", PLIK_TEKSTURA_BONUS);
    this->element->zasoby.zaladujTeksture("tekstura_pocisk", PLIK_TEKSTURA_POCISK);

    //Dzwieki
    this->zaladujDzwieki();

    //Obiekty
    this->poziom = std::make_unique<Poziom>(this->element);
    this->gracz = std::make_unique<Paleta>(this->element);
    this->tlo = std::make_unique<sf::Sprite>();
    this->ramka = std::make_unique<sf::Sprite>();
    this->pilki.push_back(std::make_unique<Pilka>(this->element));
    this->menu = std::make_unique<MenuRozgrywka>(this->element, true);

    //Poziom
    this->poziom->stworzPoziom(this->tlo, this->ramka, this->hpGrafika);
    this->boss = std::make_unique<Boss>(this->element, this->ramka->getPosition());
}

void StanBoss::zaladujDzwieki()
{
    this->element->zasoby.zaladujDzwiek("dzwiek_odbicie_boss", PLIK_DZWIEK_ODBICIE_BOSS);
    this->element->zasoby.zaladujDzwiek("dzwiek_smierc_boss", PLIK_DZWIEK_SMIERC_BOSS);

    this->dzwiekOdbicieBoss.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_odbicie_boss"));
    this->dzwiekOdbicieBoss.setVolume(25.f);
    this->dzwiekOdbiciePaleta.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_odbicie_paleta"));
    this->dzwiekOdbiciePaleta.setVolume(25.f);
    this->dzwiekSmierc.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_smierc_paleta"));
    this->dzwiekSmierc.setVolume(25.f);

    this->element->zasoby.zaladujUtwor("dzwiek_outro", PLIK_DZWIEK_OUTRO);
    this->element->zasoby.zwrocUtwor("dzwiek_outro")->setVolume(25.f);
}

void StanBoss::uzycieSpacji()
{
    //puszczenie
    //koniec gry
}

void StanBoss::przechwytujWydarzenia(const float& dt)
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
        else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
        {
            if (this->gracz->getPosition().x + this->gracz->getGlobalBounds().width / 2.f < SZEROKOSC - 9.f)
                this->gracz->move(this->gracz->szybkosc * dt);
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
                this->menu->aktywne = !this->menu->aktywne;
            if ((this->element->wydarzenie.key.code == sf::Keyboard::S || this->element->wydarzenie.key.code == sf::Keyboard::Down) && this->menu->aktywne)
                this->menu->zmianaNapisu(0);
            if ((this->element->wydarzenie.key.code == sf::Keyboard::W || this->element->wydarzenie.key.code == sf::Keyboard::Up) && this->menu->aktywne)
                this->menu->zmianaNapisu(1);
            if (this->element->wydarzenie.key.code == sf::Keyboard::Enter && this->menu->aktywne)
            {
                switch (this->menu->ktoryNapis)
                {
                case 0:
                    this->menu->aktywne = false;
                    break;
                case 1:
                    this->element->gornyInterface.resetuj();
                    this->element->narzedzie.dodajStan(StanWskaznik(new StanMenu(this->element)));
                    break;
                }
            }    
            break;
        case sf::Event::KeyReleased:
            if (this->element->wydarzenie.key.code == sf::Keyboard::Right || this->element->wydarzenie.key.code == sf::Keyboard::D)
                this->gracz->ruchWPrawo = false;
            break;
        }
    }
}

void StanBoss::sprawdzKolizje()
{
    //KOLIZJA PILKI
    for (auto& pilka : this->pilki)
    {
        if (pilka->getGlobalBounds().intersects(this->boss->hitBox.getGlobalBounds()))
        {
            Kierunek kierunekOdbicia = this->boss->ktoraSciana(*pilka, pilka->szybkosc);

            if (kierunekOdbicia != Zaden)
            {
                pilka->odbij(kierunekOdbicia);
                this->boss->uderzenie();
                this->element->gornyInterface.dodajPunkty(1000);
                if(this->boss->stan == &this->boss->smierc)
                    while (!this->pilki.empty()) this->pilki.pop_back();
                this->dzwiekOdbicieBoss.play();
            }
        }

        //Z PALETA
        if (pilka->getGlobalBounds().intersects(this->gracz->getGlobalBounds()))
        {
            int miejsceKolizji = this->gracz->miejsceKolizji(*pilka);
            if (miejsceKolizji != 0)
            {
                pilka->odbijPaleta(miejsceKolizji);
                pilka->odbita = false;
            }
        }
        else if (!pilka->odbita)
        {
            this->dzwiekOdbiciePaleta.play();
            pilka->odbita = true;
        }
            

        //Z PODLOGA
        if (pilka->getPosition().y > WYSOKOSC + pilka->getGlobalBounds().height)
        {
            while (!this->pilki.empty()) this->pilki.pop_back();
            this->hp--;
            this->gracz->stan = &this->gracz->smiercAnimacja;
            this->dzwiekSmierc.play();
        }
    }  

    //KOLIZJA PALETY Z POCISKIEM
    for (unsigned int i = 0; i < this->boss->pociski.size(); i++)
    {
        
        if (std::get<0>(this->boss->pociski[i])->getGlobalBounds().intersects(this->gracz->getGlobalBounds()) && this->gracz->czyWRuchu)
        {
            this->boss->pociski.erase(this->boss->pociski.begin() + i);
            while (!this->pilki.empty()) this->pilki.pop_back();            
            if(this->poziom->start) this->poziom->resetujStart();
            this->hp--;
            this->boss->stan = &this->boss->zamkniecie;
            this->gracz->stan = &this->gracz->smiercAnimacja;
            this->dzwiekSmierc.play();
        }
        else if (std::get<0>(this->boss->pociski[i])->getPosition().y > WYSOKOSC)
            this->boss->pociski.erase(this->boss->pociski.begin() + i);
            
    }

}

void StanBoss::sprawdzajStanGry(const float& dt)
{
    //JESLI DOSZLO DO SMIERCI BOSSA LUB GRACZA
    if (this->gracz->getGlobalBounds().left >= SZEROKOSC)
    {
        if (this->czasOdczekanie == 0.f)
        {
            while (!this->pilki.empty()) this->pilki.pop_back();
            this->gracz->czyRysowac = false;
        }

        this->czasOdczekanie += dt;
        if (this->czasOdczekanie > 1.f)
        {
            if (this->hp == 0)
                this->element->narzedzie.dodajStan(StanWskaznik(new TossCoin(this->poziom->numer + 1, this->element)));               
            else if (this->gracz->smierc)
                this->poziom->start = true;

            this->resetuj();
        }
    }
    else if (!this->boss->czyRysowac)
    {
        this->czasOdczekanie += dt;

        if (this->boss->czyOutro && this->czasOdczekanie > 1.f)
        {
            this->element->zasoby.zwrocUtwor("dzwiek_outro")->play();
            this->boss->czyOutro = false;
        }
    }
}

void StanBoss::resetuj()
{
    //Pilki
    this->pilki.push_back(std::make_unique<Pilka>(this->element));

    //Gracz
    this->gracz.reset();
    this->gracz = std::make_unique<Paleta>(this->element);

    this->czasOdczekanie = 0.f;
}

void StanBoss::aktualizuj(const float& dt)
{
    if (!this->menu->aktywne)
    {
        if (this->poziom->start) this->poziom->zacznijPoziom(this->gracz, this->pilki.back(), dt);

        this->gracz->aktualizuj(dt);

        for (auto& pilka : this->pilki) pilka->aktualizuj(dt, sf::Vector2f(SZEROKOSC, WYSOKOSC));

        this->boss->aktualizuj(dt, this->gracz);

        this->sprawdzKolizje();

        this->sprawdzajStanGry(dt);
    }
    else
        this->menu->aktualizuj(dt);

    this->element->gornyInterface.aktualizuj(dt);
}

void StanBoss::rysuj(const float& dt)
{
    this->element->okno.clear();

    this->element->okno.draw(*this->tlo);

    this->element->okno.draw(*this->ramka);

    this->boss->rysuj();

    for (auto& pilka : this->pilki) pilka->rysujCien();

    this->gracz->rysuj();

    for (auto& pilka : this->pilki) pilka->rysuj();

    if (this->hp > 1)
        for (int i = 0; i < this->hp - 1; i++)
            this->element->okno.draw(*this->hpGrafika[i]);

    this->element->gornyInterface.rysuj();

    this->poziom->rysuj();

    this->menu->rysuj();

    this->element->okno.display();
}