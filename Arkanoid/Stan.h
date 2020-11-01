#pragma once

class Stan
{
public:
	virtual ~Stan() {};

	virtual void inicjalizuj() = 0;

	virtual void przechwytujWydarzenia(const float& dt) = 0;
	virtual void aktualizuj(const float& dt) = 0;
	virtual void rysuj(const float& dt) = 0;

	virtual void pauza() {};
	virtual void kontynuuj() {}
};
