/****************************************************************
Projekt IMS, 2014 - Rok včelstva
Autoři: Michal Veselý (xvesel63), Lukáš Vokráčko (xvokra00)

 Soubor main.cc - obsahuje funkci main - inicializuje a spouští simulaci
*****************************************************************/

#include "main.h"
#include <ctime>

Params params; // globální instance třídy parametrů


// funkce main - inicializuje a spouští simulaci
int main(int argc, char * argv[])
{
	try
	{
		if(!params.set(argc, argv)) // parsování parametrů
		{
			return EXIT_SUCCESS;
		}
	}
	catch(std::exception & e)
	{
		std::cerr << e.what() << std::endl; // výpis chyby v případě špatných parametrů na stderr
		return EXIT_FAILURE;
	}

	params.longlivingCount = params.beeCount;

	RandomSeed(std::time(NULL)); // inicializace generátoru náhodných čísel
	Init(0, 365); // simulační čas 0-365 - dny roku
	params.startLongliving = Uniform(210, 230); // kdy se začnou rodit dlouhodobé včely

	(new Day(0))->Activate();

	Run();

	std::cout << "Množství medu [kg]: " << params.honey/1000000 << std::endl;
	std::cout << "Počet zimujících včel: " << params.beeCount << std::endl;
}
