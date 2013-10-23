#include "CgiMySql.h"
#include "ReponseHttp.h"

using namespace CppCgi;

MySql::MySql() throw (ExceptionSqlInitialisation)
{
	result = NULL;
	row = NULL;

	if (mysql_init(&mysql) == NULL)
		throw ExceptionSqlInitialisation("MySql: Initialisation échouée", INFOS_EXCEPTION);

	mysql_options(&mysql, MYSQL_READ_DEFAULT_GROUP, "option");
}

MySql::~MySql()
{
	if (result!=NULL)
		mysql_free_result(result);
	if (connexion)
		mysql_close(&mysql);
}

void MySql::seConnecter(const std::string& hote, const std::string& utilisateur, const std::string& mot_de_passe,
					 const std::string& base, unsigned int port) throw (ExceptionSqlConnexion)
{
	if(mysql_real_connect(&mysql, hote.c_str(), utilisateur.c_str(),
						  mot_de_passe.c_str(), base.c_str(), port, NULL,0) == 0)
		throw ExceptionSqlConnexion("MySQL: Connexion échouée", INFOS_EXCEPTION);

	connexion = true;

	if (ReponseHttp::Instance().EncodageCaractere() == EJeuxDeCaracteres::UTF8)
		requete("SET NAMES 'utf8'");
}

void MySql::seDeconnecter()
{
	if (result != NULL)
	{
		mysql_free_result(result);
		result = NULL;
	}
	mysql_close(&mysql);
	connexion = false;
}

unsigned long MySql::idInsere() throw (ExceptionSqlNonConnecte)
{
	if (!connexion)
		throw ExceptionSqlNonConnecte("MySQL: Connexion non effectuée.", INFOS_EXCEPTION);

	return mysql_insert_id(&mysql);
}

void MySql::requete(const std::string& requete) throw (ExceptionSqlNonConnecte, ExceptionSqlRequeteEchouee)
{
	if (result != NULL)
	{
		mysql_free_result(result);
		result = NULL;
	}

	if (!connexion)
		throw ExceptionSqlNonConnecte("MySQL: Connexion non effectuée.", INFOS_EXCEPTION);

	if (mysql_real_query(&mysql, requete.c_str(), requete.size())!=0)
		throw ExceptionSqlRequeteEchouee("MySQL: Requête '"+requete+"' échouée.", INFOS_EXCEPTION);

	result = mysql_use_result(&mysql);

	if (mysql_errno(&mysql)!=0)
		throw ExceptionSqlRequeteEchouee("MySQL: L'erreur suivante c'est produite: ", INFOS_EXCEPTION);
}

bool MySql::extraireLigne(CgiSqlLigneNumero& ligne_string) throw (ExceptionSqlNonConnecte, ExceptionSqlAucunResultat)
{
	if (!connexion)
		throw ExceptionSqlNonConnecte("MySQL: Connexion non effectuée.", INFOS_EXCEPTION);

	if (result == NULL)
		throw ExceptionSqlAucunResultat("MySQL: Aucun résultat à exploiter", INFOS_EXCEPTION);
	
	if ((row = mysql_fetch_row(result)) == NULL)
	{
		mysql_free_result(result);
		result = NULL;
		return false;
	}

	ligne_string.clear();

	unsigned int nb_champs = mysql_num_fields(result);
	
	if (nb_champs == 0)
	{
		mysql_free_result(result);
		row = NULL;
		result = NULL;
		throw ExceptionSqlAucunResultat("MySQL: Aucun champ dans la table", INFOS_EXCEPTION);
	}

	unsigned long *longueur;
	longueur = mysql_fetch_lengths(result);

	for (unsigned int i=0; i<nb_champs; ++i)
	{
		if (row[i] == NULL)
			ligne_string.push_back( ChampSql() );
		else
		{
			std::string parametre = row[i];
			ligne_string.push_back( ChampSql(parametre) );
		}
	}

	return true;
}

bool MySql::extraireLigne(CgiSqlLigneNom& ligne_string) throw (ExceptionSqlNonConnecte, ExceptionSqlAucunResultat)
{
	if (!connexion)
		throw ExceptionSqlNonConnecte("MySQL: Connexion non effectuée.", INFOS_EXCEPTION);

	if (result == NULL)
		throw ExceptionSqlAucunResultat("MySQL: Aucun résultat à exploiter", INFOS_EXCEPTION);

	ligne_string.clear();
	
	if ((row = mysql_fetch_row(result)) == NULL)
	{
		mysql_free_result(result);
		result = NULL;
		return false;
	}

	unsigned int nb_champs = mysql_num_fields(result);
	
	if (nb_champs == 0)
	{
		mysql_free_result(result);
		row = NULL;
		result = NULL;
		throw ExceptionSqlAucunResultat("MySQL: Aucun champ dans la table", INFOS_EXCEPTION);
	}

	for (unsigned int i=0; i<nb_champs; ++i)
	{
		MYSQL_FIELD *champ = mysql_fetch_field(result);
		if (champ == NULL)
			throw ExceptionSqlAucunResultat("MySQL: Aucun champ dans la table", INFOS_EXCEPTION);
		if (row[i] == NULL)
			ligne_string.insert( std::make_pair(champ->name, ChampSql()) );
		else
		{
			std::string parametre = row[i];
			ligne_string.insert( std::make_pair(champ->name, ChampSql(parametre)) );
		}
	}

	return true;
}
