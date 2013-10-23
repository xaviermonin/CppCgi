/*!
* \file Outils.h
* Fichier entête de la classe Outils.
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

#ifndef _CGI_OUTILS_H_
#define _CGI_OUTILS_H_

#include "TypesUtilisateur.h"
#include "Exception.h"

namespace CppCgi {

/*!
* \class Outils
* \brief Liste d'outils mis à disposition tel que l'extraction dans une chaine.
*/
class Outils
{
public:
	static bool stringsEquivalentes(const std::string& premier, const std::string& deuxieme);
	static bool stringsEquivalentes(const std::string& premier, const std::string& deuxieme, size_t limite);
	
	static std::string extraireEntre(const std::string& source, const std::string& extremite_gauche,
									 const std::string& extremite_droite);

	static ListeString extraire(const std::string& source, const std::string& separateur);

	static std::string applatirListeString(const ListeString& liste_string, const std::string& separateur);

	static std::string decoderUrl(const std::string& contenu_code);
	static std::string encoderUrl(const std::string& contenu);

	static std::string ajouterSlash(const std::string& chaine);
	static std::string retirerSlash(const std::string& chaine);

	static std::string remplacer(const std::string& texte, const std::string& element_recherche,
								 const std::string element_remplaceant);
	static std::string supprimerEspacesBlancs(const std::string& texte);

	static AssocStrings scinderEnClefsValeurs(const std::string& entree, const std::string& separateur1,
													const std::string& separateur2);
	static AssocStrings scinderEtDecoderEnClefsValeurs(const std::string& entree, const std::string& separateur1,
													const std::string& separateur2);
	static std::string applatirAssocStrings(const AssocStrings& liste, const std::string& separateur1,
												const std::string& separateur2);
};

} // namespace

#endif
