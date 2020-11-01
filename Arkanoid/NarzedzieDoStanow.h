#pragma once

#include <memory>
#include <stack>
#include "Stan.h"

typedef std::unique_ptr<Stan> StanWskaznik;

class NarzedzieDoStanow
{
public:
	NarzedzieDoStanow();

	void dodajStan(StanWskaznik nowyStan, bool czyPodmieniac = true);
	void usunStan();
	void zarzadzajZmianaStanu();
	void usunWszystkieStany();

	StanWskaznik& zwrocObecnyStan();

private:
	std::stack<StanWskaznik> stany;
	StanWskaznik nowyStan;

	bool czyUsuwa;
	bool czyDodaje;
	bool czyPodmienia;

};