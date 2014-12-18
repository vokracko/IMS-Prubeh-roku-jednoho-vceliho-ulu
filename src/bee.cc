/*************************************************************************************
Projekt IMS, 2014 - Rok včelstva
Autoři: Michal Veselý (xvesel63), Lukáš Vokráčko (xvokra00)

 Soubor bee.cc - implementace metod třídy Bee
  - metody chování včely v jednotlivých životních stádiích
  - odvozování počtu letu včely za potravou v závislosti na radiusu potravy
  - metoda určující pravděpodobnost umrznutí včely
*************************************************************************************/

#include "bee.h"

// nastavení časů jednotlivých stádií života včely
const int Bee::TIME_EGG = 4;
const int Bee::TIME_MAGGOT = 2;
const int Bee::TIME_FOODCHANGED = 4;
const int Bee::TIME_CAPPER = 11;
const int Bee::TIME_CLEANER = 3;
const int Bee::TIME_FEEDER = 7;
const int Bee::TIME_BUILDER = 7;
const int Bee::TIME_GUARD = 3;

// konstruktor včely - dědí od Process
Bee::Bee(int state, int dayCount): Process()
{
	this->state = state;
	this->dayCount = dayCount;
}

// popis chování včely
void Bee::Behavior()
{
	// zavolá metodu popisující chování včely v konkrétním stádiu
	switch(this->state)
	{
		case Bee::EGG: this->egg();break;
		case Bee::MAGGOT: this->maggot();break;
		case Bee::FOODCHANGED: this->foodchanged();break;
		case Bee::CAPPER: this->capper();break;
		case Bee::CLEANER: this->cleaner();break;
		case Bee::FEEDER: this->feeder();break;
		case Bee::BUILDER: this->builder() ;break;
		case Bee::GUARD: this->guard();break;
		case Bee::FORAGER: this->forager() ;break;
		case Bee::LONGLIVING: this->longliving() ;break;
		case Bee::DEAD: this->dead(); return;
	}

	// včela může umrznout, pokud je zima (čím větší zima, tím větší pravděpodobnost)
	// pouze pokud je pod nulou
	if(Random() < this->froze())
	{
		// rozlišení, jestli umírá dlouhodobá nebo krátkodobá včela
		if(this->state == Bee::LONGLIVING)
		{
			params.longlivingCount--;
			this->state = Bee::DEAD;
		}
		// umrznout můžou současně jen stádia dále než čistička
		else if(this->state >= Bee::CLEANER)
		{
			this->state = Bee::DEAD;
		}
		else
		{
			return;
		}

	}
	// aktivace chování včely na další den
	(new Bee(this->state, this->dayCount - 1))->Activate(Time + 1);
}

// chování vajíčka
void Bee::egg()
{
	this->set(Bee::MAGGOT, Bee::TIME_MAGGOT);
}

// chování larvy
void Bee::maggot()
{
	this->set(Bee::FOODCHANGED, Bee::TIME_FOODCHANGED);
}

// chování včely se  změnou potravy
void Bee::foodchanged()
{
	this->set(Bee::CAPPER, Bee::TIME_CAPPER);
}

// chování včely zavíčkované včely
void Bee::capper()
{
	// po tomto stádiu lze považovat včelu za dostatečně vyvinutou
	// je započítána do počtu skutečně žijících včel
	if(this->dayCount == 0)
	{
		params.beeCount++;
	}

	this->set(Bee::CLEANER, Bee::TIME_CLEANER);
}

// chování čističky
void Bee::cleaner()
{
	// včela čistička už jí i zásoby medu
	// předešlá stádia jí jen mateří kašičku - ta není v simulaci zahrnuta - není simulačně zajímavá
	params.honey -= 16;
	this->set(Bee::FEEDER, Bee::TIME_FEEDER);
}

// chování včely krmičky
void Bee::feeder()
{
	params.honey -= 16; // krmička jí zásoby
	this->set(Bee::BUILDER, Bee::TIME_BUILDER);
}

// chování včely stavitelky
void Bee::builder()
{
	params.honey -= 16; // stavitelka jí zásoby
	this->set(Bee::GUARD, Bee::TIME_GUARD);
}

