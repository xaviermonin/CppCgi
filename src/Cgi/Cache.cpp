#include "Cache.h"
#include "IhmComposant.h"
#include "Exception.h"

#include <fstream>
#include <sstream>

#include <sys/types.h>
#include <sys/stat.h>

using namespace CppCgi;

Cache::Cache(IhmComposant* composant, const std::string& identifiant)
{
	this->identifiant = identifiant;
	this->composant = composant;

	nom_fichier = repertoire+"composant_"+identifiant;
}

Cache::~Cache()
{}

bool Cache::expire(unsigned long duree_vie)
{
	struct stat s;

	if (stat(nom_fichier.c_str(), &s)!=0)
		return true; // Echec de lecture des informations

	time_t date_valide = (s.st_ctime+(time_t)duree_vie);

	if (s.st_mtime < date_valide)
		throw Exception("Cache: overflow time_t. Durée de vie du cache trop grande.", INFOS_EXCEPTION);

	if (date_valide > time(0))
		return false;
	else
		return true;
}

void Cache::definirRepertoireCache(const std::string& repertoire)
{
	Cache::repertoire = repertoire+"/";
}

void Cache::mettreEnCache()
{
	std::ofstream fichier(nom_fichier.c_str());
	if (fichier.is_open())
	{
		fichier << composant->renduComposant()+"CACHE";
		fichier.close();
		composant->afficher_cache=false; // On n'affiche pas le cache si on vient de le créer
	}
}

void Cache::obtDuCache()
{
	std::ifstream fichier(nom_fichier.c_str());
	if (fichier.is_open())
	{
		std::stringstream buffer;
		buffer << fichier.rdbuf();
		composant->afficher_cache = true;
		composant->cache = buffer.str();
		fichier.close();
	}
}

std::string Cache::repertoire = "cache/";
