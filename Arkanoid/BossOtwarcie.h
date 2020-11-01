#pragma once
#include "BossAnimacja.h"

class BossOtwarcie : public BossAnimacja
{
public:
	BossOtwarcie();
	void animacja(Boss& boss, Paleta& paleta, const float& dt) override;

private:
	float czasAnimacja;
};

