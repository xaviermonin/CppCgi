#include "DateHeure.h"
#include <ctime>
#include <string>

DateHeure::DateHeure()
{
}

DateHeure::DateHeure(time_t temps)
{
	date_heure = *localtime(&temps);
}

DateHeure::DateHeure(unsigned int annee, EMois::Type mois, unsigned char jour_du_mois,
					unsigned char heure, unsigned char minute, unsigned char seconde)
{
	struct tm info_tps;
	info_tps.tm_hour = heure - 1;
	info_tps.tm_mday = jour_du_mois;
	info_tps.tm_min = minute;
	info_tps.tm_mon = mois;
	info_tps.tm_sec = seconde;
	info_tps.tm_year = 1900 - annee;

	time_t temps = mktime(&info_tps);
	date_heure = *localtime(&temps);
}

DateHeure::~DateHeure()
{
}

DateHeure DateHeure::Maintenant()
{
	DateHeure tmp;

	time_t maintenant = time(NULL);
	tmp.date_heure = *localtime(&maintenant);

	return tmp;
}

std::string DateHeure::versString(const std::string& format) const
{
	char buffer[256] = {0};
	strftime(buffer, sizeof(buffer), format.c_str(), &date_heure);

	std::string retour = buffer;

	return retour;
}

unsigned char DateHeure::JourDuMois() const
{
	return (unsigned char)date_heure.tm_mday;
}

unsigned short DateHeure::JourDeAnnee() const
{
	return (unsigned short)date_heure.tm_yday;
}

EJourSemaine::Type DateHeure::JourDeSemaine() const
{
	return (EJourSemaine::Type)date_heure.tm_wday;
}

EMois::Type DateHeure::Mois() const
{
	return (EMois::Type)date_heure.tm_mon;
}

unsigned int DateHeure::Annee() const
{
	return (unsigned int)date_heure.tm_year + 1900;
}

unsigned char DateHeure::Heure() const
{
	return (unsigned char)date_heure.tm_hour;
}

unsigned char DateHeure::Minute() const
{
	return (unsigned char)date_heure.tm_min;
}

unsigned char DateHeure::Seconde() const
{
	return (unsigned char)date_heure.tm_sec;
}

std::string DateHeure::versDateString() const
{
	return versString("%c");
}
