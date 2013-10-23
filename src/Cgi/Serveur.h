/*!
* \file Serveur.h
* Fichier entête de la classe Serveur.
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

#ifndef _CGI_SERVEUR_H_
#define _CGI_SERVEUR_H_

#include "Outils.h"

namespace CppCgi {

/*!
* \class Cookie
* \brief Représente les informations serveur.
*/
class Serveur
{
public:
	static std::string NomServeur();
	static std::string NomScript();
	static std::string CheminComplet();
	static std::string UrlRequete();
	static std::string ParametresUrl();
	static unsigned short Port();
	static std::string RacineDocument();
	static std::string IpServeur();
};

} // namespace

#endif
