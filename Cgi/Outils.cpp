/*!
* \file Outils.cpp
* Fichier source de la classe Outils.
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

#include "Outils.h"
#include "Exception.h"
#include <algorithm>
#include <functional>
#include <cctype>

using namespace CppCgi;

/*!
* \brief Indique si une string est équivalente (insensible à la casse) à une autre.
* \param[in] premier : Première string à comparer.
* \param[in] deuxieme : Deuxième string à comparer.
* \return true si les chaines sont équivalentes sinon false.
*/
bool Outils::stringsEquivalentes(const std::string& premier, const std::string& deuxieme)
{
	std::string::const_iterator p1 = premier.begin();
	std::string::const_iterator p2 = deuxieme.begin();
	std::string::const_iterator l1 = premier.end();
	std::string::const_iterator l2 = deuxieme.end();

	while(p1 != l1 && p2 != l2)
	{
		if(std::tolower(*(p1++)) != std::tolower(*(p2++)))
			return false;
	}
	return (deuxieme.size() == premier.size()) ? true : false;
}

/*!
* \brief Indique si une string est équivalente (insensible à la casse) à n caractères.
* La comparaison est effectuée jusqu'au nème caractère.
* \param[in] premier : Première string à comparer.
* \param[in] deuxieme : Deuxième string à comparer.
* \param[in] limite : Position de l'index maximum à comparer.
* \return true si les chaines sont équivalentes jusqu'à l'index n sinon false.
*/
bool Outils::stringsEquivalentes(const std::string& premier, const std::string& deuxieme, size_t limite)
{
	std::string::const_iterator p1 = premier.begin();
	std::string::const_iterator p2 = deuxieme.begin();
	bool ok = (limite <= premier.length() && limite <= deuxieme.length());
	std::string::const_iterator l1 = ok ? (premier.begin() + limite) : premier.end();
	std::string::const_iterator l2 = ok ? (deuxieme.begin() + limite) : deuxieme.end();
	while(p1 != l1 && p2 != l2)
	{
		if(std::tolower(*(p1++)) != std::tolower(*(p2++)))
			return false;
	}
	return ok;
}

/*!
* \brief Ajoute des anti-slash ( \ ) à une chaine.
* \param[in] chaine : Chaine de caractères nescessitant l'ajout d'anti-slash.
* \return La chaine avec des anti-slash.
*/
std::string Outils::ajouterSlash(const std::string& chaine)
{
	std::string retour;
	retour.reserve(chaine.size());
	for (size_t i=0; i<chaine.size(); ++i)
	{
		if (chaine[i] == '\'' || chaine[i] == '\"' || chaine[i] == '`')
			retour.push_back('\\');
		retour.push_back(chaine[i]);
	}
	return retour;
}

/*!
* \brief Enlève les anti-slash ( \ ) d'une chaine.
* \param[in] chaine : Chaine de caractères nescessitant la suppression d'anti-slash.
* \return La chaine sans anti-slash.
*/
std::string Outils::retirerSlash(const std::string& chaine)
{
	std::string retour;
	bool slash=false;
	retour.reserve(chaine.size());
	for (size_t i=0; i<chaine.size(); ++i)
	{
		if ( (chaine[i] == '\\') && (!slash) )
		{
			slash = !slash;
			continue;
		}
		slash = false;
		retour.push_back(chaine[i]);
	}
	return chaine;
}

/*!
* \brief Encode une chaine au format Url (ex: Le caractère espace devient %20).
* \param[in] contenu : Chaine de caractères à encoder.
* \return La chaine de caractères encode au format url.
*/
std::string Outils::encoderUrl(const std::string& contenu)
{
	std::string encode;
	for (unsigned int i=0; i<contenu.size(); ++i)
	{
		if ( (contenu[i]>='0' && contenu[i]<='9') ||
			 (contenu[i]>='A' && contenu[i]<='Z') ||
			 (contenu[i]>='a' && contenu[i]<='z') ||
			  contenu[i]=='-' || contenu[i]=='_'    )
		{
			// Pas besoin d'encoder les alphanumeriques et autres
			encode += contenu[i];
			continue;
		}

		// Encodage en hexadécimal
		std::ostringstream chaine_hexa;
		chaine_hexa << std::hex << ((unsigned int)contenu[i])%0xFF; // %0xFF permet de se limiter à 1 octet. % => cyclique

		encode += '%';
		encode += chaine_hexa.str();
	}
	return encode;
}

