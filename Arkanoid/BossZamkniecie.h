#pragma once
#include "BossAnimacja.h"


class BossZamkniecie : public BossAnimacja
{
public:
	BossZamkniecie();
	void animacja(Boss& boss, Paleta& paleta, const float& dt) override;

private:
	float czasAnimacja;
};
