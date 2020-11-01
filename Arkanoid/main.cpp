#include "Silnik.h"
#include "Naglowki.h"

using namespace std;

int main()
{
    int skala = 3;
    if (sf::VideoMode::getDesktopMode().height < 864)
        skala = 2;

    Silnik silnik(skala * static_cast<int>(SZEROKOSC), skala * static_cast<int>(WYSOKOSC), "Arkanoid");

    return 0;
}