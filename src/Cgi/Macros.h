#ifndef _MACROS_H_
#define _MACROS_H_

#if defined (WIN32) || defined (_WIN32)
#define __WINDOWS__
#endif

#if defined(_DEBUG) || defined(DEBUG)
	#define __DEBUGAGE__
#endif

#define NOM_CLASSE(classe) std::string(typeid(classe).name())
#define NOM_OBJET(objet) std::string(typeid(objet).name())

#endif
