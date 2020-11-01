#include "Poziom.h"

Poziom::Poziom(ElementySilnikaWskaznik elementy)
    :numer(-1), liczbaZwyklychPoziomow(32), rysujRunda(false), rysujReady(false), element(elementy)
{
    this->wytnijTla();
    this->ustawCzcionki();


    this->dzwiekStart.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_start"));
    this->dzwiekStart.setVolume(25.f);

    this->dzwiekStartBoss.setBuffer(this->element->zasoby.zwrocDzwiek("dzwiek_start_boss"));
    this->dzwiekStartBoss.setVolume(25.f);
}

void Poziom::ustawCzcionki()
{
    this->Runda.setFont(this->element->zasoby.zwrocCzcionke("czcionka"));
    this->Runda.setString("ROUND 1");
    this->Runda.setCharacterSize(8 * static_cast<int>(this->element->skala));
    this->Runda.setScale(sf::Vector2f(1 / this->element->skala, 1 / this->element->skala));
    this->Runda.setOrigin(std::round(this->Runda.getLocalBounds().width / 2.f), std::round(this->Runda.getLocalBounds().height / 2.f));
    this->Runda.setFillColor(sf::Color::White);
    this->Runda.setPosition(sf::Vector2f(SZEROKOSC/2.f, WYSOKOSC - 80.f));

    this->Ready.setFont(*this->Runda.getFont());
    this->Ready.setString("READY");
    this->Ready.setCharacterSize(8 * static_cast<int>(this->element->skala));
    this->Ready.setScale(sf::Vector2f(1 / this->element->skala, 1 / this->element->skala));
    this->Ready.setOrigin(std::round(this->Ready.getLocalBounds().width / 2.f), std::round(this->Ready.getLocalBounds().height / 2.f));
    this->Ready.setFillColor(sf::Color::White);
    this->Ready.setPosition(sf::Vector2f(SZEROKOSC/2.f, this->Runda.getPosition().y + 15.f));  
}

void Poziom::wytnijTla()
{
    int szerokosc = 224, wysokosc = 240;
    int odlegloscX = 8, odlegloscY = 16;
    int ileTekstur = 5;

    for (int i = 0; i < ileTekstur; i++)
        this->tla.push_back(sf::IntRect(i * (szerokosc + odlegloscX), 0, szerokosc, wysokosc));
   

    //Zwykle poziomy - co 4 poziomy powtarzaja sie tla
    int j = 0;
    for (unsigned int i = 0; i < this->liczbaZwyklychPoziomow; i++)
    {
        this->poziomy.push_back(this->tla[j]);
        j++;
        if (j > 3) j = 0;
    }
    
    //Ostatni poziom - ma oddzielne tlo
    this->poziomy.push_back(this->tla[4]);
}

void Poziom::rysuj()
{
    if (this->rysujRunda) this->element->okno.draw(this->Runda);
    if (this->rysujReady) this->element->okno.draw(this->Ready);
}

//STANDARDOWY POZIOM
void Poziom::stworzPoziom(std::vector<std::unique_ptr<Klocek>>& klocki, std::unique_ptr<sf::Sprite>& tlo, std::vector<std::unique_ptr<sf::Sprite>>& hpGrafika,int ktoryPoziom, bool wczytaj)
{ 
    //Numery poziomow zaczynaja sie od 0 (wiec poziom pierwszy to numer 0)

    if (ktoryPoziom == 0 && this->numer != -1)
        this->numer++;
    else
        this->numer = ktoryPoziom;
        
    this->ustawTlo(tlo, hpGrafika);
    for (auto& obiekt : hpGrafika) obiekt->setTexture(*tlo->getTexture());
    this->ustawUklad(klocki, wczytaj);
    this->Runda.setString("ROUND " + std::to_string(this->numer + 1));
    this->Runda.setOrigin(std::round(this->Runda.getLocalBounds().width / 2.f), std::round(this->Runda.getLocalBounds().height / 2.f));
    this->start = true;
}

