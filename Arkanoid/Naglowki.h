#pragma once

enum class Kierunek { Prawa, Lewa, Dol, Gora, Zaden };

#define SZEROKOSC 224.f
#define WYSOKOSC 256.f

#define LIMIT_FPS 120

//Sciezki

//Pliki
#define PLIK_DANE "Dane.json"

//Tekstury
#define PLIK_TEKSTURA_TLO "Zasoby/Tekstury/Backgrounds.png"
#define PLIK_TEKSTURA_KLOCEK "Zasoby/Tekstury/Bricks.png"
#define PLIK_TEKSTURA_PALETA "Zasoby/Tekstury/Palettes.png"
#define PLIK_TEKSTURA_BONUS "Zasoby/Tekstury/Powerups.png"
#define PLIK_TEKSTURA_BOSS "Zasoby/Tekstury/Boss.png"
#define PLIK_TEKSTURA_POCISK "Zasoby/Tekstury/Projectiles.png"
#define PLIK_TEKSTURA_COPYRIGHT "Zasoby/Tekstury/Copyright.png"
#define PLIK_TEKSTURA_NIEBO "Zasoby/Tekstury/Sky.png"
#define PLIK_TEKSTURA_STATEK_ARKANOID "Zasoby/Tekstury/Arkanoid_Ship.png"
#define PLIK_TEKSTURA_STATEK "Zasoby/Tekstury/Ship.png"

//Czcionki
#define PLIK_CZCIONKA "Zasoby/Czcionki/ARCADE_N.ttf"
#define PLIK_CZCIONKA_TAITO "Zasoby/Czcionki/RedRose-Bold.ttf"

//Dzwieki
#define PLIK_DZWIEK_WYDLUZENIE "Zasoby/Dzwieki/Arkanoid_SFX_4.wav"
#define PLIK_DZWIEK_ODBICIE_BOSS "Zasoby/Dzwieki/Arkanoid_SFX_5.wav"
#define PLIK_DZWIEK_ODBICIE_PALETA "Zasoby/Dzwieki/Arkanoid_SFX_6.wav"
#define PLIK_DZWIEK_ODBICIE_KLOCEK "Zasoby/Dzwieki/Arkanoid_SFX_7.wav"
#define PLIK_DZWIEK_ODBICIE_KLOCEK_ZLOTY "Zasoby/Dzwieki/Arkanoid_SFX_8.wav"
#define PLIK_DZWIEK_SMIERC_PALETA "Zasoby/Dzwieki/Arkanoid_SFX_10.wav"
#define PLIK_DZWIEK_SMIERC_BOSS "Zasoby/Dzwieki/Arkanoid_SFX_12.wav"

#define PLIK_DZWIEK_INTRO "Zasoby/Dzwieki/01_-_Arkanoid_-_ARC_-_Start_Demo.ogg"
#define PLIK_DZWIEK_START "Zasoby/Dzwieki/02_-_Arkanoid_-_ARC_-_Game_Start.ogg"
#define PLIK_DZWIEK_ZYCIE "Zasoby/Dzwieki/03_-_Arkanoid_-_ARC_-_Extend.ogg"
#define PLIK_DZWIEK_START_BOSS "Zasoby/Dzwieki/04_-_Arkanoid_-_ARC_-_Dimensional_Fortress_Doh.ogg"
#define PLIK_DZWIEK_KONIEC_GRY "Zasoby/Dzwieki/05_-_Arkanoid_-_ARC_-_Game_Over.ogg"
#define PLIK_DZWIEK_OUTRO "Zasoby/Dzwieki/06_-_Arkanoid_-_ARC_-_Name_Entry.ogg"
#define PLIK_DZWIEK_LASER "Zasoby/Dzwieki/Arkanoid_2_Revenge_of_Doh_SFX_(1).ogg"