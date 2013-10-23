/*!
* \file Post.h
* Fichier entête de la classe Post.
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

#ifndef _CGI_POST_H_
#define _CGI_POST_H_

#include "Exception.h"
#include "Outils.h"
#include "Modeles/Singleton.h"
#include <string>
#include <vector>

namespace CppCgi {

/*!
* \struct PostFichier
* \brief Structure représentant un fichier envoyé par méthode post.
*/
typedef struct
{
	//! \brief Données du fichier envoyé.
	std::string donnees;
	//! \brief Nom du fichier envoyé (nom coté client).
	std::string nom_fichier;
	//! \brief Nom de l'élément post.
	std::string nom_element;
	//! \brief Type Mime du fichier.
	std::string type_mime;
} PostFichier;

/*!
* \class Post
* \brief Classe d'accès aux paramètres Post.
* Les valeurs passées par méthode post sont analysées et stockées dans cette classe.
* Les données sont extraites à partir du flux d'entrée de Environnement::Entree. (std::cout par défaut).
* Il s'agit d'une classe singleton.
*/
class Post : public Modele::Singleton<Post>
{
	friend class Modele::Singleton<Post>;
public:
	const PostFichier& obtFichier(const std::string& nom_element);
	const std::vector<PostFichier>& obtListeFichiers();

	bool elementExistant(const std::string& clef);
	bool fichierExistant(const std::string& nom);
	std::string operator [](const std::string& clef);
	const std::string& Valeur(const std::string& clef);
	const AssocStrings& ListeElements();

	static void definirTailleMaximumPost(size_t taille_max);
	static void definirTailleMaximumFichier(size_t taille_max);

private:
	Post();
	//! \brief Destructeur de Post.
	virtual ~Post() {}

	void analyserMultiPart(const std::string& donnees);

	//! \brief Liste des paramètres post envoyé par le navigateur client (sauf fichiers).
	AssocStrings liste_post;
	//! \brief Liste des fichiers envoyés par le navigateur.
	std::vector<PostFichier> liste_fichiers;

	//! \brief Limite de la taille d'un fichier reçu. (2 Mo par défaut).
	static size_t taille_max_fichier;
	//! \brief Limite de la taille des données reçu. (4 Mo par défaut).
	static size_t taille_max_post;
};

} // namespace

#endif
