/*************************************************************************************
Projekt IMS, 2014 - Rok včelstva
Autoři: Michal Veselý (xvesel63), Lukáš Vokráčko (xvokra00)

 Soubor day.cc - Implementace metod třídy Day
  - chování v jednotlivých měsících roku
  - vlivy počasí v konkrétních dnech a posloupnostech dní
*************************************************************************************/

#include "day.h"

const int Day::SWING = 15; // max. rozsah teplot od střední hodnoty


// Konstruktor nového dne
Day::Day(int dayNum) : Event()
{
	// params.tmp = 0;
	this->dayNumber = dayNum; // poznačení aktuálního dne

	if(dayNum == 0) // pokud se jedná o první den (1.1. XXXX), jsou vytvořeny dlouhodobé včely, které doposud žily v úlu
	{
		// vytvoření dlouhodobých včel - předána informace o typu včely a době života
		for(int i = 0; i < params.beeCount; ++i)
		{
			(new Bee(Bee::LONGLIVING, Uniform(60, 120)))->Activate();
		}
	}

}

Day::~Day()
{
}

// Chování dne - podle aktuální hodnoty dne se rozpozná příslušný měsíc
// zavolání metody daného měsíce
void Day::Behavior()
{

	if (this->dayNumber==367) return;
	else if (this->dayNumber > 334) this->december();
	else if (this->dayNumber > 304) this->november();
	else if (this->dayNumber > 273) this->october();
	else if (this->dayNumber > 243) this->september();
	else if (this->dayNumber > 212) this->august();
	else if (this->dayNumber > 181) this->july();
	else if (this->dayNumber > 151) this->june();
	else if (this->dayNumber > 120) this->may();
	else if (this->dayNumber > 90) this->april();
	else if (this->dayNumber > 59) this->march();
	else if (this->dayNumber > 31) this->february();
	else this->january();

	params.log(this->dayNumber); // generování hodnot pro tabulky a grafy


	(new Day(this->dayNumber+1))->Activate(Time + 1); // naplánování dalšího dne
}

// chování v jednotlivých měsících

void Day::january()
{
	int temp = this->temperature(0);
	this->reproduce(0);
}

void Day::february()
{
	int temp = this->temperature(1);
	this->reproduce(1);
}

void Day::march()
{
	int temp = this->temperature(2);
	this->reproduce(2);
}

void Day::april()
{
	int temp = this->temperature(3);
	this->reproduce(3);
}

void Day::may()
{
	int temp = this->temperature(4);
	this->reproduce(4);
}

void Day::june()
{
	int temp = this->temperature(5);
	this->reproduce(5);
}

void Day::july()
{
	int temp = this->temperature(6);
	params.winter = false;
	this->reproduce(6);
}

void Day::august()
{
	int temp = this->temperature(7);
	this->reproduce(7);
}

void Day::september()
{
	int temp = this->temperature(8);
	this->reproduce(8);
}

void Day::october()
{
	int temp = this->temperature(9);
	this->reproduce(9);
}

void Day::november()
{
	int temp = this->temperature(10);
	this->reproduce(10);
}

void Day::december()
{
	int temp = this->temperature(11);
	this->reproduce(11);
}

// generovani poctu nakladenych vajicek tento den
int Day::eggLayed()
{
	int eggDiff;

	// vypocitani interpolace kladeni vajicek
	if (params.month == 11) eggDiff = (this->eggCount[0] - this->eggCount[params.month])/30*(this->dayNumber%30);
	else eggDiff = (this->eggCount[params.month+1] - this->eggCount[params.month])/30*(this->dayNumber%30);

	// počet včel klesl pod kritickou hranici, matka umírá, včely dožívají
	if(params.beeCount < 2000)
	{
		return 0;
	}

	// vraceni poctu vajicek (nahoda - podle stavu matky, dopocet mezi mesici, koeficient reprodukce - ovliveni pocasim)
	return Uniform(0.9, 1.1) * this->eggCount[params.month] * this->reproduceKoeficient + eggDiff;
}


