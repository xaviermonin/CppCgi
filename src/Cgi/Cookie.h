/*!
* \file Cookie.h
* Fichier entête de la classe Cookie.
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

#ifndef _CGI_COOKIE_H_
#define _CGI_COOKIE_H_

#include "Outils.h"
#include "Modeles/Singleton.h"
#include <string>
#include <ctime>
#include <vector>

namespace CppCgi {

/*!
* \struct InfoCookie
* \brief Structure représentant les données d'un cookie.
*/
typedef struct
{
	//! \brief Nom du cookie.
	std::string nom;
	//! \brief Valeur du cookie.
	std::string valeur;
	//! \brief Domaine valide du cookie.
	std::string path;
	//! \brief Temps d'expiration du cookie (en secondes).
	time_t expiration;
} InfoCookie;

/*!
* \class Cookie
* \brief Classe d'accès aux cookies.
* Les cookies sont parsés dans cette classe.
* Elle crée aussi les cookies à envoyer au navigateur client.
* Il s'agit d'une classe singleton.
*/
class Cookie : public Modele::Singleton<Cookie>
{
	friend class Modele::Singleton<Cookie>;
public:
	void ajouterModifierCookie(const InfoCookie& cookie);
	void ajouterModifierCookie(const std::string& nom, const std::string& valeur="",
						const std::string& path="/", time_t expiration=0);
	void supprimerCookie(const std::string& clef);

	bool elementExistant(const std::string& clef);
	const std::string& obtCookie(const std::string& clef);
	const std::string& operator [](const std::string& clef);
	AssocStrings ListeCookiesRecus();

private:
	Cookie();
	//! \brief Destructeur de Cookie.
	virtual ~Cookie() {}

	friend class ReponseHttp;
	std::string genererRendu(); // Seul ReponseHttp utilise la génération HTTP des cookies.

	//! \brief Liste des cookies à renvoyer au client.
	std::vector< InfoCookie > liste_cookies_a_envoyer;
	//! \brief Liste des cookies reçus.
	AssocStrings liste_cookies_recus;
};

} // namespace

#endif
