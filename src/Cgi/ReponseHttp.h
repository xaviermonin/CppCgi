/*!
* \file Outils.h
* Fichier entête de la classe ReponseHttp.
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

#ifndef _REPONSE_HTTP_H_
#define _REPONSE_HTTP_H_

#include "Modeles/Singleton.h"
#include "TypesUtilisateur.h"
#include <string>
#include <sstream>

namespace CppCgi {

//! \brief Type d'encodage des caractères.
namespace EJeuxDeCaracteres
{
	enum Type { UTF8, ISO_8859_1, ISO_8859_15 };
}

/*!
* \class ReponseHttp
* \brief Classe représentative de la réponse HTTP au navigateur client.
* Cette classe représente et génère le code HTTP à renvoyer au client.
*/
class ReponseHttp : public Modele::Singleton<ReponseHttp>
{
	friend class Modele::Singleton<ReponseHttp>;
public:
	const std::string& Reponse(const std::string& nom);
	void Reponse(const std::string& nom, const std::string& valeur);

	void Redirection(const std::string& url);
	const std::string& Redirection();

	void TypeContenu(const std::string& type_contenu);
	const std::string& TypeContenu();

	void EncodageCaractere(EJeuxDeCaracteres::Type encodage);
	EJeuxDeCaracteres::Type EncodageCaractere();

	std::ostringstream& Contenu();

	void executer();
private:
	ReponseHttp();
	virtual ~ReponseHttp(){}

	std::string genererRenduEntete();

	AssocStrings liste_reponses;

	// \brief Encodage des caractères. (UTF8 par défaut).
	EJeuxDeCaracteres::Type encodage_caract;

	std::ostringstream contenu;

	bool genere;
};

} // namespace

#endif
