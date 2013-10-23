#include "EnvSimulation.h"

using namespace CppCgi;

EnvSimulation::EnvSimulation() : fichier_var_env("var_env.sim"), fichier_entree("entree.sim")
{
}

EnvSimulation::~EnvSimulation()
{
}

std::istream& EnvSimulation::obtEntree()
{
	if (!entree.is_open())
		entree.open(fichier_entree.c_str());

	return entree;
}

void EnvSimulation::FichierEntree(const std::string& fichier)
{
	fichier_entree = fichier;
}

const std::string& EnvSimulation::FichierEntree() const
{
	return fichier_entree;
}

void EnvSimulation::FichierVarEnv(const std::string& fichier)
{
	fichier_entree = fichier;
}

const std::string& EnvSimulation::FichierVarEnv() const
{
	return fichier_var_env;
}

bool EnvSimulation::obtVarEnv(const std::string& variable, std::string& valeur)
{
	if (!var_env.is_open())
	{
		var_env.open(fichier_var_env.c_str());

		std::stringstream buffer;
		buffer << var_env.rdbuf();

		liste_vars = Outils::scinderEnClefsValeurs(buffer.str(), "=", "\n");
	}

	if (liste_vars.find(variable) == liste_vars.end())
		return false;
	else
	{
		valeur = liste_vars[variable];
		return true;
	}
}
