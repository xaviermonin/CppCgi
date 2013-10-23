/*!
* \file Serveur.cpp
* Fichier source de la classe Serveur.
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

#include "Exception.h"
#include "Serveur.h"
#include "Outils.h"
#include "Environnement.h"
#include "Convertisseur.h"

using namespace CppCgi;

/*!
* \brief Obtient le nom du serveur (ex: http://www.xavier-monin.net).
* \return Nom du serveur.
*/
std::string Serveur::NomServeur()
{
	std::string protocole = "http://";
	std::string suffixe;
	unsigned short port_verif = 80;

	unsigned int port = Port();
	try {
		std::string https;
		Environnement::VarEnv("HTTPS", https);
		if (https == "on")
		{
			protocole = "https://";
			port_verif = 443;
		}
	}
	catch(...) {}
	
	if (port != port_verif)
			suffixe = ':' + Convertire<std::string>(port) + '/';
	
	std::string server_name;
	Environnement::VarEnv("SERVER_NAME", server_name);
	return protocole+server_name+suffixe; // A COMPLETER
}

/*!
* \brief Obtient le nom du script (ex: /index.cgi).
* \return Nom du script.
*/
std::string Serveur::NomScript()
{
	std::string script_name;
	Environnement::VarEnv("SCRIPT_NAME", script_name);
	return script_name;
}

/*!
* \brief Obtient l'URL de la requête (ex: /article5-page1.html).
* \return URL de la requête.
*/
std::string Serveur::UrlRequete()
{
	std::string request_uri;
	Environnement::VarEnv("REQUEST_URI", request_uri);
	return request_uri;
}

/*!
* \brief Obtient le chemin complet du script courant (ex: http://www.xavier-monin.net/index.cgi).
* \return Chemin complet du script courant.
*/
std::string Serveur::CheminComplet()
{
	return NomServeur()+NomScript();
}

/*!
* \brief Obtient les paramètres d'url. (ex: ?action=editer)
* \return Chaine de caractères des paramètres d'url.
*/
std::string Serveur::ParametresUrl()
{
	std::string query_string;
	Environnement::VarEnv("QUERY_STRING", query_string);
	return query_string;
}

/*!
* \brief Obtient le port du serveur sur lequel le client est connecté.
* \return Port du serveur.
*/
unsigned short Serveur::Port()
{
	std::string server_port;
	Environnement::VarEnv("SERVER_PORT", server_port);
	return Convertire<unsigned int>(server_port);
}

std::string Serveur::RacineDocument()
{
	std::string document_root;
	Environnement::VarEnv("DOCUMENT_ROOT", document_root);
	return document_root;
}

std::string Serveur::IpServeur()
{
	std::string server_addr;
	Environnement::VarEnv("SERVER_ADDR", server_addr);
	return server_addr;
}
