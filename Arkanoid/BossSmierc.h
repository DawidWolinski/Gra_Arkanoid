#pragma once
#include "BossAnimacja.h"

class BossSmierc : public BossAnimacja
{
public:

	BossSmierc();
	void animacja(Boss& boss, Paleta& paleta, const float& dt) override;

private:

	float czasAnimacja;
	int i;

};

