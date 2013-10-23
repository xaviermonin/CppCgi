#include "EnvironnementCgi.h"
#include <cstdlib>

using namespace CppCgi;

EnvironnementCgi::EnvironnementCgi()
{
	premier_tour = true;
}

EnvironnementCgi::~EnvironnementCgi()
{
}

std::istream& EnvironnementCgi::obtEntree()
{
	return std::cin;
}

std::ostream& EnvironnementCgi::obtSortie()
{
	return std::cout;
}

void EnvironnementCgi::quitter()
{
	exit(0);
}

bool EnvironnementCgi::obtVarEnv(const std::string& variable, std::string& valeur)
{
	char* env = std::getenv(variable.c_str());
	if (env == NULL)
		return false;
	else
	{
		valeur = env;
		return true;
	}
}

bool EnvironnementCgi::nouveauClient()
{
	bool tmp = premier_tour;
	premier_tour = false;
	return tmp;
}
