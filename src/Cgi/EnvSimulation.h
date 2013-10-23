#ifndef _ENVIRONNEMENT_SIMULATION_H_
#define _ENVIRONNEMENT_SIMULATION_H_

#include "EnvironnementCgi.h"
#include <fstream>
#include "Outils.h"

namespace CppCgi {

class EnvSimulation : public EnvironnementCgi
{
public:
	EnvSimulation();
	virtual ~EnvSimulation();

	void FichierVarEnv(const std::string& fichier);
	const std::string& FichierVarEnv() const;

	void FichierEntree(const std::string& fichier);
	const std::string& FichierEntree() const;

protected:
	virtual std::istream& obtEntree();
	virtual bool obtVarEnv(const std::string& variable, std::string& valeur);

private:
	std::ifstream entree;
	std::ifstream var_env;
	AssocStrings liste_vars;

	std::string fichier_var_env;
	std::string fichier_entree;
};

} // namespace

#endif