//BOSS
void Poziom::stworzPoziom(std::unique_ptr<sf::Sprite>& tlo, std::unique_ptr<sf::Sprite>& ramka, std::vector<std::unique_ptr<sf::Sprite>>& hpGrafika)
{
    //Numery poziomow zaczynaja sie od 0 (wiec poziom pierwszy to numer 0)

    this->numer = this->poziomy.size() - 1;
   
    this->ustawTlo(tlo, hpGrafika);
    for (auto& obiekt : hpGrafika) obiekt->setTexture(*tlo->getTexture());
    ramka->setTexture(this->element->zasoby.zwrocTeksture("tekstura_boss"));
    ramka->setTextureRect(sf::IntRect(0, 0, 88, 135));
    ramka->setPosition(sf::Vector2f(72.f, 16.f+17.f));
    this->Runda.setString("ROUND " + std::to_string(this->numer + 1));
    this->Runda.setOrigin(std::round(this->Runda.getLocalBounds().width / 2.f), std::round(this->Runda.getLocalBounds().height / 2.f));
    this->start = true;
}

void Poziom::ustawTlo(std::unique_ptr<sf::Sprite>& tlo, std::vector<std::unique_ptr<sf::Sprite>>& hpGrafika)
{
    if (tlo->getTexture() == NULL) 
        tlo->setTexture(element->zasoby.zwrocTeksture("tekstura_tlo"));
    
    tlo->setTextureRect(this->poziomy[this->numer]);
    tlo->setPosition(sf::Vector2f(0.f, 16.f));

    for (int i = 0; i < 5; i++)
    {
        if (hpGrafika.size() < 5)
            hpGrafika.push_back(std::make_unique<sf::Sprite>(element->zasoby.zwrocTeksture("tekstura_tlo")));

        hpGrafika[i]->setTextureRect(sf::IntRect(this->poziomy[this->numer].left + 8, this->poziomy[this->numer].top + 240, 16, 8));
        hpGrafika[i]->setPosition(tlo->getPosition().x + 10.f + i * 16, tlo->getGlobalBounds().top + tlo->getGlobalBounds().height - hpGrafika[i]->getGlobalBounds().height);
    }
}

void Poziom::ustawUklad(std::vector<std::unique_ptr<Klocek>>& klocki, bool wczytaj)
{
    klocki.clear();
    Klocek::ileZlotych = 0;

    //Standardowe tworzenie poziomow na podstawie konkretnego ukladu (z funkcji wybierzUklad())
    if (!wczytaj)
    {
        std::vector<int> uklad = wybierzUklad();
        if (uklad.size() > 0 && uklad.size() % 13 == 0)
        {
            int rzedy = uklad.size() / 13;
            for (int i = 0; i < rzedy; i++)
                for (int j = 0; j < 13; j++)
                    if (uklad[i * 13 + j] != 100)
                    {
                        klocki.push_back(std::make_unique<Klocek>(uklad[i * 13 + j], float(j) * 16.f + 16.f, float(i) * 8.f + 28.f, this->element, this->numer + 1));
                        if (uklad[i * 13 + j] == 9) Klocek::ileZlotych++;
                    }
        }
    }
    //Odtwarzanie zapisanego poziomu (wraz ze zniszczonymi klockami) z pliku json
    else
    {
        Klocek::ileZlotych = 0;

        for (unsigned int i = 0; i < this->element->daneJson[2].size(); i++)
        {
            klocki.push_back(std::make_unique<Klocek>(this->element->daneJson[2][i][2], this->element->daneJson[2][i][0], this->element->daneJson[2][i][1], this->element, this->element->daneJson[1]["level"], this->element->daneJson[2][i][3]));
            if (klocki.back()->rodzaj == 9) Klocek::ileZlotych++;
        }
    }

    for (auto& klocek : klocki)
        klocek->sprawdzKlockiObok(klocki);
}

