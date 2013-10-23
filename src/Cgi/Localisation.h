#ifndef _LOCALISATION_H_
#define _LOCALISATION_H_

#include <locale>
#include <string>

namespace CppCgi {

class Localisation
{
public:
	static std::locale generer(const std::string& nom);
};

}

#endif
