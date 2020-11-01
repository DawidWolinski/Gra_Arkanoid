#pragma once
#include "BossAnimacja.h"

class BossSpoczynek : public BossAnimacja
{
public:
	BossSpoczynek();
	void animacja(Boss& boss, Paleta& paleta, const float& dt) override;

private:
	float czasAnimacja;
};