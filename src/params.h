/*************************************************************************************
Projekt IMS, 2014 - Rok včelstva
Autoři: Michal Veselý (xvesel63), Lukáš Vokráčko (xvokra00)

 Soubor params.cc - třída Params
  - zpracovává parametry command line
  - poskytuje některé globální informace ostatním třídám
*************************************************************************************/

class Params;

#pragma once

#include <vector>
#include <utility>
#include <stdexcept>
#include <fstream>
#include <string>
#include <cstring>
#include <iostream>
#include <simlib.h>
#include <tuple>
#include "day.h"
#include "bee.h"



// třída Params - zpracovává parametry command line a předává některé globální informace
class Params
{
public:
	using Month = std::pair<double, double>; // 2 hodnoty pro každý měsíc (střední hodnota, odchylka)
	std::vector<Params::Month> months; // vektor měsíců
	std::vector<std::tuple<int, int, int, int>> radius; // vektor pro informace o radiusu potravy
	std::vector<std::pair<int, double>> harvestRadius; // vektor, kterým zadává uživatel, jak se má radius vygenerovat

	// březen - říjen, maximální hodnoty úrody typické pro čr
	// odvozeno z tabulek pylových sezón a podobně
	// leden, únor, listopad, prosinec neuvažován - včely nejsou aktivní
	static const int MARCH_AVERAGE_HARVEST;
	static const int APRIL_AVERAGE_HARVEST;
	static const int MAY_AVERAGE_HARVEST;
	static const int JUNE_AVERAGE_HARVEST;
	static const int JULY_AVERAGE_HARVEST;
	static const int AUGUST_AVERAGE_HARVEST;
	static const int SEPTEMBER_AVERAGE_HARVEST;
	static const int OCTOBER_AVERAGE_HARVEST;

	// opět jen březen - říjen, průměrný rozsah potravy
	// odpovídá vzdálenosti, kam musí včely začít lítat, když vyopylují nejbližší zdroj potravy
	// odvozen z pylových sezón
	static const int MARCH_HARVEST_RANGE;
	static const int APRIL_HARVEST_RANGE;
	static const int MAY_HARVEST_RANGE;
	static const int JUNE_HARVEST_RANGE;
	static const int JULY_HARVEST_RANGE;
	static const int AUGUST_HARVEST_RANGE;
	static const int SEPTEMBER_HARVEST_RANGE;
	static const int OCTOBER_HARVEST_RANGE;

	int beeCount = 0; // počet všech včel
	int longlivingCount = 0; // počet dlouhověkých včel
	int foragerCount = 0; // počet létavek
	int startLongliving; // indikátor začátku dlohověkých včel
	double honey = 0; // množství medu v úlu 1.1. XXXX (med, který včely potřebují na dozimování)
	double honeyTotal = 0; // množství medu v úlu 1.1. XXXX (med, který včely potřebují na dozimování)
	double seasonKoeficient[12] = {0, 0, 0.2, 0.5, 1, 1, 1, 0.85, 0.5, 0.2, 0, 0}; // kolik létavek létá (v procentech)
	int month;
	int water;
	bool winter = true;
	int roundIndex = 0;
	int roundBuffer[3] = {0};
	double tmp = 0;
	double temperature = 0;


	std::ofstream beeCountLog;
	std::ofstream honeyLog;
	std::ofstream weatherLog;

private:
	void loadInput(std::string filename);
public:
	bool set(int argc,char * argv[]);
	void log(int dayNumber);
	void help();
};
