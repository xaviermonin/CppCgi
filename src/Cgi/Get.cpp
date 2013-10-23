/*!
* \file Get.cpp
* Fichier source de la classe Get.
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

#include "Get.h"
#include "Serveur.h"

using namespace CppCgi;

/*!
* \brief Constructeur de Get appelé par Instance().
*/
Get::Get()
{
	try {
		std::string chaine = Serveur::ParametresUrl();
		chaine = Outils::decoderUrl(chaine);
		liste_get = Outils::scinderEtDecoderEnClefsValeurs(chaine, "=", "&");
	}
	catch(const Exception&) {
		throw ExceptionGetObtention("Obtention des paramètres GET impossible.", INFOS_EXCEPTION);
	}
}

/*!
* \brief Vérifie l'existance d'une valeur get.
* \param[in] clef : Nom de la clef du paramètre get.
* \return Si la clef existe: true sinon false.
*/
bool Get::elementExistant(const std::string& clef)
{
	if (liste_get.find(clef) == liste_get.end())
		return false;
	else
		return true;
}

/*!
* \brief Obtient une valeur get.
* \param[in] clef : Nom de la clef du paramètre get.
* \return La valeur du paramètre get.
*/
const std::string& Get::Valeur(const std::string& clef) throw (ExceptionIntrouvable)
{
	if (liste_get.find(clef) == liste_get.end())
		throw ExceptionIntrouvable("Element inexistant", INFOS_EXCEPTION);
	else
		return liste_get[clef];
}

/*!
* \brief Obtient la liste des valeurs get reçues.
* \return La liste des valeurs get.
*/
const AssocStrings& Get::ListeElements()
{
	return liste_get;
}

/*!
* \brief Obtient une valeur get.
* \param[in] clef : Nom de la clef du paramètre get.
* \return La valeur du paramètre get.
*/
const std::string& Get::operator [](const std::string& clef) throw (ExceptionIntrouvable)
{
	return Valeur(clef);
}
