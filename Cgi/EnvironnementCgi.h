#ifndef _ENVIRONNEMENT_CGI_H_
#define _ENVIRONNEMENT_CGI_H_

#include "Environnement.h"
#include <cstdlib>

namespace CppCgi {

class EnvironnementCgi : public Environnement
{
public:
	EnvironnementCgi();
	virtual ~EnvironnementCgi();

	// Procédures
	virtual void preExecution(){};
	virtual void postExecution(){};
	virtual bool nouveauClient();
	virtual void preClient(){};
	virtual void postClient(){};

	virtual void quitter();

protected:
	// E/S
	virtual std::ostream& obtSortie();
	virtual std::istream& obtEntree();
	virtual bool obtVarEnv(const std::string& variable, std::string& valeur);

private:
	bool premier_tour;
};

} // namespace

#endif
