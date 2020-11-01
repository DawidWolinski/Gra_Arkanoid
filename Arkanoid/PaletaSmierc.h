#pragma once
#include "Animacja.h"

class PaletaSmierc : public Animacja
{
public:

	PaletaSmierc();
	void animacja(Paleta& paleta, const float& dt) override;

private:


	float czasAnimacja;
	std::vector<sf::IntRect> klatki;
	unsigned int i;

	void wytnijTekstury();
};

