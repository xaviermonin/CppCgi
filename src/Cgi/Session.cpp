/*!
* \file Session.cpp
* Fichier source de la classe Session.
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

#include "Session.h"
#include "Cookie.h"
#include "Client.h"
#include "Exception.h"
#include "Environnement.h"
#include "Macros.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <sys/types.h>
#include <sys/stat.h>

#if defined (__WINDOWS__)
#include <windows.h>
#else
#include <dirent.h>
#endif

using namespace CppCgi;

/*!
* \brief Constructeur de Session appelé par Instance().
* Crée l'identifiant de session s'il n'existe pas.
*/
Session::Session() : modifie(false)
{
	std::srand((unsigned int)time(0));
	for (int i=0; i<10; ++i)
		std::rand();

	supprimerSessionsExpirees();

	Cookie& cookie = Cookie::Instance();
	try { // Tentative de recupération du cookie session
		identifiant_session = cookie.obtCookie("SESSIONID");
		charger();
	} catch (const Exception&) {
		// Création du cookie session puisqu'il n'existe pas
		identifiant_session = genererIdentifiantSession();
		cookie.ajouterModifierCookie("SESSIONID", identifiant_session);
	}
}

Session::~Session()
{
	try { sauvegarder(); }
	catch (const std::exception& except)
	{ Environnement::Debug() << ERREUR_DESTRUCTEUR << except.what(); }
}

/*!
* \brief Supprimer une variable de session.
* \param[in] clef : Nom de la clef de la variable de session.
*/
void Session::supprimer(const std::string& clef)
{
	if (elementExistant(clef))
		liste_donnees_session.erase(clef);
	else
		throw Exception("Session: Elément inexistant", INFOS_EXCEPTION);

	modifie = true;
}

/*!
* \brief Supprimer toutes les variables de session.
*/
void Session::supprimerTout()
{
	std::string nom_fichier = repertoire+identifiant_session+Client::IpClient();
	remove(nom_fichier.c_str());
	liste_donnees_session.clear();
	modifie = false;
}

/*!
* \brief Charge les variables de session du client.
*/
void Session::charger()
{
	std::string tmp = (repertoire+identifiant_session+Client::IpClient());
	std::ifstream fichier(tmp.c_str());

	if (!fichier)
		return;
	
	while(!fichier.eof())
	{
		std::string clef, valeur;
		fichier >> clef;

		if (clef.empty())
			continue;

		fichier >> valeur;

		clef = Outils::decoderUrl(clef);
		valeur = Outils::decoderUrl(valeur);
		liste_donnees_session[clef] = valeur;
	}

	fichier.close();
}

/*!
* \brief Définit le répertoire de session.
* \param[in] repertoire : Nom du répertoire de session.
*/
void Session::RepertoireSession(const std::string& repertoire)
{
	Session::repertoire = repertoire+'/';
}

/*!
* \brief Définit le temps d'expiration de la session.
* \param[in] expiration : Temps d'expiration de la session en secondes.
*/
void Session::TempsExpiration(unsigned long expiration)
{
	secondes_expiration = expiration;
}

/*
* \brief Obtient une variable de session
* \param[in] clef : Clef de la variable de session.
* \return Valeur de la variable de session.
*/
const std::string& Session::operator [](const std::string &clef)
{
	return Element(clef);
}

/*!
* \brief Obtient une variable de session.
* \param[in] clef : Clef de la variable de session.
* \return Valeur de la variable de session.
*/
const std::string& Session::Element(const std::string& clef)
{
	if (liste_donnees_session.find(clef) == liste_donnees_session.end())
		throw Exception("Elément inexistant.", INFOS_EXCEPTION);

	return liste_donnees_session[clef];
}

/*!
* \brief Crée/définit une variable de session.
* \param[in] clef : Clef de la variable de session.
* \param[in] valeur : Valeur de la variable de session.
*/
void Session::Element(const std::string& clef, const std::string& valeur)
{
	liste_donnees_session[clef] = valeur;
	modifie = true;
}

