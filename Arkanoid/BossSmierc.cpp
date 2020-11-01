#include "BossSmierc.h"
#include <iostream>
#include "Boss.h"
#include "Paleta.h"

BossSmierc::BossSmierc()
	: czasAnimacja(0.f), i(0)
{

}

void BossSmierc::animacja(Boss& boss, Paleta& paleta, const float& dt)
{
	if (boss.czyRysowac)
	{
		this->czasAnimacja += 10.f * dt;

		if (this->i == 0)
		{
			if (boss.getTextureRect().left > 0)
				this->i = 1;
			else
				this->i = 2;
		}

		else if (this->i == 1 && this->czasAnimacja > 1.f)
		{
			this->czasAnimacja = 0.f;

			if (boss.getTextureRect().left == 0)
				this->i++;
			else
				boss.setTextureRect(sf::IntRect(boss.getTextureRect().left - 64, boss.getTextureRect().top, 62, 96));		
		}

		else if (this->i == 2 && this->czasAnimacja > 15.f)
		{
			this->czasAnimacja = 0.f;
			boss.setTextureRect(sf::IntRect(0, 624, 62, 96));
			this->i++;
		}

		else if (this->i == 3 && this->czasAnimacja > 1.5f)
		{
			this->czasAnimacja = 0.f;

			if (boss.getTextureRect().left == 0 && boss.getTextureRect().top == 816)
				this->i++;

			if (boss.getTextureRect().left == 192)
				boss.setTextureRect(sf::IntRect(0, boss.getTextureRect().top + 96, 62, 96));
			else
				boss.setTextureRect(sf::IntRect(boss.getTextureRect().left + 64, boss.getTextureRect().top, 62, 96));	
		}

		else if (this->i == 4 && this->czasAnimacja > 0.75f)
		{
			this->czasAnimacja = 0.f;

			boss.setColor(sf::Color(boss.getColor().r, boss.getColor().g, boss.getColor().b, boss.getColor().a - 25));

			if (boss.getColor().a < 25)
			{
				boss.czyRysowac = false;
				this->i = 5;
			}
			
			boss.czyOutro = true;

		}

	}
	


}
