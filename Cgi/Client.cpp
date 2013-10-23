/*!
* \file Client.cpp
* Fichier source de la classe Client.
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

//\todo Vérifier les Environnement::VarEnv() suite à la modification throw safe

#include "Client.h"
#include "Environnement.h"
#include "Convertisseur.h"

#include <algorithm>

using namespace CppCgi;

/*!
* \brief Obtient les types Mime acceptés par le client.
* \return Liste des types mimes acceptés par le client.
*/
ListeClassee Client::Mimes()
{
	std::string http_accept;
	Environnement::VarEnv("HTTP_ACCEPT", http_accept);
	return analyserEtClasser(http_accept);
}

/*!
* \brief Obtient les langues acceptées par le client.
* \return Liste les langues acceptées par le client.
*/
ListeClassee Client::Langages()
{
	std::string http_accept_language;
	Environnement::VarEnv("HTTP_ACCEPT_LANGUAGE", http_accept_language);
	return analyserEtClasser(http_accept_language);
}

/*!
* \brief Obtient les jeux de caractères acceptés par le client.
* \return Liste les jeux de caractères acceptés par le client.
*/
ListeClassee Client::JeuxDeCaracteres()
{
	std::string http_accept_charset;
	Environnement::VarEnv("HTTP_ACCEPT_CHARSET", http_accept_charset);
	ListeClassee liste = analyserEtClasser(http_accept_charset);

	bool iso = false;
	bool etoile = false;
	unsigned short score_etoile = 100;

	for (ListeClassee::iterator it = liste.begin(); it != liste.end(); ++it)
	{
		if (it->first == "ISO-8859-1")
		{
			iso = true;
			if (etoile)
				break;
		}
		if (it->first == "*")
		{
			etoile = true;
			score_etoile = it->second;
			if (iso)
				break;
		}
	}

	if (!iso)
		liste.insert(liste.begin(), PaireClassement("ISO-8859-1", score_etoile));

	return liste;
}

/*!
* \brief Obtient l'url de provenance du client.
* \return Url de provenance du client.
*/
std::string Client::UrlProvenance()
{
	std::string http_referer;
	Environnement::VarEnv("HTTP_REFERER", http_referer);
	return http_referer;
}

/*!
* \brief Renvoi l'information sur une requete Ajax.
* \return true si le client a envoyé une requete Ajax sinon false.
*/
bool Client::RequeteAjax()
{
	std::string http_x_requested_with;
	return Environnement::VarEnv("HTTP_X_REQUESTED_WITH", http_x_requested_with);
}

/*!
* \brief Obtient le type de navigateur du client.
* \return Type de navigateur du client.
*/
std::string Client::Navigateur()
{
	std::string http_user_agent;
	Environnement::VarEnv("HTTP_USER_AGENT", http_user_agent);
	return http_user_agent;
}

/*!
* \brief Obtient l'IP du client.
* \return IP du client.
*/
std::string Client::IpClient()
{
	std::string ip;
	if (!Environnement::VarEnv("HTTP_X_FORWARDED_FOR", ip))
	{
		if (!Environnement::VarEnv("HTTP_CLIENT_IP", ip))
		{
			if (!Environnement::VarEnv("REMOTE_ADDR", ip))
				throw Exception("Impossible d'obtenir l'IP du client.", INFOS_EXCEPTION);
		}
	}

	return ip;
}

/*
* \brief Obtient le type d'authentification du client (cookie, http, config).
* \return Type d'authentification.
*/
TypeAuth::Type Client::Authentification()
{
	std::string type;

	if (!Environnement::VarEnv("AUTH_TYPE", type))
		return TypeAuth::AUCUNE;

	if (Outils::stringsEquivalentes(type, "http"))
		return TypeAuth::HTTP;
	if (Outils::stringsEquivalentes(type, "config"))
		return TypeAuth::CONFIG;
	if (Outils::stringsEquivalentes(type, "cookie"))
		return TypeAuth::COOKIE;

	return TypeAuth::INCONNUE;
}

/*
* \brief Obtient le port utilisé par le client.
* \return Port du client.
*/
unsigned short Client::Port()
{
	std::string port;
	Environnement::VarEnv("REMOTE_PORT", port);
	return Convertire<unsigned short>(port);
}

/*
* \brief Obtient le type de requête utilisé par le client (get, post, etc).
* \return Type de requête.
*/
TypeRequete::Type Client::MethodeRequete()
{
	std::string methode;
	Environnement::VarEnv("REQUEST_METHOD", methode);

	if (Outils::stringsEquivalentes(methode, "get"))
		return TypeRequete::GET;
	if (Outils::stringsEquivalentes(methode, "post"))
		return TypeRequete::POST;
	if (Outils::stringsEquivalentes(methode, "put"))
		return TypeRequete::PUT;
	if (Outils::stringsEquivalentes(methode, "head"))
		return TypeRequete::HEAD;

	return TypeRequete::AUTRE;
}

/*
* \brief Obtient le type de contenu de l'élément envoyé par le client.
* \return Type Mime de l'élément envoyé par le client.
*/
std::string Client::TypeContenuEnvoye()
{
	std::string content_type;
	Environnement::VarEnv("CONTENT_TYPE", content_type);
	return content_type;
}

//! \brief Prédicat effectuant un tri croissant.
bool trierPairParValeur (const PaireClassement& gauche, const PaireClassement& droite)
{
	return gauche.second > droite.second;
}

/*
* \brief Analyse et classe les informations de type Accept.
* Ex: nl-be, nl;q=0.8, en-us;q=0.5, en;q=0.3.
* \param[in] texte : Texte à analyser et classer.
* \return Liste classée des informations.
*/
ListeClassee Client::analyserEtClasser(const std::string& texte)
{
	std::string entree = Outils::remplacer(texte, " ", "");

	ListeClassee final;

	ListeString liste = Outils::extraire(entree, ",");
	for (ListeString::iterator it = liste.begin(); it != liste.end(); ++it)
	{
		unsigned short score = 100; // Score max.
		ListeString langue = Outils::extraire((*it), ";");

		if (langue.size() > 1)
		{
			ListeString q = Outils::extraire(langue[1], "=");
			// Passe de float à entier.
			score = (unsigned short)(Convertire<float>(q.at(1)) * 100);
		}

		final.push_back( PaireClassement(langue[0], score) );
	}

	std::sort(final.begin(), final.end(), trierPairParValeur);

	return final;
}
