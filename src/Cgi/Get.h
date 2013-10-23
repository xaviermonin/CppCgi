/*!
* \file Get.h
* Fichier entête de la classe Get.
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

#ifndef _CGI_GET_H_
#define _CGI_GET_H_

#include "Exception.h"
#include "Outils.h"
#include "Modeles/Singleton.h"
#include <string>

namespace CppCgi {

class ExceptionGetObtention : public Exception {
public: ExceptionGetObtention(const std::string& description_erreur,
								 std::string nom_fichier, int num_ligne)
		: Exception(description_erreur, nom_fichier, num_ligne)
		{}
};

/*!
* \class Get
* \brief Classe d'accès aux paramètres get.
* Les valeur passées par paramètre get sont parsées dans cette classe.
* Il s'agit d'une classe singleton.
*/
class Get : public Modele::Singleton<Get>
{
	friend class Modele::Singleton<Get>;
public:
	bool elementExistant(const std::string& clef);
	const std::string& operator [](const std::string& clef) throw (ExceptionIntrouvable);
	const std::string& Valeur(const std::string& clef) throw (ExceptionIntrouvable);
	const AssocStrings& ListeElements();
private:
	Get();
	//! \brief Destructeur de Get.
	virtual ~Get() {}

	//! \brief Liste des valeurs passées par paramètre get reçues.
	AssocStrings liste_get;
};

} // namespace

#endif
