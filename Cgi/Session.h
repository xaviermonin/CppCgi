/*!
* \file Session.h
* Fichier entête de la classe Session.
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

#ifndef _CGI_SESSION_H_
#define _CGI_SESSION_H_

#include "Outils.h"
#include "Modeles/Singleton.h"
#include <string>
#include <fstream>
#include <map>

namespace CppCgi {

/*!
* \class Session
* \brief Classe d'accès aux variables de session.
* Cette classe permet de sauvegarder des informations sur le client.
* Ces informations sont stockées coté serveur.
* Il s'agit d'une classe singleton.
*/
class Session : public Modele::Singleton<Session>
{
	friend class Modele::Singleton<Session>;
public:
	void supprimer(const std::string& clef);
	const std::string& Element(const std::string& clef);
	void Element(const std::string& clef, const std::string& valeur);
	const std::string& operator [](const std::string& clef);
	bool elementExistant(const std::string& clef);
	void supprimerTout();

	static void RepertoireSession(const std::string& repertoire);
	static void TempsExpiration(unsigned long expiration);
private:
	Session();
	//! \brief Destructeur de Session.
	virtual ~Session();

	static void sauvegarder();

	void charger();
	void supprimerSessionsExpirees();
	std::string genererIdentifiantSession();

	//! \brief Liste des valeurs de session.
	AssocStrings liste_donnees_session;
	//! \brief Identifiant unique représentant le client.
	static std::string identifiant_session;
	//! \brief Stock la modification de la session.
	bool modifie;
	//! \brife Nom du répertoire où sont stocké les informations de session. (session par défaut).
	static std::string repertoire;
	//! \brief Temps d'expiration de la session en seconde. (10s par défaut).
	static unsigned long secondes_expiration;
};

} // namespace

#endif
