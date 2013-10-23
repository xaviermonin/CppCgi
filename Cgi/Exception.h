/*!
* \file Exception.h
* Fichier entête de la classe Exception.
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

//! \todo Implémenter un système permettant de connaitre les appels effectués lors de l'exception

#ifndef _CGI_EXCEPTIONS_H_
#define _CGI_EXCEPTIONS_H_

#include <exception>
#include <string>
#include <sstream>
#include <typeinfo>

#ifdef _MSC_VER
#pragma warning( disable : 4290 ) // Warning de VS sur les spécifications d'exceptions ignorées
#endif

#define INFOS_EXCEPTION __FILE__, __LINE__

namespace CppCgi {

/*!
* \class Exception
* \brief Classe d'exception.
*/
class Exception: public std::exception
{
public:
	/*!
	* \brief Constructeur d'Exception.
	* \param[in] description_erreur : Description de l'erreur.
	* \param[in] nom_fichier : Nom du fichier levant l'exception.
	* \param[in] num_ligne : Numéro de la ligne levant l'exception.
	*/
	Exception(const std::string& description_erreur,
			std::string nom_fichier, int num_ligne) :
		description(description_erreur), fichier(nom_fichier), ligne(num_ligne)
	{
		std::ostringstream ligne_to_string;
		ligne_to_string << ligne;
		details = "Exception \"";
		details += description;
		details += "\" levée à la ligne ";
		details += ligne_to_string.str();
		details += " du fichier ";
		details += fichier;
	}

	virtual ~Exception() throw()
	{
	}

	/*!
	* \brief Renvoi le nom du fichier ayant levé l'exception.
	* \return : Nom du fichier.
	*/
	inline const char* fichierErreur() const
	{
		return fichier.c_str();
	}

	/*!
	* \brief Renvoi la description de l'exception.
	* \return : Description de l'erreur.
	*/
	inline const char* descriptionErreur() const
	{
		return description.c_str();
	}

	/*!
	* \brief Renvoi le numéro de ligne du fichier ayant levé l'exception.
	* \return : Numéro de la ligne.
	*/
	inline const int ligneErreur() const
	{
		return ligne;
	}

	/*!
	* \brief Renvoi les détails de l'exception.
	* \return : Détails de l'exception.
	*/
	virtual const char * what() const throw()
	{
		if (description_complete.empty())
		{
			description_complete += "Type d'exception: ";
			description_complete += typeid(*this).name();
			description_complete += ". <br />";
			description_complete += details;
		}

		return description_complete.c_str();
	}

protected:
	//! \brief Détails de l'erreur.
	std::string details;
	//! \brief Description de l'exception.
	std::string description;
	//! \brief Fichier levant l'exception.
	std::string fichier;
	//! \brief Numéro de ligne ayant levé l'exception.
	int ligne;

	mutable std::string description_complete;
};

class ExceptionIntrouvable : public Exception {
public: ExceptionIntrouvable(const std::string& description_erreur,
									  std::string nom_fichier, int num_ligne)
		: Exception(description_erreur, nom_fichier, num_ligne)
		{}
};

class ExceptionOperationIllegal : public Exception {
public: ExceptionOperationIllegal(const std::string& description_erreur,
									  std::string nom_fichier, int num_ligne)
		: Exception(description_erreur, nom_fichier, num_ligne)
		{}
};

} // namespace

#endif /* _EXCEPTIONS_H_ */