// chování včely strážkyně
void Bee::guard()
{
	// může umřít při hlídání úlu (např. napadení rejskem, sršni apod.)
	if(Random() < 0.001)
	{
		this->state = Bee::DEAD;
		return;
	}
	params.honey -= 16; // jí zásoby

	if(this->dayCount == 0) // na konci stádia vzniká pro simulaci důležitá létavka
	{
		params.foragerCount++;
	}
	this->set(Bee::FORAGER, Uniform(7, 15)); // náhodná doba dožití
}

// chování včely létavky
void Bee::forager()
{
	int i = 0;

	// pokud je zapotřebí donést ještě vodu, letí včela pro vodu
	if(params.water > 0)
	{
		for(i = 0; i < Uniform(7, 16); ++i)
		{
			params.water -= Uniform(20, 40);
		}
	}
	else
	{
		// počet včel které létají
		// pokud není nutné donést vodu, letí pro nektar a pyl (pyl není modelován)
		for(i = 0; i < this->flightCount(); ++i)
		{
			// létavka může při letu zahynout - např. bodnutím žihadla
			if(Uniform(0, 1) <= 0.0005)
			{
				params.foragerCount--;
				this->state = Bee::DEAD;
				return;
			}

			// naplní košíček - plný má kapacitu 40-70 mg, ale nikdy jej neplní úplně
			// zdroje uvádí naplnění mezi 20 - 30 mg
			// nektar je v úle uložen pro zpracování na med
			// medu z nektaru je vyrobeno 30-50 %
			// model je zjednodušen okamžitou transformací nektaru na med (proces tvorby medu není modelován)
			double tmp = Uniform(20, 40) * Uniform(0.3, 0.5);
			params.honey += tmp;
			params.tmp += tmp;
			params.honeyTotal += tmp;

		}
	}


	// létavka jí, když létá, tudíž nesní celých 16 mg
	params.honey -= (16 - i);

	if(this->dayCount == 0)
	{
		params.foragerCount--;
	}

	this->set(Bee::DEAD, 0);
}

// včela zemřela
void Bee::dead()
{
	params.beeCount--;
}

// chování dlouhověké včely
void Bee::longliving()
{
	// dlouhověká včela zemřela přirozenou smrtí
	if(this->dayCount == 0)
	{
		params.longlivingCount--;
	}
	// narození dlouhověké včely
	else if(this->dayCount == ~0)
	{
		params.beeCount++;
		params.longlivingCount++;
	}

	this->setWinterOver(); // ověření, zda se rodí krátkověkké včely
	params.honey -= 16; // jezení dlouhověké včely
	this->set(Bee::DEAD, 0);
}

// nastavení přechodu do dalšího vývojového stádia
void Bee::set(int state, int dayCount)
{
	if(this->dayCount == 0)
	{
		this->state = state;
		this->dayCount = dayCount;
	}
}

// pokud je 3x zasebou více než 12°C, přestanou se rodit dlouhověké včely a začnou krátkověkké
void Bee::setWinterOver()
{
	if(params.roundBuffer[0] > 12 && params.roundBuffer[1] > 12 && params.roundBuffer[2] > 12 && params.winter)
	{
		if (Random()<0.1) // šance, že z dlouhověké včely se už stane krátkověkká - začne létat
		{
			this->state = Bee::FORAGER;
			params.longlivingCount--;
			params.foragerCount++;
		}
	}
}

// výpočet pravděpodobnosti včely na umrznutí
// pokud je pod nulou, tak s každým stupněm roste šance o půl promile
double Bee::froze()
{
	if(params.temperature < 0)
	{
		return params.temperature * -0.001;
	}
	else
	{
		return 0;
	}
}

// Vypocet poctu letu vcely pocitam podle radiusu potravy
// distance - aktualni vzdalenost potravy
// quality - mnozstvi potravy
// step - o kolik bude letat dale, kdyz spotrebuje veskerou potravu z tohoto rozsahu
int Bee::flightCount()
{
	int distance,quality,step,flights,qualityOrigin;
	std::tie(distance,quality,step,qualityOrigin) = params.radius[params.month];

	if(distance > 10000 || distance == -1) return 0; // pokud je potrava prilis daleko tak pro ni neleti
	else
	{
		flights = (10000-distance)/625 + 1; // vypocet poctu letu pro potravu

		std::get<1>(params.radius[params.month])= quality-flights; // snizeni mnozstvi o 1
		if (quality-flights < 0) // pokud uz zde potrava neni, posun distance o step
		{
			std::get<1>(params.radius[params.month])= qualityOrigin;
			std::get<0>(params.radius[params.month])= distance+step;
		}
		return flights;
	}
}
