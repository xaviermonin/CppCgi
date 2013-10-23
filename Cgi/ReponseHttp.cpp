/*!
* \file ReponseHttp.cpp
* Fichier source de la classe ReponseHttp.
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

#include "ReponseHttp.h"
#include "Environnement.h"
#include "Cookie.h"
#include "App.h"

using namespace CppCgi;

/*!
* \brief Réinitialise le code HTTP.
* Utile pour générer un autre entête.
*/
ReponseHttp::ReponseHttp() : contenu()
{
	TypeContenu("text/html");
	EncodageCaractere(EJeuxDeCaracteres::UTF8);

	genere = false;
}

/*
* \brief Ajoute/modifie une réponse à envoyer au client HTTP.
* La réponse est envoyée au client au format suivant: Clef: Valeur.
* \param[in] nom : Clef/Nom de la réponse.
* \param[in] valeur : Valeur de la réponse.
*/
void ReponseHttp::Reponse(const std::string &nom, const std::string &valeur)
{
	liste_reponses[nom] = valeur;
}

/*
* \brief Obtient une réponse qui sera envoyée au client HTTP.
* \param[in] nom: Clef/Nom de la réponse. Ex: Content-Lenght
*/
const std::string& ReponseHttp::Reponse(const std::string &nom)
{
	if (liste_reponses.find(nom) == liste_reponses.end())
		throw ExceptionIntrouvable("Réponse inexistante", INFOS_EXCEPTION);
	else
		return liste_reponses[nom];
}

/*!
* \brief Demande une redirection du client vers l'url concernée.
* ATTENTION A CORRIGER !!
* \param[in] url : Url où le client sera redirigé.
*/
void ReponseHttp::Redirection(const std::string& url)
{
	liste_reponses.clear();
	contenu.clear();

	Reponse("Location", url);

	executer();

	Environnement::Instance()->quitter();
}

/*!
* \brief Indique l'url où le client sera redirigé.
* Utilité de cette méthode ?
* \return Url de redirection.
*/
const std::string& ReponseHttp::Redirection()
{
	return Reponse("Location");
}

/*!
* \brief Définit le type MIME du contenu renvoyé au client.
* \param[in] type_contenu : Type Mime du contenu.
*/
void ReponseHttp::TypeContenu(const std::string& type_contenu)
{
	Reponse("Content-Type", type_contenu);
}

/*!
* \brief Obtient le type MIME du contenu renvoyé au client.
* \return Type Mime du contenu.
*/
const std::string& ReponseHttp::TypeContenu()
{
	return Reponse("Content-Type");
}

/*!
* \brief Défini l'encodage de caractères de la page.
* \param[in] encodage_caract : Encodage des caractères désiré.
*/
void ReponseHttp::EncodageCaractere(EJeuxDeCaracteres::Type encodage_caract)
{
	ReponseHttp::encodage_caract = encodage_caract;
}

/*!
* \brief Obtient l'encodage des caractactères de la page.
* \return Encodage des caractères de la page.
*/
EJeuxDeCaracteres::Type ReponseHttp::EncodageCaractere()
{
	return encodage_caract;
}

/*!
* \brief Obtient le flux du contenu (pas de l'entête) à envoyer au client.
* \return Le flux du contenu .
*/
std::ostringstream& ReponseHttp::Contenu()
{
	return contenu;
}

/*
* \brief Génère et envoi le code HTTP au client.
*/
void ReponseHttp::executer()
{
	Environnement::Sortie() << genererRenduEntete();
	Environnement::Sortie() << contenu.str();
}

/*!
* \brief Génère l'entête HTTP à envoyer au client.
* Le rendu n'est généré qu'une seule fois celà permet de s'assurer
* que l'entête sera envoyé au navigateur une seule fois.
* Il est nescessaire d'appeler reinitialiser() pour un générer un autre rendu.
* \return Code de l'entête HTTP ou une chaine vide si l'entête a déja été généré.
*/
std::string ReponseHttp::genererRenduEntete()
{
	if (genere)
		return "";

	Cookie& cookie = Cookie::Instance();

	std::string rendu;

	std::string encodage;

	switch(encodage_caract)
	{
		case EJeuxDeCaracteres::ISO_8859_1: encodage = "iso-8859-1";
		case EJeuxDeCaracteres::UTF8: 
		default: encodage = "utf-8"; break;
	}

	try { TypeContenu(TypeContenu()+"; charset="+encodage); }
	catch(...) {}

	for (AssocStrings::const_iterator it = liste_reponses.begin(); it != liste_reponses.end(); ++it)
		rendu += it->first + ": " + it->second + '\n';

	// Cookies
	std::string rendu_cookie = cookie.genererRendu();
	if (!rendu_cookie.empty())
		rendu += rendu_cookie + '\n';

	rendu += '\n';

	genere = true;

	return rendu;
}
