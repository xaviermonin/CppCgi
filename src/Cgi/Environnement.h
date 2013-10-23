/*!
* \file Environnement.h
* Fichier entête des classes Environnement/VariableEnvironnement.
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

#ifndef _ENVIRONNEMENT_H_
#define _ENVIRONNEMENT_H_

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Exception.h"

#if defined (__WINDOWS__)
#include <windows.h>
#endif

#define INFOS_ERREUR std::string("Fichier: ") + __FILE__ + " Ligne: " /*+ Convertire<std::string>(__LINE__)*/
#define ERREUR_FATALE std::string("<Erreur Fatale> ") + INFOS_ERREUR + '.'
#define ERREUR_IMPORTANTE std::string("<Erreur Importante> ") + INFOS_ERREUR + '.'
#define ERREUR_MINEURE std::string("<Erreur Mineure> ") + INFOS_ERREUR + '.'
#define ERREUR_DESTRUCTEUR std::string("<Erreur Destructeur> ") + INFOS_ERREUR

namespace CppCgi {

class StreamBuf : public std::streambuf
{
public:
	StreamBuf(int taille_buffer);
	virtual ~StreamBuf();

	virtual void ecrire(const std::string& chaine) = 0;
private:
	virtual int overflow(int c);
	virtual int sync();
};

class DebugStreamBuf : public StreamBuf
{
public:
	DebugStreamBuf(const std::string& fichier = "debug.log");
	~DebugStreamBuf();

	virtual void ecrire(const std::string& chaine);
private:
	std::ofstream fichier;
	std::string nom_fichier;
};

/*!
* \class Environnement
* \brief Classe de base d'accès aux données passées par le serveur HTTP.
*/
class Environnement
{
public:
	Environnement();

	// Raccourcis
	static std::ostream& Sortie();
	static std::istream& Entree();
	static std::ostream& Debug();
	static bool VarEnv(const std::string& variable, std::string& valeur);

	static bool Instancie();
	static Environnement* Instance();
	
	static void liberer();

	// Procédures
	virtual void preExecution()=0;
	virtual void postExecution()=0;
	virtual bool nouveauClient()=0;
	virtual void preClient()=0;
	virtual void postClient()=0;

	virtual void quitter()=0;

protected:
	virtual ~Environnement();

	// E/S
	virtual std::ostream& obtSortie()=0;
	virtual std::istream& obtEntree()=0;
	virtual std::ostream& obtDebug();
	virtual bool obtVarEnv(const std::string& variable, std::string& valeur)=0;

private:
	DebugStreamBuf debug_stream_buf;
	static Environnement* instance;
};

} // namespace

#endif
