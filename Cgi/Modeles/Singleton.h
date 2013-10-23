#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <string>

namespace CppCgi { namespace Modele {

template <class Classe>
class Singleton
{
public:
	static bool instanceExistante()
	{
		return (instance != NULL);
	}

	static Classe& Instance()
	{
		if (instance == NULL)
			instance = new Classe;

		return *instance;
	}

	static void libererInstance()
	{
		delete instance;
		instance = NULL;
	}

protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static Classe* instance;

	Singleton(const Singleton&);
	void operator =(const Singleton&);
};

template <class Classe> Classe* Singleton<Classe>::instance = NULL;

} }

#endif
