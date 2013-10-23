#include "Localisation.h"
#include "Exception.h"
#include "Macros.h"

#ifdef __WINDOWS__
#include <windows.h>
#else
#include "Outils.h"
#include <algorithm>
#endif

using namespace CppCgi;

std::locale Localisation::generer(const std::string& nom)
{
	std::locale retour;

	#ifdef __WINDOWS__
	
	std::wstring wresultat(255, NULL);

	std::wstring wentree(nom.length(), NULL);
	std::copy(nom.begin(), nom.end(), wentree.begin());

	if (GetLocaleInfoEx(wentree.c_str(), LOCALE_SABBREVCTRYNAME, (LPWSTR)wresultat.c_str(), 255) == 0)
		throw ExceptionIntrouvable("Locale introuvable", INFOS_EXCEPTION);

	std::string resultat(wresultat.size(), NULL);
	std::copy(wresultat.begin(), wresultat.end(), resultat.begin());

    try { retour = std::locale(resultat.c_str()); }
    catch(...) {
		throw ExceptionIntrouvable("Locale introuvable", INFOS_EXCEPTION);
	}

	#else

	std::string resultat = Outils::remplacer(nom, "-", "_");
	ListeString liste = Outils::extraire(resultat, "_");
	if (liste.size() == 2)
	{
		std::transform(liste[0].begin(), liste[0].end(), liste[0].begin(), ::tolower);
		std::transform(liste[1].begin(), liste[1].end(), liste[1].begin(), ::toupper);
		resultat = liste[0] + "_" + liste[1];
	}

	try { retour = std::locale(resultat.c_str()); }
	catch(...) {
		throw ExceptionIntrouvable("Locale introuvable", INFOS_EXCEPTION);
	}

	#endif

	return retour;
}
