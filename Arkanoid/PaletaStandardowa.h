#pragma once
#include "Animacja.h"


class PaletaStandardowa : public Animacja
{
public:

	PaletaStandardowa();
	void animacja(Paleta& paleta, const float& dt) override;

private:

	float czasAnimacja;
	int szerokoscPalety;

};

