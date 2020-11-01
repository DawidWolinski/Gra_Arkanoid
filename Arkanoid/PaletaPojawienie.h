#pragma once
#include "Animacja.h"


class PaletaPojawienie : public Animacja
{
public:

	void animacja(Paleta& paleta, const float& dt) override;

private:

	float czasAnimacja;

};

