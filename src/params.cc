#include "params.h"

// inicializace maximální úrody
const int Params::MARCH_AVERAGE_HARVEST = 1000;
const int Params::APRIL_AVERAGE_HARVEST = 40000;
const int Params::MAY_AVERAGE_HARVEST = 50000;
const int Params::JUNE_AVERAGE_HARVEST = 40000;
const int Params::JULY_AVERAGE_HARVEST = 40000;
const int Params::AUGUST_AVERAGE_HARVEST = 24000;
const int Params::SEPTEMBER_AVERAGE_HARVEST = 13000;
const int Params::OCTOBER_AVERAGE_HARVEST = 1000;

// inicializace rozsahu potravy
const int Params::MARCH_HARVEST_RANGE = 30;
const int Params::APRIL_HARVEST_RANGE = 5;
const int Params::MAY_HARVEST_RANGE = 3;
const int Params::JUNE_HARVEST_RANGE = 5;
const int Params::JULY_HARVEST_RANGE = 8;
const int Params::AUGUST_HARVEST_RANGE = 8;
const int Params::SEPTEMBER_HARVEST_RANGE = 15;
const int Params::OCTOBER_HARVEST_RANGE = 30;


// otestování vstupních parametrů
bool Params::set(int argc, char * argv[])
{
	// nápověda
	if(argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0))
	{
		this->help();
		return false;
	}
	// zadání vstupního souboru popisující rok, který chceme modelovat
	if(argc == 3 && strcmp(argv[1], "--input") == 0)
	{
		this->loadInput(argv[2]);
	}
	// chyba parametrů
	else
	{
		this->help();
		throw std::invalid_argument("Missing/invalid argument");
	}

	//vzdalenost nejblizsi potravy
	int marchDistance = this->harvestRadius.at(0).first;
	int aprilDistance = this->harvestRadius.at(1).first;
	int mayDistance = this->harvestRadius.at(2).first;
	int juneDistance = this->harvestRadius.at(3).first;
	int julyDistance = this->harvestRadius.at(4).first;
	int augutstDistance = this->harvestRadius.at(5).first;
	int septemberDistance = this->harvestRadius.at(6).first;
	int octoberDistance = this->harvestRadius.at(7).first;

	// úrodu v procentech
	double marchHarvest = this->harvestRadius.at(0).second;
	double aprilHarvest = this->harvestRadius.at(1).second;
	double mayHarvest = this->harvestRadius.at(2).second;
	double juneHarvest = this->harvestRadius.at(3).second;
	double julyHarvest = this->harvestRadius.at(4).second;
	double augustHarvest = this->harvestRadius.at(5).second;
	double septemberHarvest = this->harvestRadius.at(6).second;
	double octoberHarvest = this->harvestRadius.at(7).second;

	// vytvoření pomocného vektoru, pro počítání letů včel podle zdrojů potravy
	this->radius.push_back(std::make_tuple(-1,0,0,0)); // leden
	this->radius.push_back(std::make_tuple(-1,0,0,0)); // unor
	this->radius.push_back(std::make_tuple(marchDistance, marchHarvest * MARCH_AVERAGE_HARVEST,30,marchHarvest * MARCH_AVERAGE_HARVEST)); // brezen
	this->radius.push_back(std::make_tuple(aprilDistance, aprilHarvest * APRIL_AVERAGE_HARVEST,5,aprilHarvest * APRIL_AVERAGE_HARVEST)); // duben
	this->radius.push_back(std::make_tuple(mayDistance, mayHarvest * MAY_AVERAGE_HARVEST,3,mayHarvest * MAY_AVERAGE_HARVEST)); // kveten
	this->radius.push_back(std::make_tuple(juneDistance, juneHarvest * JUNE_AVERAGE_HARVEST,5,juneHarvest * JUNE_AVERAGE_HARVEST)); // cerven
	this->radius.push_back(std::make_tuple(julyDistance, julyHarvest * JULY_AVERAGE_HARVEST,8,julyHarvest * JULY_AVERAGE_HARVEST)); // cervenec
	this->radius.push_back(std::make_tuple(augutstDistance, augustHarvest * AUGUST_AVERAGE_HARVEST,8,augustHarvest * AUGUST_AVERAGE_HARVEST)); // srpen
	this->radius.push_back(std::make_tuple(septemberDistance, septemberHarvest * SEPTEMBER_AVERAGE_HARVEST,15,septemberHarvest * SEPTEMBER_AVERAGE_HARVEST)); // zari
	this->radius.push_back(std::make_tuple(octoberDistance, octoberHarvest * OCTOBER_AVERAGE_HARVEST,30,octoberHarvest * OCTOBER_AVERAGE_HARVEST)); // rijen
	this->radius.push_back(std::make_tuple(-1,0,0,0)); // listopad
	this->radius.push_back(std::make_tuple(-1,0,0,0)); // prosinec

	// parametry pro graf
	this->beeCountLog.open("experiments/beeCount.dat", std::ofstream::trunc);
	this->honeyLog.open("experiments/honey.dat", std::ofstream::trunc);
	// this->weatherLog.open("weather.dat", std::ofstream::trunc);

	return true;
}