// reprodukce - na základě aktuálního měsíce vypočítá, kolik se má naklást vajíček
void Day::reproduce(int month)
{
	this->setKoeficient();

	int eggCount = eggLayed();// pocet nakladenych vajicek

	// vytvoření vajíček
	for(int i = 0; i < eggCount; i++)
	{
		// podmínka, jestli se rodí dlouhodobé nebo krátkodobé včely
		if(this->dayNumber >= params.startLongliving)
		{
			(new Bee(Bee::LONGLIVING, ~0))->Activate(); // vytvoření dlouhodobé včely
		}
		else
		{
			(new Bee(Bee::EGG, Bee::TIME_EGG))->Activate(); // vytvoření vajíčka
		}

	}
}

// vypočítání teploty aktuálního dne
// podle stredni hodnoty a odchylky konkretniho mesice normalnim rozlozenim pravdepodobnosti
// pouziva swing - generovani v rozmezi 15 stupnu
int Day::temperature(int month)
{
	params.month = month;
	Params::Month tmp = params.months.at(month);
	int res = Normal(tmp.first, tmp.second);

	if(res > tmp.first + Day::SWING) this->temperature(month); // kontrola vyse rozsahu
	if(res < tmp.first - Day::SWING) this->temperature(month); // kontrola vyse rozsahu

	this->saveTemperature(res); // nastavi teplotu
	this->setWaterNeeds(res); // nastavi pocet potrebne vody

	return res;
}

// uložení teploty dne do kruhového bufferu
void Day::saveTemperature(int res)
{
	params.temperature = res;
	params.roundBuffer[params.roundIndex++ % 3] = res;
}


// funkce sleduje 3 po sobě jdoucí dny, a pokud jsou všechny teplé, nebo studené,
// tak jistým způsobem zvýší nebo sníží počet nakladených vajíček
// nastavuje pouze procentuelní koeficient množství nakladených vajíček - to záleží totiž i na měsíci
void Day::setKoeficient()
{
	// 3 teplé dny - zvyš o 20 procent počet vajíček
	if(this->reproduceState != Day::INCREMENT20 && params.roundBuffer[0] > 15 && params.roundBuffer[1] > 15 && params.roundBuffer[2] > 15)
	{
		this->reproduceState = Day::INCREMENT20;
		this->reproduceKoeficient = 1.2;
	}
	// 2 teplé dny - zvyš o 10 procent počet vajíček
	else if(this->reproduceState != Day::INCREMENT10 && params.roundBuffer[(params.roundIndex + 2) % 3] > 15 && params.roundBuffer[(params.roundIndex + 1) % 3] > 15)
	{
		this->reproduceState = Day::INCREMENT10;
		this->reproduceKoeficient = 1.1;
	}
	// 3 studené dny - sniž o 10 procent počet vajíček
	else if(this->reproduceState != Day::DECREMENT10 && params.roundBuffer[0] < 15 && params.roundBuffer[1] < 15 && params.roundBuffer[2] < 15)
	{
		this->reproduceState = Day::DECREMENT10;
		this->reproduceKoeficient = 0.9;
	}
	// dva studené dny - sniž o 5 procent počet vajíček
	else if(this->reproduceState != Day::DECREMENT5 && params.roundBuffer[(params.roundIndex + 2) % 3] < 15 && params.roundBuffer[(params.roundIndex + 1) % 3] < 15)
	{
		this->reproduceState = Day::DECREMENT5;
		this->reproduceKoeficient = 0.95;
	}
	// jinak neměň počet nakladených vajíček
	else
	{
		this->reproduceState = Day::DEFAULT;
		this->reproduceKoeficient = 1;
	}
}

// nastaví množství vody, které včely potřebují na den - závisí na teplotě
void Day::setWaterNeeds(int temp)
{
	if(temp < 10) params.water = 200;
	else if(temp > 30) params.water = 2000;
	else
	{
		params.water = (200 + (temp-10) * 90) * 1000;
	}
}
