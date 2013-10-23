#include "IhmComposant.h"
#include <iostream>

using namespace CppCgi;

IhmComposant::IhmComposant() : afficher_cache(false)
{
	instances_crees++;
}

IhmComposant::~IhmComposant()
{
	instances_detruites++;
	nettoyerComposants();
}

void IhmComposant::ajouterComposant(IhmComposant* composant_tas, bool auto_suppression)
{
	liste_composants.push_back(composant_tas);

	if (auto_suppression)
		liste_suppression.push_back(composant_tas);
}

void IhmComposant::ajouterComposant(IhmComposant& composant_pile)
{
	liste_composants.push_back(&composant_pile);
}

void IhmComposant::ajouterContenu(const std::string& contenu)
{
	IhmComposant* composant = new IhmComposant();
	composant->contenu = contenu;
	ajouterComposant(composant);
}

std::string IhmComposant::renduComposant() const
{
	return contenu;
}

void IhmComposant::nettoyerComposants()
{
	for (size_t i = 0; i < liste_suppression.size(); ++i)
	{
		if (liste_suppression[i] != NULL)
			delete liste_suppression[i];

		liste_suppression[i] = NULL;
	}

	liste_suppression.clear();
	liste_composants.clear();
}

std::string IhmComposant::rendu() const
{
	if (afficher_cache)
		return cache;
	else
		return renduComposant();
}

unsigned int IhmComposant::instances_detruites = 0;
unsigned int IhmComposant::instances_crees = 0;