// načtení vstupů ze souboru
void Params::loadInput(std::string filename)
{
	int monthCount = 12;
	int radiusCount = 8;
	std::ifstream file(filename);

	// test souboru
	if(!file.good()) throw std::invalid_argument("File not found");

	// načtení startovních včel a medu v úlu
	file >> this->beeCount;

	if(this->beeCount < 2000 || this->beeCount > 25000)
	{
		throw std::invalid_argument("Počet včel mimo platný rozsah <2000, 25000>");
	}

	file >> this->honey;

	if(this->honey < 0)
	{
		throw std::invalid_argument("Množství medu nesmí být záporné");
	}


	// načtení počasí
	while(monthCount--)
	{
		double first, second;

		file >> first;
		file >> second;
		this->months.push_back(std::make_pair(first, second));
	}

	// načtení úrody
	while(radiusCount--)
	{
		int first;
		double second;

		file >> first;
		file >> second;

		if(first < 0 || first > 8000)
		{
			throw std::invalid_argument("Vzdálenost potravy musí být v intervalu <0, 8000>");
		}

		if(second < 0 || second > 1)
		{
			throw std::invalid_argument("Úroda potravy musí být v intervalu <0, 1>");
		}

		this->harvestRadius.push_back(std::make_pair(first, second));
	}

}

// tvoření grafů
void Params::log(int dayNumber)
{
	this->beeCountLog << dayNumber << " " << this->beeCount << " " << this->longlivingCount << " " << this->foragerCount << std::endl;
	this->honeyLog << dayNumber << " " << this->honey/1000000 << " " << this->honeyTotal/1000000 << std::endl;
	// this->weatherLog << dayNumber << " " << this->temperature << std::endl;
}

// nápověda
void Params::help()
{
	std::cout << "SYNOPSIS" << std::endl;
	std::cout << "\tsimulace [--help, -h, --input file]" << std::endl;
	std::cout << "STRUCT of input file" << std::endl;
	std::cout << "\t INT(bee count 1.1.XXXX)" << std::endl;
	std::cout << "\t INT(honey count 1.1.XXXX [kg])" << std::endl;
	std::cout << "\t12 × line: DOUBLE(average temperature) DOUBLE(swing)" << std::endl;
	std::cout << "\t8 × line: INT(harvest distance [m]) DOUBLE(harvest percentage <0, 1>, 0= desert, 1 = rain forest)" << std::endl;
	std::cout << "AUTHORS" << std::endl;
	std::cout << "\t Lukáš Vokráčko, xvokra00" << std::endl;
	std::cout << "\t Michal Veselý, xvesel63" << std::endl;

	//TODO názevbinárky, výstupy které produkujeme
}
