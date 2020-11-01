#pragma once
#include "Silnik.h"

class Boss;
class Paleta;

class BossAnimacja
{
public:

	virtual void animacja(Boss& boss, Paleta& paleta, const float& dt) = 0;
};