void Poziom::zacznijPoziom(std::unique_ptr<Paleta>& gracz, std::unique_ptr<Pilka>& pilki, const float& dt)
{
    //Sekwencja rozpoczynania poziomu (rowniez po smierci)

    switch (this->i)
    {
    case 0:                                     //Dzwiek
        if (this->numer < poziomy.size() - 1)
            this->dzwiekStart.play();
        else
            this->dzwiekStartBoss.play();
        this->i++;
        break;

    case 1:                                     //ROUND ...      
        this->rysujRunda = true;
        this->czasAnimacja += dt;
        if (this->czasAnimacja > 1.5f) i++;
        break;

    case 2:                                     //READY
        this->rysujReady = true;
        this->czasAnimacja += dt;
        if (this->czasAnimacja > 3.f) i++;
        break;

    case 3:                                     //Animacja pojawienia sie palety
        this->rysujRunda = false;
        this->rysujReady = false;
        gracz->czyRysowac = true;
        gracz->stan = &gracz->pojawienie;
        this->i++;
        break;

    case 4:                                     //Utworzenie pilki
        if(gracz->stan == &gracz->standardowa)
        {
            gracz->czyWRuchu = true;
            pilki->czyRysowac = true;
            this->i++;
            this->czasAnimacja = 0.f;
        }
        break;

    case 5:                                     //Odczekanie
        pilki->ustawNaPalecie(*gracz);
        this->czasAnimacja += dt;
        if (this->czasAnimacja > 3.f) this->i++;
        break;

    case 6:                                     //Puszczenie pilki
        pilki->czyWRuchu = true;
        this->start = false;
        this->czasAnimacja = 0.f;
        this->i = 0;
        break;
    }

}

void Poziom::resetujStart()
{
    this->start = false;
    this->i = 0;
    this->czasAnimacja = 0.f;
}

