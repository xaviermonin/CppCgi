/*!
* \file Convertire.h
* Fichier entête de la classe Convertire.
* \author Xavier Monin
* \date 20/09/2009
* \par projet CppCgi
* \namespace CppCgi
* \par licence
* Copyright 2009 - Xavier Monin - Tous droits réservés
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#ifndef _CONVERTISSEUR_H_
#define _CONVERTISSEUR_H_

#include "Macros.h"
#include "Environnement.h"

#include <string>
#include <sstream>
#include "Exception.h"

namespace CppCgi
{

class ExceptionConversionImpossible : public Exception {
public: ExceptionConversionImpossible(const std::string& description_erreur,
									  std::string nom_fichier, int num_ligne)
		: Exception(description_erreur, nom_fichier, num_ligne)
		{}
};

template<typename T_S, typename T_E>
T_S Convertire(const T_E& entree)
{
	T_S sortie;

	std::stringstream ss;
	ss.imbue(std::locale::classic());

	ss << entree;
	if (!ss)
		throw ExceptionConversionImpossible("Conversion impossible à partir du type: "
											+ NOM_OBJET(entree), INFOS_EXCEPTION);
	ss >> sortie;
	if (!ss)
		throw ExceptionConversionImpossible("Conversion impossible vers le type: "
											+ NOM_OBJET(sortie), INFOS_EXCEPTION);

	return sortie;
}

}

#endif
