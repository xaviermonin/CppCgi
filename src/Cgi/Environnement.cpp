/*!
* \file Environnement.cpp
* Fichier source des classes Environnement/VariableEnvironnement.
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

#include "Environnement.h"
#include "Exception.h"
#include "DateHeure.h"

using namespace CppCgi;

// Classe StreamBuf

StreamBuf::StreamBuf(int taille_buffer)
{
	if (taille_buffer > 0)
	{
		char* p = new char[taille_buffer];
		setp(p, p + taille_buffer);
	}
	else
		setp(0, 0);
}

StreamBuf::~StreamBuf()
{
	sync();
	delete[] pbase();
}

int StreamBuf::overflow(int c)
{
	sync();

	if (c != EOF)
	{
		if (pbase() == epptr())
		{
			std::string tmp;
			tmp += (char)c;
			ecrire(tmp);
		}
		else
			sputc(c);
	}

	return 0;
}

int StreamBuf::sync()
{
	if (pbase() != pptr())
	{
		int taille = (int)(pptr() - pbase());
		std::string tmp(pbase(), taille);
		ecrire(tmp);
		setp(pbase(), epptr());
	}
	return 0;
}

// Classe DebugStreamBuf

DebugStreamBuf::DebugStreamBuf(const std::string& fichier) : StreamBuf(255)
{
	nom_fichier = fichier;
}

DebugStreamBuf::~DebugStreamBuf()
{
	if (fichier.is_open())
	{
		fichier << "[FERMETURE: Session de debugage à " << DateHeure::Maintenant().versDateString() << "]\n";
		fichier.close();
	}
}

void DebugStreamBuf::ecrire(const std::string &chaine)
{
#ifdef __WINDOWS__
	OutputDebugStringA(chaine.c_str());
#endif

	if (!fichier.is_open() && !nom_fichier.empty())
	{
		fichier.open(nom_fichier.c_str(), std::ios_base::app);
		if (!fichier.is_open())
			return;

		fichier << "[OUVERTURE: Session de debugage à " << DateHeure::Maintenant().versDateString() << "]\n";
	}

	fichier << chaine;
}

// Classe Environnement.

Environnement::Environnement()
{
	if (instance != NULL)
		throw Exception("Une instance d'environnement existe déja.", INFOS_EXCEPTION);

	instance = this;

	std::clog.rdbuf(&debug_stream_buf);
}

Environnement::~Environnement()
{
}

void Environnement::liberer()
{
	delete instance;
	instance = NULL;
}

/*!
* \brief Obtient le flux de sortie.
* \return Flux de sortie.
*/
std::ostream& Environnement::Sortie()
{
	if (instance == NULL)
		throw Exception("Aucune instance d'environnement n'existe.", INFOS_EXCEPTION);

	return instance->obtSortie();
}

/*!
* \brief Obtient le flux de sortie.
* \return Flux de sortie.
*/
std::ostream& Environnement::Debug()
{
	if (instance == NULL)
		throw Exception("Aucune instance d'environnement n'existe.", INFOS_EXCEPTION);

	return instance->obtDebug();
}

/*!
* \brief Obtient le flux d'entrée.
* \return Flux d'entrée.
*/
std::istream& Environnement::Entree()
{
	if (instance == NULL)
		throw Exception("Aucune instance d'environnement n'existe.", INFOS_EXCEPTION);

	return instance->obtEntree();
}

/*!
* \brief Obtient la valeur de la variable d'environnement.
* \param[in] variable : Nom de la variable d'environnement à obtenir.
* \return Valeur de la variable d'environnement.
*/
bool Environnement::VarEnv(const std::string& variable, std::string& valeur)
{
	if (instance == NULL)
		throw Exception("Aucune instance d'environnement n'existe.", INFOS_EXCEPTION);

	return instance->obtVarEnv(variable, valeur);
}

std::ostream& Environnement::obtDebug()
{
	return std::clog;
}

bool Environnement::Instancie()
{
	return (instance != NULL);
}

Environnement* Environnement::Instance()
{
	if (instance == NULL)
		throw ExceptionIntrouvable("L'instance d'environnement n'existe pas.", INFOS_EXCEPTION);

	return instance;
}

Environnement* Environnement::instance = NULL;