#ifdef __WINDOWS__
/*!
* \brief Supprime les sessions qui ont expirés.
*/
void Session::supprimerSessionsExpirees()
{
	struct stat s;
	std::string nom_fichier_ref = repertoire+"ref_expiration";
	stat(nom_fichier_ref.c_str(), &s);

	if ( !( std::ofstream(nom_fichier_ref.c_str(), std::ios::trunc) ) )
		throw Exception("Fichier de référence d'expiration impossible à mettre à jour", INFOS_EXCEPTION);

	time_t maintenant = time(0);

	if ( (time_t)(maintenant+secondes_expiration) < maintenant)
		throw Exception("Temps d'expiration trop grand, overflow de time_t.", INFOS_EXCEPTION);

	// Vérification de la nécessité de suppression des fichiers session
	if ( maintenant < (time_t)( s.st_mtime+secondes_expiration ) )
		return; // Suppression inutile, derniere suppression de session récente

	WIN32_FIND_DATAA fichier;
	HANDLE h;
	std::string tmp = repertoire+"*.*";

	h = FindFirstFileA(tmp.c_str(), &fichier);
	if (h == INVALID_HANDLE_VALUE)
		throw Exception("Répertoire de session invalide", INFOS_EXCEPTION);

	do
	{
		std::string nom_fichier = repertoire + fichier.cFileName;
		if (nom_fichier == (repertoire+".") ||
			nom_fichier == (repertoire+"..")||
			nom_fichier == (repertoire+nom_fichier_ref))
			continue;

		struct stat s;
		if (stat(nom_fichier.c_str(), &s)!=0)
		{
			FindClose(h);
			throw Exception("Fichier de session "+nom_fichier+" invalide", INFOS_EXCEPTION);
		}
		if ( maintenant > (time_t)( s.st_atime+secondes_expiration ) )
			remove(nom_fichier.c_str());

	} while (FindNextFileA(h, &fichier));
	FindClose(h);
}
#else
void Session::supprimerSessionsExpirees()
{
	struct stat s;
	std::string nom_fichier_ref = repertoire+"ref_expiration";
	stat(nom_fichier_ref.c_str(), &s);

	if ( !( std::ofstream(nom_fichier_ref.c_str(), std::ios::trunc) ) )
		throw Exception("Fichier de référence d'expiration impossible à mettre à jour", INFOS_EXCEPTION);

	time_t maintenant = time(0);

	if ( (time_t)(maintenant+secondes_expiration) < maintenant)
		throw Exception("Temps d'expiration trop grand, overflow de time_t.", INFOS_EXCEPTION);

	// Vérification de la nécessité de suppression des fichiers session
	if ( maintenant < (time_t)( s.st_mtime+secondes_expiration ) )
		return; // Suppression inutile, derniere suppression de session récente

	DIR* rep = opendir(repertoire.c_str());

	if (rep == NULL)
		throw Exception("Répertoire de session invalide", INFOS_EXCEPTION);

	struct dirent* ent;
	while ((ent = readdir(rep)) != NULL)
	{
		std::string nom_fichier = repertoire+ent->d_name;
		
		if (nom_fichier == "."			||
			nom_fichier == ".."			||
			nom_fichier == nom_fichier_ref)
			continue;

		if (stat(nom_fichier.c_str(), &s)!=0)
		{
			closedir(rep);
			throw Exception("Fichier de session "+nom_fichier+" invalide", INFOS_EXCEPTION);
		}

		if ( maintenant > (time_t)( s.st_atime+secondes_expiration ) )
			remove(nom_fichier.c_str());
	}
	closedir(rep);

	if ( !( std::ifstream(nom_fichier_ref.c_str()) ) )
		throw Exception("Fichier de référence expiration inaccesible", INFOS_EXCEPTION);
}
#endif

/*!
* \brief Vérifie l'existance d'une variable de session.
* \param[in] clef : La clef de la variable de session à vérifier.
* \return Si la variable de session existe: true sinon false.
*/
bool Session::elementExistant(const std::string& clef)
{
	if (liste_donnees_session.find(clef) == liste_donnees_session.end())
		return false;
	else
		return true;
}

//! \brief Sauvegarde la session.
void Session::sauvegarder()
{
	if (!Session::instanceExistante())
		return;

	Session& session = Session::Instance();

	if (!session.modifie)
		return;

	std::ofstream fichier((repertoire+session.identifiant_session+Client::IpClient()).c_str(), std::ios::trunc);
	if (!fichier)
		throw Exception("Impossible de sauvegarder le fichier de session.", INFOS_EXCEPTION);

	for (AssocStrings::iterator it=session.liste_donnees_session.begin();
				it!=session.liste_donnees_session.end(); ++it)
		fichier << Outils::encoderUrl(it->first) << " " << Outils::encoderUrl(it->second) << std::endl;

	fichier.close();
}

/*!
* \brief Génère un identifiant unique.
* L'identifiant est composé d'une chaine aléatoire de 10 caractère et de l'ip du client.
* \return L'identifiant généré.
*/
std::string Session::genererIdentifiantSession()
{
	char caracteres_codage[] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m',
								 'n','o','p','q','r','s','t','u','v','w','x','y','z',
								 'A','B','C','D','E','F','G','H','I','J','K','L','M',
								 'N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
								 '0','1','2','3','4','5','6','7','8','9','-','_','+'};

	std::string id;
	id.reserve(25); // 10 aléatoires + 15 IP max.

	for (unsigned int i = 0; i < 10; ++i)
	{
		// Génère un nombre (pseudo) aléatoire entre 0 et sizeof(caracteres_codage).
		int index = (int)((double)std::rand()/(RAND_MAX+1.0) * (sizeof(caracteres_codage)/sizeof(char)) );
		id.push_back(caracteres_codage[ index ]);
	}

	return id;
}

std::string Session::repertoire = "session/";
unsigned long Session::secondes_expiration = 86400; // 1 journée
std::string Session::identifiant_session = "";
