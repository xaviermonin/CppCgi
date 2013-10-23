#ifndef _CGI_MYSQL_H_
#define _CGI_MYSQL_H_

#include <sstream>

#include "Macros.h"
#include "Sql.h"

#ifdef __WINDOWS__
	#include <windows.h>
	#include <mysql.h>
#else
	#include <mysql/mysql.h>
#endif

namespace CppCgi {

class MySql : public Sql
{
public:
	MySql() throw (ExceptionSqlInitialisation);
	~MySql();

	virtual void seConnecter(const std::string& hote, const std::string& utilisateur, const std::string& mot_de_passe,
					 const std::string& base, unsigned int port=0) throw (ExceptionSqlConnexion);
	virtual void seDeconnecter();
	virtual void requete(const std::string& requete) throw (ExceptionSqlNonConnecte, ExceptionSqlRequeteEchouee);
	virtual unsigned long idInsere() throw (ExceptionSqlNonConnecte);

	virtual bool extraireLigne(CgiSqlLigneNumero& ligne_string) throw (ExceptionSqlNonConnecte, ExceptionSqlAucunResultat);
	virtual bool extraireLigne(CgiSqlLigneNom& ligne_string) throw (ExceptionSqlNonConnecte, ExceptionSqlAucunResultat);
private:
	MYSQL mysql;
	MYSQL_RES *result;
	MYSQL_ROW row;
};

} // namespace

#endif

