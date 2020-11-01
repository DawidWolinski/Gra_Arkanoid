#pragma once
#include "Silnik.h"

class EkranKoncowy : public Stan
{
public:

	EkranKoncowy(sf::Texture tekstura, int wynik, ElementySilnikaWskaznik elementy);

	virtual void inicjalizuj() override;

	virtual void przechwytujWydarzenia(const float& dt) override;
	virtual void aktualizuj(const float& dt) override;
	virtual void rysuj(const float& dt) override;

private:
	ElementySilnikaWskaznik element;

	void ustawZaslone();
	void ustawNapis();

	float czasAnimacja;
	int i;
	int wynik;
	sf::Vector2f szybkosc;

	sf::Sprite zdjecie;
	sf::Texture tekstura;
	sf::RectangleShape zaslona;

};