/*!
* \brief Décode une chaine au format Url (ex: %20 devient le caractère espace).
* \param[in] contenu_code : Chaine de caractères à décoder.
* \return La chaine de caractères décodé.
*/
std::string Outils::decoderUrl(const std::string& contenu_code)
{
	std::string decode;

	for (unsigned int i=0; i<contenu_code.size(); ++i)
	{
		if ( contenu_code[i] == '%' )
		{
			unsigned int caractere;
			if ( (i+2) > contenu_code.size() )
				throw Exception("Chaine non encodée en URL", INFOS_EXCEPTION);

			std::istringstream iss( contenu_code.substr(i+1, 2) );
			iss >> std::hex >> caractere;
			if (iss.fail())
				throw Exception("Chaine non encodée en URL", INFOS_EXCEPTION);

			decode += (unsigned char)caractere;

			i+=2;
			continue;
		}
		
		if ( contenu_code[i] == '+' )
		{
			decode += ' ';
			continue;
		}

		decode += contenu_code[i];
	}

	return decode;
}

/*!
* \brief Extrait la chaine de caractère située entre deux chaines.
* Exemple : Avec la chaine "Bonjour" et la borne gauche "Bo" et la borne droite "our" on obtient "nj".
* \param[in] source : Chaine de caractères à extraire.
* \param[in] extremite_gauche : Borne de gauche.
* \param[in] extremite_droite : Borne de droite.
* \return La chaine de caractères situé entre les deux bornes.
*/
std::string Outils::extraireEntre(const std::string& source, const std::string& extremite_gauche,
								  const std::string& extremite_droite)
{
	std::string::size_type debut = source.find(extremite_gauche);
	if (debut == std::string::npos)
		return "";
	debut += extremite_gauche.size();

	std::string::size_type fin = source.find(extremite_droite, debut);
	if (fin == std::string::npos)
		return "";
	return source.substr(debut, fin - debut);
}

/*!
* \brief Extrait sous forme de liste une chaine à l'aide d'un séparateur.
* Exemple : "BonjourETAurevoirETSalut" avec "ET" comme séparateur donne
* une liste: { "Bonjour", "Aurevoir", "Salut" }.
* \param[in] source : Chaine de caractères à extraire.
* \param[in] separateur : Séparateur.
* \return Liste de string.
*/
ListeString Outils::extraire(const std::string& source, const std::string& separateur)
{
	ListeString liste;
	std::string::size_type position;
	std::string::size_type debut=0;

	while ( (position = source.find(separateur, debut)) != std::string::npos )
	{
		liste.push_back(source.substr(debut, position - debut));
		debut = position + separateur.size();
	}

	liste.push_back(source.substr(debut));

	return liste;
}

std::string Outils::applatirListeString(const ListeString& liste, const std::string& separateur)
{
	std::string texte;

	for (size_t i=0; i<liste.size(); ++i)
	{
		texte += liste.at(i);
		if (i < liste.size() - 1)
			texte += separateur;
	}

	return texte;
}

/*!
* \brief Découpe une chaine en liste de clefs/valeurs (std::map).
* Exemple :  "Bonjour=Salut&Aurevoir=Ciao" avec = comme séparateur1 et & comme séparateur2
* donne { Bonjour=>Salut, Aurevoir=>Ciao }.
* \param[in] entree : Chaine de caractères à découper.
* \param[in] separateur1 : Séparateur entre clef et valeur.
* \param[in] separateur2 : Séparateur enter chaque clef/valeur.
* \return La liste de clefs/valeurs.
*/
AssocStrings Outils::scinderEnClefsValeurs(const std::string& entree,
												const std::string& separateur1,
												const std::string& separateur2)
{
	AssocStrings map_decoupe;
	std::string valeur;
	std::string clef;
	size_t position_separateur1 = 0;
	size_t position_separateur2 = 0;

	// Boucle tant qu'on trouve un séparateur 1.
	while( (position_separateur1 = entree.find(separateur1, position_separateur2)) != std::string::npos)
	{
		clef = entree.substr(position_separateur2, position_separateur1 - position_separateur2); // Extraction de la clef.
		position_separateur1 += separateur1.size();

		position_separateur2 = entree.find(separateur2, position_separateur1);

		if (position_separateur2 == std::string::npos)
			position_separateur2 = entree.size(); // Séparateur 2 introuvable, on se place à la fin pour tout extraire.

		valeur = entree.substr(position_separateur1, position_separateur2 - position_separateur1);
		position_separateur2 += separateur2.size();

		map_decoupe[clef] = valeur;
	}

	if (position_separateur2 < entree.size())
	{
		clef = entree.substr(position_separateur2, entree.size() - position_separateur2); // Extraction de la clef.
		map_decoupe[clef];
	}

	return map_decoupe;
}

