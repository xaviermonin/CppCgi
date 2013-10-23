#ifndef _CHAMP_SQL_
#define _CHAMP_SQL_

#include <string>
#include "Convertisseur.h"

namespace CppCgi {

class ChampSql
{
public:
	ChampSql(const std::string& type_entree) : donnee(type_entree) { champ_null = false; }
	ChampSql() { champ_null = true; }

	virtual ~ChampSql(){};

	template<typename T_S> operator T_S()
	{
		T_S type_sortie;

		std::istringstream iss(donnee);
		iss >> type_sortie;

		if (iss.fail())
			throw Exception("Conversion de std::string ( contenant: " + donnee +  " ) en " + std::string(typeid(type_sortie).name()) + " impossible.", INFOS_EXCEPTION);

		return type_sortie;
	}

	operator const char*()
	{
		return donnee.c_str();
	}

	bool estNull() { return champ_null; };

private:
	operator std::string()
	{
		return "";
	}

	std::string donnee;
	bool champ_null;
};

} // namespace

#endif
