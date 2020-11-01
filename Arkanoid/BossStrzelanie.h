#pragma once
#include "BossAnimacja.h"

class BossStrzelanie : public BossAnimacja
{
public:

	BossStrzelanie();
	void animacja(Boss& boss, Paleta& paleta, const float& dt) override;
	void resetuj();

private:

	sf::Vector2f zwrocKierunek(sf::Vector2f pociskPozycja, Paleta& paleta);

	float czasAnimacja;
	float czasStrzal;
	sf::RectangleShape pocisk;
	
};