/*!
* \brief Découpe une chaine en liste de clefs/valeurs (std::map) et effectue un décodage url.
* Exemple :  "Bonjour=Salut&Aurevoir=Ciao" avec = comme séparateur1 et & comme séparateur2
* donne { Bonjour=>Salut, Aurevoir=>Ciao }.
* \param[in] entree : Chaine de caractères à découper.
* \param[in] separateur1 : Séparateur entre clef et valeur.
* \param[in] separateur2 : Séparateur enter chaque clef/valeur.
* \return La liste de clefs/valeurs.
*/
AssocStrings Outils::scinderEtDecoderEnClefsValeurs(const std::string& entree,
												const std::string& separateur1,
												const std::string& separateur2)
{
	AssocStrings map_decoupe;
	std::string valeur;
	std::string clef;
	size_t position_separateur1 = 0;
	size_t position_separateur2 = 0;

	// Boucle tant qu'on trouve un séparateur 1.
	while( (position_separateur1 = entree.find(separateur1, position_separateur2)) != std::string::npos)
	{
		clef = Outils::decoderUrl(entree.substr(position_separateur2,
								  position_separateur1 - position_separateur2)); // Extraction de la clef.
		position_separateur1 += separateur1.size();

		position_separateur2 = entree.find(separateur2, position_separateur1);

		if (position_separateur2 == std::string::npos)
			position_separateur2 = entree.size(); // Séparateur 2 introuvable, on se place à la fin pour tout extraire.

		valeur = Outils::decoderUrl(entree.substr(position_separateur1,
									position_separateur2 - position_separateur1));
		position_separateur2 += separateur2.size();

		map_decoupe[clef] = valeur;
	}

	if (position_separateur2 < entree.size())
	{
		clef = Outils::decoderUrl(entree.substr(position_separateur2,
								  entree.size() - position_separateur2)); // Extraction de la clef.
		map_decoupe[clef];
	}

	return map_decoupe;
}

/*
* \brief Applati une association de chaine de caractères en une chaine de caractères.
* \param[in] liste : Association de strings à applatir.
* \param[in] separateur1 : Séparateur appliqué entre la clef et la valeur.
* \param[in] separateur2 : Séparateur appliqué entre 2 associations.
* \return Chaine de caractères applatie.
*/
std::string Outils::applatirAssocStrings(const AssocStrings& liste, const std::string& separateur1,
											 const std::string& separateur2)
{
	std::string liste_applatie;

	for (AssocStrings::const_iterator it = liste.begin(); it != liste.end(); ++it)
		liste_applatie += it->first + separateur1 + it->second + separateur2;

	return liste_applatie;
}

/*!
* \brief Remplace une sous-chaine dans une chaine de caractères.
* \param[in] texte : Chaine de caractères source.
* \param[in] element_recherche : Sous-chaine à remplacer.
* \param[in] element_remplaceant : Sous-chaine remplaceante.
* \return La chaine de caractères remplacée.
*/
std::string Outils::remplacer(const std::string& texte, const std::string& element_recherche,
								 const std::string element_remplaceant)
{
	std::string::size_type position;
	std::string::size_type ancienne_position = 0;
	std::string resultat;
	do
	{
		position = texte.find(element_recherche, ancienne_position);
		resultat += texte.substr(ancienne_position, position - ancienne_position);
		if (position != std::string::npos)
			resultat += element_remplaceant;
		ancienne_position = position + element_recherche.size();
	} while ( position != std::string::npos );

	return resultat;
}

/*
* \brief Supprime les espaces blancs: tabulation, espace, saut de ligne.
* \param[in] texte : Texte dont les espaces blancs seront retirés
* \return Chaine de caractère sans espace blanc.
*/
std::string Outils::supprimerEspacesBlancs(const std::string& texte)
{
	std::string vide;
	return remplacer(remplacer(remplacer(remplacer(texte, "\n", vide), "\r", vide), " ", vide), "\t", vide);
}