std::vector<int> Poziom::wybierzUklad()
{
    std::vector<int> ukladPoziomu;
    switch (this->numer)
    {
    case 0:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 });

        break;
    case 1:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, 3, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, 3, 4, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, 3, 4, 5, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, 3, 4, 5, 6, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, 3, 4, 5, 6, 7, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, 3, 4, 5, 6, 7, 0, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4 });
        break;

    case 2:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 0, 0 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, 5, 5, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 2, 2, 2 });
    
        break;

    case 3:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 1, 2, 3, 8, 5, _, 7, 0, 1, 2, 3, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 2, 3, 8, 5, 6, _, 0, 1, 2, 3, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 3, 8, 5, 6, 7, _, 1, 2, 3, 8, 5, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, 5, 6, 7, 0, _, 2, 3, 8, 5, 6, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 5, 6, 7, 0, 1, _, 3, 8, 5, 6, 7, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 6, 7, 0, 1, 2, _, 8, 5, 6, 7, 0, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 7, 0, 1, 2, 3, _, 5, 6, 7, 0, 1, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 0, 1, 2, 3, 8, _, 6, 7, 0, 1, 2, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 1, 2, 3, 8, 5, _, 7, 0, 1, 2, 3, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 2, 3, 8, 5, 6, _, 0, 1, 2, 3, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 3, 8, 5, 6, 7, _, 1, 2, 3, 8, 5, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, 5, 6, 7, 0, _, 2, 3, 8, 5, 6, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 5, 6, 7, 0, 1, _, 3, 8, 5, 6, 7, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 6, 7, 0, 1, 2, _, 8, 5, 6, 7, 0, _ });


        break;

    case 4:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 7, _, _, _, _, _, 7, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 7, _, _, _, _, _, 7, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 7, _, _, _, 7, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 7, _, _, _, 7, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 8, 8, 8, 8, 8, 8, 8, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 8, 8, 8, 8, 8, 8, 8, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 8, 8, 4, 8, 8, 8, 4, 8, 8, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 8, 8, 4, 8, 8, 8, 4, 8, 8, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, 8, 8, 8, 8, 8, 8, 8, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, 8, _, _, _, _, _, 8, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, 8, _, _, _, _, _, 8, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 8, 8, _, 8, 8, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 8, 8, _, 8, 8, _, _, _, _ });
        break;

    case 5:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 4, _, 3, _, 2, _, 3, _, 4, _, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 4, _, 3, _, 2, _, 3, _, 4, _, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 4, _, 3, _, 2, _, 3, _, 4, _, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 4, _, 3, _, 2, _, 3, _, 4, _, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 4, _, 3, _, 2, _, 3, _, 4, _, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 9, 1, 9, 1, 9, 1, 9, 1, 9, _, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 4, _, 3, _, 2, _, 3, _, 4, _, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 4, _, 3, _, 2, _, 3, _, 4, _, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 4, _, 3, _, 2, _, 3, _, 4, _, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 4, _, 3, _, 2, _, 3, _, 4, _, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, _, 1, _, 9, _, 1, _, 9, _, 1, _, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, _, 4, _, 3, _, 2, _, 3, _, 4, _, 5 });
        break;

    case 6:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, 7, 7, 6, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 7, 7, 6, 6, 5, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 7, 7, 6, 6, 5, 5, 4, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 7, 6, 6, 5, 5, 4, 4, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 7, 6, 6, 5, 5, 4, 4, 3, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 6, 6, 5, 5, 4, 4, 3, 3, 2, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 6, 5, 5, 4, 4, 3, 3, 2, 2, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 5, 5, 4, 4, 3, 3, 2, 2, 1, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 5, 4, 4, 3, 3, 2, 2, 1, 1, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 4, 4, 3, 3, 2, 2, 1, 1, 0, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 3, 3, 2, 2, 1, 1, 0, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 3, 2, 2, 1, 1, 0, 0, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 2, 1, 1, 0, 0, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, 1, 0, 0, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });

        break;

    case 7:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 9, _, 9, _, 9, _, 9, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, _, _, _, _, 9, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, 9, _, 9, _, _, _, 9, _, 9, 9, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, 0, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, 9, 1, 9, _, _, _, 9, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 9, _, _, 2, _, _, 9, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, 3, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 9, _, _, 4, _, _, 9, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, 9, 5, 9, _, _, _, 9, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, 6, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, 9, _, 9, _, _, _, 9, _, 9, 9, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, _, _, _, _, 9, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 9, _, 9, _, 9, _, 9, _, _, _ });

        break;

    case 8:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, 9, _, _, _, _, _, 9, _, 9, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, 3, 9, _, _, _, _, _, 9, 3, 9, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, 2, 9, _, _, _, _, _, 9, 2, 9, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, 9, 9, _, _, _, _, _, 9, 9, 9, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 6, 0, 0, 0, 7, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 6, 1, 1, 1, 7, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 6, 2, 2, 2, 7, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 6, 3, 3, 3, 7, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 6, 4, 4, 4, 7, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 6, 5, 5, 5, 7, _, _, _, _ });

        break;

    case 9:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, 5, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, 5, 2, 5, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, 5, 2, 0, 2, 5, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, 5, 2, 0, 2, 0, 2, 5, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, 5, 2, 0, 2, 8, 2, 0, 2, 5, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, 5, 2, 0, 2, 0, 2, 5, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, 5, 2, 0, 2, 5, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, 5, 2, 5, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, 5, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 });

        break;

    case 10:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, _, _, _, _, _, _, _, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, 8, 8, 8, 8, 8, 8, 8, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, 8, _, _, _, _, _, 8, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, 8, _, 8, 8, 8, _, 8, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, 8, _, 8, _, 8, _, 8, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, 8, _, 8, 8, 8, _, 8, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, 8, _, _, _, _, _, 8, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, 8, 8, 8, 8, 8, 8, 8, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, _, _, _, _, _, _, _, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, _ });

        break;

    case 11:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 9, _, _, _, _, _, 9, 6, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, 0, _, 9, _, _, _, _, _, 9, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, 9, _, _, 9, _, _, 9, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, 9, 3, _, 9, _, _, 9, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, 9, _, _, 9, _, _, 9, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, 1, 9, _, _, 9, _, 5, 9, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, 9, _, _, 9, _, _, 9, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, 9, _, _, 9, _, _, 9, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, 9, _, 4, 9, _, _, 9, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, 9, _, _, 9, _, _, 9, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, 2, _, _, _, _, 9, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, _, _, _, _, _, 9, _, _, _, _, 7 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9 });

        break;

    case 12:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 7, 7, 7, _, 0, 0, 0, _, 7, 7, 7, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 6, 6, 6, _, 1, 1, 1, _, 6, 6, 6, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 5, 5, 5, _, 2, 2, 2, _, 5, 5, 5, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 4, 4, 4, _, 3, 3, 3, _, 4, 4, 4, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 3, 3, 3, _, 4, 4, 4, _, 3, 3, 3, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 2, 2, 2, _, 5, 5, 5, _, 2, 2, 2, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 1, 1, 1, _, 6, 6, 6, _, 1, 1, 1, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 0, 0, 0, _, 7, 7, 7, _, 0, 0, 0, _ });

        break;

    case 13:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 9, _, _, _, _, _, _, _, _, _, _, _, 9 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 9, _, _, _, _, _, _, _, _, _, _, _, 9 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 9, _, _, _, _, _, _, _, _, _, _, _, 9 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 9, _, _, _, _, _, _, _, _, _, _, _, 9 });

        break;

    case 14:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 0, 9, 2, 2, 2, 2, 2, 2, 2, 9, 0, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 0, 7, 9, 2, 2, 2, 2, 2, 9, 3, 0, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 0, 7, 7, 9, 2, 2, 2, 9, 3, 3, 0, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 0, 7, 7, 7, 9, 0, 9, 3, 3, 3, 0, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 0, 7, 7, 7, 7, 0, 3, 3, 3, 3, 0, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 0, 7, 7, 7, 7, 0, 3, 3, 3, 3, 0, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 0, 7, 7, 7, 7, 0, 3, 3, 3, 3, 0, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 8, 7, 7, 7, 7, 0, 3, 3, 3, 3, 8, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 2, 8, 7, 7, 7, 0, 3, 3, 3, 8, 2, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 2, 2, 8, 7, 7, 0, 3, 3, 8, 2, 2, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 2, 2, 2, 8, 7, 0, 3, 8, 2, 2, 2, 2 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 2, 2, 2, 2, 2, 8, 0, 8, 2, 2, 2, 2, 2 });

        break;

    case 15:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, 9, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 0, 0, _, 0, 0, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 0, 0, _, _, 9, _, _, 0, 0, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 0, 0, _, _, 1, 1, _, 1, 1, _, _, 0, 0 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 1, 1, _, _, 9, _, _, 1, 1, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, 1, _, _, 7, 7, _, 7, 7, _, _, 1, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 7, 7, _, _, 9, _, _, 7, 7, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 7, 7, _, _, 3, 3, _, 3, 3, _, _, 7, 7 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 3, 3, _, _, 9, _, _, 3, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 3, 3, _, _, 4, 4, _, 4, 4, _, _, 3, 3 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 4, 4, _, _, 9, _, _, 4, 4, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 4, 4, _, _, 5, 5, _, 5, 5, _, _, 4, 4 });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 5, 5, _, _, _, _, _, 5, 5, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 5, 5, _, _, _, _, _, _, _, _, _, 5, 5 });

        break;

    case 16:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, 8, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, 5, 5, 5, 8, 3, 3, 3, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 5, 5, 5, 0, 0, 0, 5, 5, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 5, 5, 0, 0, 0, 0, 0, 5, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 3, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 3, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 5, 5, 5, 0, 0, 0, 0, 0, 5, 5, 3, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, 8, _, _, 8, _, 8, _, 8, _, _, 8, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, 8, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, 8, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, 8, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 9, _, 9, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, 9, 9, 9, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, 9, _, _, _, _, _, _, _ });

        break;

    case 17:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, _, 9, 7, 7, 7, 7, 7, 7, 7, 9, _, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, _, 9, 9, 7, 7, 7, 7, 7, 9, 9, _, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, _, 9, _, 9, 7, 7, 7, 9, _, 9, _, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, _, 9, _, 6, 9, 7, 9, 2, _, 9, _, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, _, 9, _, 6, _, 8, _, 2, _, 9, _, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, _, 9, _, 6, _, 3, _, 2, _, 9, _, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, _, 9, _, 6, _, 3, _, 2, _, 9, _, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, _, 9, _, 6, _, 3, _, 2, _, 9, _, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, _, 9, _, 6, _, 3, _, 2, _, 9, _, 1 });
        ukladPoziomu.insert(ukladPoziomu.end(), { 1, 9, 9, 9, 6, _, 3, _, 2, 9, 9, 9, 1 });

        break;

    case 18:
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, _, _, _, _, _, _, _, _, _, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 9, 9, 9, 9, 9, 9, 9, 9, 9, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 3, 4, 5, 6, 9, 6, 5, 4, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 3, 4, 5, 6, 9, 6, 5, 4, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 3, 4, 5, 6, 9, 6, 5, 4, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 3, 4, 5, 6, 7, 6, 5, 4, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 3, 4, 5, 6, 9, 6, 5, 4, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 3, 4, 5, 6, 9, 6, 5, 4, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 3, 4, 5, 6, 9, 6, 5, 4, 3, _, _ });
        ukladPoziomu.insert(ukladPoziomu.end(), { _, _, 9, 9, 9, 9, 9, 9, 9, 9, 9, _, _ });


        break;
    }

    return ukladPoziomu;
}