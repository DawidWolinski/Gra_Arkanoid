#include "BossSpoczynek.h"
#include "Boss.h"
#include "Paleta.h"

BossSpoczynek::BossSpoczynek()
	: czasAnimacja(0.f)
{

}

void BossSpoczynek::animacja(Boss& boss, Paleta& paleta, const float& dt)
{
	this->czasAnimacja += 10.f * dt;
		
	if (this->czasAnimacja > 30.f && paleta.czyWRuchu)
	{
		this->czasAnimacja = 0.f;

		boss.stan = &boss.otwarcie;
	}


}
