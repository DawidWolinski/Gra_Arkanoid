#pragma once
#include "Silnik.h"


class Paleta;

class Animacja
{
public:
	virtual void animacja(Paleta& paleta, const float& dt) = 0;
};

