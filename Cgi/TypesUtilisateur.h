#ifndef _TYPES_UTILISATEURS_H_
#define _TYPES_UTILISATEURS_H_

#include <map>
#include <string>
#include <vector>

typedef std::map<std::string, std::string> AssocStrings;
typedef std::vector<std::string> ListeString;
typedef std::pair<std::string, unsigned short> PaireClassement;
typedef std::vector< PaireClassement > ListeClassee;

#endif
