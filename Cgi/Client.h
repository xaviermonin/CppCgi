/*!
* \file Client.h
* Fichier entête de la classe Client.
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
*
* \todo Placer les méthodes dans les bonnes classes (Serveur et Client).
* \todo Ajouter une méthode vérifiant le type de requete (Post?).
*/

#ifndef _CGI_CLIENT_H_
#define _CGI_CLIENT_H_

#include "Outils.h"

namespace CppCgi {

namespace TypeAuth
{
	enum Type { HTTP, CONFIG, COOKIE, INCONNUE, AUCUNE };
}

namespace TypeRequete
{
	enum Type { POST, GET, HEAD, PUT, AUTRE };
}

/*!
* \class Client
* \brief Représente les informations client.
*/
class Client
{
public:
	static ListeClassee Mimes();
	static ListeClassee Langages();
	static ListeClassee JeuxDeCaracteres();
	static std::string UrlProvenance();
	static bool RequeteAjax();
	static std::string Navigateur();
	static std::string IpClient();
	static TypeAuth::Type Authentification();
	static unsigned short Port();
	static TypeRequete::Type MethodeRequete();
	static std::string TypeContenuEnvoye();

private:
	static ListeClassee analyserEtClasser(const std::string& texte);
};

} // namespace

#endif
