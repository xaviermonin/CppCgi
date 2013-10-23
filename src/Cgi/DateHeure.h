#ifndef _DATE_HEURE_H_
#define _DATE_HEURE_H_

#include <string>

struct tm;

namespace EJourSemaine {
	enum Type { Dimanche, Lundi, Mardi, Mercredi, Jeudi, Vendredi, Samedi };
}
namespace EMois {
	enum Type { Janvier, Fevrier, Mars, Avril, Mai, Juin, Juillet, Aout, Septembre, Octobre, Novembre, Decembre };
}

class DateHeure
{
public:
	DateHeure();
	DateHeure(time_t date_heure);
	// Heure [1;24]
	DateHeure(unsigned int annee, EMois::Type mois, unsigned char jour_du_mois,
				unsigned char heure = 0, unsigned char minute = 0, unsigned char seconde = 0);
	~DateHeure();

	static DateHeure Maintenant();

	std::string versString(const std::string& format) const;
	std::string versDateString() const;

	unsigned char JourDuMois() const;
	unsigned short JourDeAnnee() const;

	EJourSemaine::Type JourDeSemaine() const;
	EMois::Type Mois() const;

	unsigned int Annee() const;
	unsigned char Heure() const;
	unsigned char Minute() const;
	unsigned char Seconde() const;

private:
	tm date_heure;
};

#endif
