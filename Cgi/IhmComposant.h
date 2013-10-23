#ifndef _IHM_COMPOSANT_H
#define _IHM_COMPOSANT_H

#include "Cache.h"

#include <string>
#include <vector>

namespace CppCgi {

class IhmComposant
{
public:
	void ajouterComposant(IhmComposant* composant_tas, bool auto_suppression=true);
	void ajouterComposant(IhmComposant& composant_pile);
	void ajouterContenu(const std::string& contenu);

	std::string rendu() const;

	static unsigned int instances_crees;
	static unsigned int instances_detruites;

protected:
	IhmComposant();
	virtual ~IhmComposant();

	typedef std::vector<IhmComposant*> ListeComposants;

	virtual std::string renduComposant() const;

	void nettoyerComposants();

	ListeComposants liste_composants;
	ListeComposants liste_suppression;
private:
	friend void Cache::obtDuCache();
	friend void Cache::mettreEnCache();

	bool afficher_cache;
	std::string cache;

	std::string contenu; // Créé par ajoutContenu()
};

} // namespace

#endif
