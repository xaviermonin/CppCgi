#ifndef _ENVIRONNEMENT_FAST_CGI_H_
#define _ENVIRONNEMENT_FAST_CGI_H_

#define FAST_CGI

#include "Environnement.h"

#ifdef FAST_CGI

#include <fcgio.h>
#ifdef _MSC_VER
#pragma comment(lib, "libfcgi.lib")
#endif // _MSC_VER

namespace CppCgi {

class EnvironnementFastCgi : public Environnement
{
public:
	EnvironnementFastCgi(){}
	virtual ~EnvironnementFastCgi(){}

	// Procédures
	virtual void preExecution()
	{
		FCGX_Init();
		FCGX_InitRequest(&requete, 0, 0);
	}

	virtual void postExecution(){}
	virtual bool nouveauClient() { return (FCGX_Accept_r(&requete) == 0); }
	virtual void preClient()
	{
		cin_fcgi_streambuf.attach(requete.in);
		cout_fcgi_streambuf.attach(requete.out);
		cerr_fcgi_streambuf.attach(requete.err);

		std::cin.rdbuf(&cin_fcgi_streambuf);
		std::cout.rdbuf(&cout_fcgi_streambuf);
		std::cerr.rdbuf(&cerr_fcgi_streambuf);
	}
	virtual void postClient(){}

	virtual void quitter()
	{
		FCGX_SetExitStatus(0, requete.out);
	}

protected:
	// E/S
	virtual std::ostream& obtSortie() { return std::cout; }
	virtual std::istream& obtEntree() { return std::cin; }
	virtual bool obtVarEnv(const std::string& variable, std::string& valeur)
	{
		char* env = FCGX_GetParam(variable.c_str(), requete.envp);
		if (env == NULL)
			return false;
		else
		{
			valeur = env;
			return true;
		}
	}

private:
	FCGX_Request requete;
	fcgi_streambuf cin_fcgi_streambuf;
	fcgi_streambuf cout_fcgi_streambuf;
	fcgi_streambuf cerr_fcgi_streambuf;
};

} // namespace

#endif // FAST_CGI

#endif
