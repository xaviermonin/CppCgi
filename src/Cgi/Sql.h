#ifndef _CGI_SQL_H_
#define _CGI_SQL_H_

#include <string>
#include <vector>
#include <map>
#include "ChampSql.h"
#include "Exception.h"

namespace CppCgi {

typedef std::vector<ChampSql> CgiSqlLigneNumero;
typedef std::map<std::string, ChampSql> CgiSqlLigneNom;

class ExceptionSqlNonConnecte : public Exception {
public: ExceptionSqlNonConnecte(const std::string& description_erreur,
									  std::string nom_fichier, int num_ligne)
		: Exception(description_erreur, nom_fichier, num_ligne)
		{}
};

class ExceptionSqlInitialisation : public Exception {
public: ExceptionSqlInitialisation(const std::string& description_erreur,
									  std::string nom_fichier, int num_ligne)
		: Exception(description_erreur, nom_fichier, num_ligne)
		{}
};

class ExceptionSqlConnexion : public Exception {
public: ExceptionSqlConnexion(const std::string& description_erreur,
									  std::string nom_fichier, int num_ligne)
		: Exception(description_erreur, nom_fichier, num_ligne)
		{}
};

class ExceptionSqlAucunResultat : public Exception {
public: ExceptionSqlAucunResultat(const std::string& description_erreur,
									  std::string nom_fichier, int num_ligne)
		: Exception(description_erreur, nom_fichier, num_ligne)
		{}
};

class CgiExceptionSqlAucunChamp : public Exception {
public: CgiExceptionSqlAucunChamp(const std::string& description_erreur,
									  std::string nom_fichier, int num_ligne)
		: Exception(description_erreur, nom_fichier, num_ligne)
		{}
};

class ExceptionSqlRequeteEchouee : public Exception {
public: ExceptionSqlRequeteEchouee(const std::string& description_erreur,
									  std::string nom_fichier, int num_ligne)
		: Exception(description_erreur, nom_fichier, num_ligne)
		{}
};

class Sql
{
public:
	Sql() { connexion = false; }
	virtual ~Sql(){};

	virtual void seConnecter(const std::string& hote, const std::string& utilisateur, const std::string& mot_de_passe,
					 const std::string& base, unsigned int port=0)=0;
	virtual void seDeconnecter()=0;
	virtual void requete(const std::string& requete)=0;
	virtual unsigned long idInsere()=0;
	virtual bool extraireLigne(CgiSqlLigneNumero& ligne_string)=0;
	virtual bool extraireLigne(CgiSqlLigneNom& ligne_string)=0;

	inline bool etatConnexion() { return connexion; }
protected:
	bool connexion;
};

} // namespace

#endif
