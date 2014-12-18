/*************************************************************************************
Projekt IMS, 2014 - Rok včelstva
Autoři: Michal Veselý (xvesel63), Lukáš Vokráčko (xvokra00)

 Soubor bee.h - hlavičkový soubor obsahující deklaraci třídy Bee (potomek Process ze simlib.h)
  - obsahuje konstanty pojící se se včelím životem (doba jednotlivých životních období,
  	  enum různých životních stádií včely)
  - metody chování včely v jednotlivých životních stádiích
  - odvozování počtu letu včely za potravou v závislosti na radiusu potravy
  - metoda určující pravděpodobnost umrznutí včely
*************************************************************************************/

class Bee;

#pragma once

#include <simlib.h>
#include "params.h"
#include <iostream>
#include <ctime>

extern Params params;


// třída Bee - reprezentuje jednu včelu včelstva:
// 		obsahuje stav životního stádia a metody popisující chování v těchto stádiích
// 		metodu určující pravděpodobnost umrznutí
//		metodu určující počet letů včely podle radiusu potravy
class Bee: public Process
{
public:
	int Priority = 1;

	// doba životních stádií včely
	static const int TIME_EGG;
	static const int TIME_MAGGOT;
	static const int TIME_FOODCHANGED;
	static const int TIME_CAPPER;
	static const int TIME_CLEANER;
	static const int TIME_FEEDER;
	static const int TIME_BUILDER;
	static const int TIME_GUARD;

	// životní stádia včely
	enum lifeStages
	{
		EGG, // vajíčko
		MAGGOT, // larva
		FOODCHANGED, // změna jídla
		CAPPER, // zavíčkovaná
		CLEANER, // čistička
		FEEDER, // krmička
		BUILDER, // stavitelka
		GUARD, // hlídačka
		FORAGER, // létavka
		DEAD, // mrtvá včela
		LONGLIVING // dlouhožijící včela
	};

	int state; // aktuální stádium vývoje
	int dayCount; // počet dní do dalšího stádia

	Bee(int state, int dayCount); // inicializator - stav a počet dní do dalšího
	void Behavior();

private:
	// chování v jednotlivých vývojových stádiích
	void egg();
	void maggot();
	void foodchanged();
	void capper();
	void cleaner();
	void feeder();
	void builder();
	void guard();
	void forager();
	void dead();
	void longliving();


	void set(int state, int dayCount); // nastavení přechodu do dalšího stádia
	void setWinterOver();

	double froze(); // pravděpodobnost na umrznutí (závisí na denní teplotě)
	int flightCount(); // počet letů pro potravu (jen u létavek)
};
