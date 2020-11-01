#include "BossOtwarcie.h"
#include "Boss.h"
#include "Paleta.h"


BossOtwarcie::BossOtwarcie()
	: czasAnimacja(0.f)
{

}

void BossOtwarcie::animacja(Boss& boss, Paleta& paleta, const float& dt)
{
	this->czasAnimacja += 10.f * dt;

	if (!paleta.czyWRuchu)
		boss.stan = &boss.zamkniecie;

	else if (this->czasAnimacja > 1.f)
	{
		this->czasAnimacja = 0.f;

		boss.setTextureRect(sf::IntRect(boss.getTextureRect().left + 64, boss.getTextureRect().top, 62, 96));

		if (boss.getTextureRect().left == 192) 
			boss.stan = &boss.strzelanie; 
	}
	
}