#include "NarzedzieDoStanow.h"

NarzedzieDoStanow::NarzedzieDoStanow()
:czyDodaje(false), czyUsuwa(false), czyPodmienia(false)
{}

void NarzedzieDoStanow::dodajStan(StanWskaznik nowyStan, bool czyPodmieniac)
{
	this->czyDodaje = true;
	this->czyPodmienia = czyPodmieniac;

	this->nowyStan = std::move(nowyStan);
}

void NarzedzieDoStanow::usunStan()
{
	this->czyUsuwa = true;
}

void NarzedzieDoStanow::zarzadzajZmianaStanu()
{
	if (this->czyUsuwa && !this->stany.empty())
	{
		this->stany.pop();

		if (!this->stany.empty())
		{
			this->stany.top()->kontynuuj();
		}
	}

	this->czyUsuwa = false;

	if (this->czyDodaje)
	{
		if (!this->stany.empty())
		{
			if (this->czyPodmienia)
			{
				this->stany.pop();
			}
			else
			{
				this->stany.top()->pauza();
			}
		}

		this->stany.push(std::move(this->nowyStan));
		this->stany.top()->inicjalizuj();
		this->czyDodaje = false;
	}
}

StanWskaznik& NarzedzieDoStanow::zwrocObecnyStan()
{
	return this->stany.top();
}

void NarzedzieDoStanow::usunWszystkieStany()
{
	while (!this->stany.empty())
		this->stany.pop();
}