/*!
* \file Post.cpp
* Fichier source de la classe Post.
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

#include "Post.h"
#include "Serveur.h"
#include "Client.h"
#include "Environnement.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <functional>
#include <vector>

using namespace CppCgi;

/*!
* \brief Constructeur de Post appelé par Instance().
* Parse les données reçues par méthode post.
*/
Post::Post()
{
	unsigned long longueur;
	std::string donnees;

	if (Client::MethodeRequete() != TypeRequete::POST)
		return;

	std::string content_length;
	try { Environnement::VarEnv("CONTENT_LENGTH", content_length); }
	catch (const Exception&) { throw Exception("Post: Taille des données POST absente", INFOS_EXCEPTION); }

	std::istringstream convertisseur(content_length);
	convertisseur >> longueur;
	if (convertisseur.fail()) throw Exception("Post: Taille des données POST erronée", INFOS_EXCEPTION);

	if (longueur > taille_max_post)
		throw Exception("Post: Taille des données POST trop grande.", INFOS_EXCEPTION);

	std::vector<char> tmp(longueur);
	std::cin.read(&tmp[0], longueur);
	donnees = std::string(&tmp[0], longueur);

	std::string content_type;

	try { Environnement::VarEnv("CONTENT_TYPE", content_type); } catch (...) {}

	// Partie Form-UrlEncoded
	std::string form_urlencoded = "application/x-www-form-urlencoded";
	if (Outils::stringsEquivalentes(content_type, form_urlencoded, form_urlencoded.size()) ||
		content_type.empty())
	{
		liste_post = Outils::scinderEtDecoderEnClefsValeurs(donnees, "=", "&");
		return;
	}
	
	// Partie MultiPart-FormData
	std::string multipart_form_data = "multipart/form-data;";
	if (Outils::stringsEquivalentes(content_type, multipart_form_data, multipart_form_data.size()))
	{
		// Extraction du boundary
		std::string tmp = "boundary=";
		std::string::size_type position = content_type.find(tmp);
		if (position == std::string::npos)
			throw Exception("Post: Boundary introuvable.", INFOS_EXCEPTION);

		std::string boundary = "--" + content_type.substr(position+tmp.size());
		std::string boundary_fin = boundary + "--\r\n";
		boundary += "\r\n";

		// Début de la lecture des données POST
		std::string::size_type debut_donnees = donnees.find(boundary)+boundary.size();
		// Dans les cas où il y a plus d'un élément
		while( (position = donnees.find(boundary, debut_donnees)) != std::string::npos )
		{
			analyserMultiPart(donnees.substr(debut_donnees, position - debut_donnees));
			debut_donnees = position + boundary.size();
		}
		// Extraction du dernier élément
		if ( (position = donnees.find(boundary_fin, debut_donnees)) != std::string::npos )
			analyserMultiPart(donnees.substr(debut_donnees, position - debut_donnees));
	}
}

/*!
* \brief Analyse et stock les fichier envoyés au serveur.
* \param[in] donnees : Code HTTP de la partie MultiPart.
*/
void Post::analyserMultiPart(const std::string& donnees)
{
	std::string fin_entete = "\r\n\r\n";
	std::string::size_type position;
	if ( (position = donnees.find(fin_entete)) == std::string::npos )
		throw Exception("Post: Extraction de l'entête d'une donnée MIME impossible", INFOS_EXCEPTION);

	std::string entete = donnees.substr(0, position);
	if ( ( donnees.size() - entete.size() ) > taille_max_fichier )
		throw Exception("Post: Taille du fichier trop grande.", INFOS_EXCEPTION);

	// Tout le reste moins les caracteres CRLF
	std::string corps = donnees.substr(position + fin_entete.size(), donnees.size() - entete.size() - fin_entete.size() - 2);

//	Outils::extraireEntre(entete, "Content-Disposition: ", ";");
	std::string nom_element = Outils::extraireEntre(entete, "name=\"", "\"");
	if (nom_element.empty())
		throw Exception("Post: Nom d'un élément incohérent.", INFOS_EXCEPTION);

	std::string nom_fichier = Outils::decoderUrl(Outils::extraireEntre(entete, "filename=\"", "\""));
	std::string type_mime = "text/plain";

	std::string content_type = "Content-Type: ";
	if ( (position = entete.find(content_type) ) != std::string::npos)
		type_mime = entete.substr(position + content_type.size());
	
	if (nom_fichier.empty())
		liste_post[nom_element] = corps;
	else
	{
		PostFichier fichier;
		fichier.donnees = corps;
		fichier.nom_element = nom_element;
		fichier.nom_fichier = nom_fichier;
		fichier.type_mime = type_mime;

		liste_fichiers.push_back(fichier);
	}
}

/*!
* \brief Obtient un fichier.
* \param[in] nom_element : Nom de la clef du fichier à obt.
* \return : Structure du fichier demandé.
*/
const PostFichier& Post::obtFichier(const std::string& nom_element)
{
	for (size_t i=0; i<liste_fichiers.size(); ++i)
		if (liste_fichiers[i].nom_element == nom_element)
			return liste_fichiers[i];
	throw Exception("Post: Fichier inexistant.", INFOS_EXCEPTION);
}

/*!
* \brief Obtient la liste des fichiers envoyés par le navigateur client.
* \return : Liste des structure de fichiers.
*/
const std::vector<PostFichier>& Post::obtListeFichiers()
{
	return liste_fichiers;
}

/*!
* \brief Définit la taille maximum de données reçu par méthode post.
* \param[in] taille_max : Taille maximum des données reçu par méthode post (en octets).
*/
void Post::definirTailleMaximumPost(size_t taille_max)
{
	taille_max_post = taille_max;
}

/*!
* \brief Définit la taille maximum d'un fichier reçu par méthode post.
* \param[in] taille_max : Taille maximum d'un fichier reçu par méthode post (en octets).
*/
void Post::definirTailleMaximumFichier(size_t taille_max)
{
	taille_max_fichier = taille_max;
}

/*!
* \brief Vérifie l'existance d'un fichier.
* \param[in] nom : Nom de la clef du fichier.
* \return Si la clef existe: true sinon false.
*/
bool Post::fichierExistant(const std::string& nom)
{
	for (size_t i=0; i<liste_fichiers.size(); ++i)
		if (liste_fichiers[i].nom_element == nom)
			return true;
	return false;
}

/*!
* \brief Obtient une valeur post.
* \param[in] clef : Nom de la clef du paramètre post.
* \return La valeur du paramètre post.
*/
const std::string& Post::Valeur(const std::string& clef)
{
	if (liste_post.find(clef) == liste_post.end())
		throw Exception("Element inexistant", INFOS_EXCEPTION);
	else
		return liste_post[clef];
}

/*!
* \brief Vérifie l'existance d'un paramètre post (pas d'un fichier).
* \param[in] clef : Nom de la clef du paramètre post.
* \return Si la clef existe: true sinon false.
*/
bool Post::elementExistant(const std::string& clef)
{
	if (liste_post.find(clef) == liste_post.end())
		return false;
	else
		return true;
}

/*!
* \brief Obtient la liste des valeurs post reçues.
* \return La liste des valeurs post.
*/
const AssocStrings& Post::ListeElements()
{
	return liste_post;
}

/*!
* \brief Obtient une valeur post.
* \param[in] clef : Nom de la clef du paramètre post.
* \return La valeur du paramètre post.
*/
std::string Post::operator [](const std::string& clef)
{
	return Valeur(clef);
}

size_t Post::taille_max_fichier = 2048*1024; // 2 Mo
size_t Post::taille_max_post = 4196*1024; // 4 Mo
