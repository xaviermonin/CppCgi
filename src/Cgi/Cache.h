#ifndef _CGI_CACHE_H_
#define _CGI_CACHE_H_

#include <ctime>
#include <string>

namespace CppCgi {

class IhmComposant;

class Cache
{
public:
	Cache(IhmComposant* composant, const std::string& identifiant="");
	~Cache();

	bool expire(unsigned long duree_vie=0);
	void mettreEnCache();
	void obtDuCache();

	static void definirRepertoireCache(const std::string& repertoire);

private:
	static std::string repertoire;

	std::string identifiant;

	IhmComposant* composant;
	std::string nom_fichier;
};

} // namespace

#endif
