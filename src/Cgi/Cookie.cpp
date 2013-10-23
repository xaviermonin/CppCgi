/*!
* \file Cookie.cpp
* Fichier source de la classe Cookie.
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

#include "Cookie.h"
#include "Exception.h"
#include "Client.h"
#include "Environnement.h"
#include <iostream>

using namespace CppCgi;

/*!
* \brief Constructeur de Cookie appelé par Instance().
*/
Cookie::Cookie()
{
	try {
		std::string chaine;
		Environnement::VarEnv("HTTP_COOKIE", chaine);
		liste_cookies_recus = Outils::scinderEtDecoderEnClefsValeurs(chaine, "=", "; ");
	} catch(const Exception&) {}
}

/*!
* \brief Vérifie l'existance d'un cookie reçu.
* \param[in] clef : Nom de la clef du cookie.
* \return Si la clef existe: true sinon false.
*/
bool Cookie::elementExistant(const std::string& clef)
{
	for (size_t i = 0; i < liste_cookies_a_envoyer.size(); ++i)
	{
		if (liste_cookies_a_envoyer[i].nom == clef)
			return true;
	}

	if (liste_cookies_recus.find(clef) == liste_cookies_recus.end())
		return false;
	else
		return true;
}

/*!
* \brief Ajoute un cookie à envoyer au navigateur client.
* \param[in] cookie : Structure du cookie à envoyer au client.
*/
void Cookie::ajouterModifierCookie(const InfoCookie& cookie)
{
	for (size_t i = 0; i < liste_cookies_a_envoyer.size(); ++i)
	{
		if (liste_cookies_a_envoyer[i].nom == cookie.nom)
		{
			liste_cookies_a_envoyer[i] = cookie;
			return;
		}
	}

	liste_cookies_a_envoyer.push_back(cookie);
}

/*!
* \brief Ajoute ou modifie un cookie à envoyer au navigateur client.
* \param[in] nom : Nom de la clef du cookie.
* \param[in] valeur : Valeur de la clef du cookie.
* \param[in] path : Url où le cookie est valide. Le domaine courant si vide.
* \param[in] expiration : Date à laquelle le cookie expire. N'expire jamais = 0.
*/
void Cookie::ajouterModifierCookie(const std::string& nom, const std::string& valeur,
						   const std::string& path, time_t expiration)
{
	InfoCookie cookie;
	cookie.nom= nom; cookie.valeur = valeur; cookie.path = path; cookie.expiration = expiration;
	ajouterModifierCookie(cookie);
}

/*!
* \brief Supprimer un cookie.
* \param[in] clef : Nom de la clef du cookie.
*/
void Cookie::supprimerCookie(const std::string& clef)
{
	ajouterModifierCookie(clef, "", "/", -1);
}

/*!
* \brief Obtient un cookie.
* \param[in] clef : Nom de la clef du cookie.
* \return La valeur du cookie.
*/
const std::string& Cookie::obtCookie(const std::string& clef)
{	
	for (size_t i = 0; i < liste_cookies_a_envoyer.size(); ++i)
	{
		if (liste_cookies_a_envoyer[i].nom == clef)
			return liste_cookies_a_envoyer[i].valeur;
	}

	if (liste_cookies_recus.find(clef) != liste_cookies_recus.end())
		return liste_cookies_recus[clef];

	throw Exception("Cookie inexistant", INFOS_EXCEPTION);
}

/*!
* \brief Obtient un cookie émis par le client.
* \param[in] clef : Nom de la clef du cookie.
* \return La valeur du cookie.
*/
const std::string& Cookie::operator [](const std::string& clef)
{
	return obtCookie(clef);
}

/*!
* \brief Obtient la liste des cookies reçues.
* \return La liste des cookies reçus.
*/
AssocStrings Cookie::ListeCookiesRecus()
{
	return liste_cookies_recus;
}

/*!
* \brief Génère le code HTTP des cookies pour le navigateur client.
* \return Code HTTP du rendu des cookies à envoyer au client.
*/
std::string Cookie::genererRendu()
{
	std::string resultat;
	for (unsigned int i = 0; i < liste_cookies_a_envoyer.size(); ++i)
	{
		resultat += "Set-Cookie: "+liste_cookies_a_envoyer[i].nom+
				    "="+liste_cookies_a_envoyer[i].valeur+";";

		if (!liste_cookies_a_envoyer[i].path.empty())
			resultat += " path="+liste_cookies_a_envoyer[i].path+";";

		if (liste_cookies_a_envoyer[i].expiration != 0)
		{
			time_t expiration = liste_cookies_a_envoyer[i].expiration;
			if (liste_cookies_a_envoyer[i].expiration == -1)
				expiration = 0;
				
			char buffer[50];
			tm* ptm = gmtime(&(liste_cookies_a_envoyer[i].expiration));
			strftime(buffer, sizeof(buffer), "%a, %d-%m-%Y %H:%M:%S GMT", ptm);
			resultat += " expires=";
			resultat += buffer;
			resultat += ";";
		}
		if (i+1 <liste_cookies_a_envoyer.size())
			resultat += '\n';
	}

	return resultat;
}
