#include "BossZamkniecie.h"
#include "Boss.h"
#include "Paleta.h"

BossZamkniecie::BossZamkniecie()
	: czasAnimacja(0.f)
{

}

void BossZamkniecie::animacja(Boss& boss, Paleta& paleta, const float& dt)
{
	this->czasAnimacja += 10.f * dt;

	if (this->czasAnimacja > 1.f)
	{
		this->czasAnimacja = 0.f;

		if (boss.getTextureRect().left == 0)
		{
			boss.stan = &boss.spoczynek;
			boss.strzelanie.resetuj();
		}
			
		else
			boss.setTextureRect(sf::IntRect(boss.getTextureRect().left - 64, boss.getTextureRect().top, 62, 96));

		
	}
}
