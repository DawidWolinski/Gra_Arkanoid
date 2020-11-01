#pragma once
#include "Animacja.h"

#include <stack>
#include <vector>

class PaletaTransformacja : public Animacja
{
public:

	PaletaTransformacja();
	void animacja(Paleta& paleta, const float& dt) override;
	void resetuj();

private:

	float czasAnimacja;
	std::stack<sf::IntRect> klatki;
	std::vector<std::stack<sf::IntRect>> klatkiTransformacja;
	bool wyczyszczone;

	void wytnijTekstury();
};

