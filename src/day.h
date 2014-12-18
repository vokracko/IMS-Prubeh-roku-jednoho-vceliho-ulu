/*************************************************************************************
Projekt IMS, 2014 - Rok včelstva
Autoři: Michal Veselý (xvesel63), Lukáš Vokráčko (xvokra00)

 Soubor day.h - Hlavičkový soubor obsahující deklaraci třídy Day (potomek Event)
  - řídí chování simulačního modelu v jednotlivých měsících roku
  - stará se o počet nakladených vajíček v jednotlivých dnech
  - generuje množství potřebné vody pro úl
*************************************************************************************/

class Day;

#pragma once
#include <simlib.h>
#include "params.h"
#include "bee.h"
#include <iostream>
#include <vector>

extern Params params;


// Třída Day (potomek Event ze simlib.h)
// stará se o chování sim. modelu v jednotlivých dnech (podle aktuálního měsíce a počasí)
class Day: public Event
{
	int dayNumber;
	double reproduceKoeficient = 1;

	// enum možných stavů kladení vajíček (vliv počasí ve větších časových intervalech)
	enum eggCountOptions
	{
		INCREMENT10,
		INCREMENT20,
		DECREMENT5,
		DECREMENT10,
		DEFAULT
	};

	int reproduceState = DEFAULT; // aktuální stav počtu kladení vajíček

	// počet vajíček nakladených v jednotlivých měsících, získáno z různých statistik
	int eggCount[12] = {120, 323, 472, 1015, 2030, 1021, 442, 362, 231, 102, 57, 23};


public:
	static const int SWING;

	Day(int dayNum);
	~Day();

	void Behavior();

private:
	void january();
	void february();
	void march();
	void april();
	void may();
	void june();
	void july();
	void august();
	void september();
	void october();
	void november();
	void december();

	int temperature(int month);
	void saveTemperature(int res);
	void reproduce(int month);
	void setKoeficient();
	void setWaterNeeds(int temp);
	int eggLayed();

};
