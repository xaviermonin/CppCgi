#ifndef _APP_H_
#define _APP_H_

#include "Environnement.h"
#include "EnvironnementCgi.h"
#include "Exception.h"
#include "ReponseHttp.h"
#include "Macros.h"
#include "Localisation.h"
#include "Client.h"

#if defined (__WINDOWS__) && defined (__DEBUGAGE__)
#include <windows.h>
#endif

namespace CppCgi {

template<class Cible, class Env=EnvironnementCgi>
class App
{
public:
	App() : attendre_debugueur(false)
	{
		if (!Environnement::Instancie())
			environnement = new Env();
		else
			environnement = Environnement::Instance();
	}

	virtual ~App()
	{
		// Génère une erreur
		///!todo Déterminer pourquoi
		//Environnement::liberer();
	}

	inline static bool Debugage()
	{
		#ifdef __DEBUGAGE__
			return true;
		#else
			return false;
		#endif
	}

	void AttendreDebugueur(bool attendre)
	{
		attendre_debugueur = attendre;
	}

	bool AttendreDebugueur() const
	{
		return attendre_debugueur;
	}

	std::locale LocaleClient()
	{
		return locale_client;
	}

	void quitter()
	{
		Environnement::quitter();
	}

	void executer()
	{
		attendreDebug();

		// Préparation de l'exécution - Environnement
		try { environnement->preExecution(); }
		catch(const std::exception& except) {
			// Utilisation de la sortie par défaut.
			std::cout << "Content-type: text/html; charset=utf-8\n\n";
			std::cout << except.what();
			return;
		}
		catch(...) {
			std::cout << "Content-type: text/html\n\nUne erreur inconnue est survenue. Détails: PreExecution - Environnement";
			return;
		}

		// Boucle tant qu'une requête est à traiter.
		while(environnement->nouveauClient())
		{
			// Préparation du client - Environnement
			try { environnement->preClient(); }
			catch(const std::exception& except)
			{
				// Utilisation de la sortie par défaut.
				std::cout << "Content-type: text/html; charset=utf-8\n\n";
				std::cout << "Erreur lors de la préparation du client - Environnement::PreClient() : " << except.what();
				continue;
			}
			catch(...)
			{
				std::cout << "Content-type: text/html\n\nUne erreur inconnue est survenue. Détails: PreClient - Environnement";
				continue;
			}

			try { configurerLocalisation(); }
			catch(const std::exception& except)
			{
				// Utilisation de la sortie par défaut.
				std::cout << "Content-type: text/html; charset=utf-8\n\n";
				std::cout << "Erreur lors de la configuration de la localisation du client"
							 "- configurerLocalisation() : " << except.what();
				continue;
			}
			catch(...)
			{
				std::cout << "Content-type: text/html\n\nUne erreur inconnue est survenue. Détails: PreClient - Environnement";
				continue;
			}

			// Préparation du client - App
			try { preClient(); }
			catch(const std::exception& except)
			{
				Environnement::Sortie() << "Content-type: text/html; charset=utf-8\n\n";
				Environnement::Sortie() << except.what();
				try { postClient(); } catch(...){}
				continue;
			}
			catch(...)
			{
				Environnement::Sortie() << "Content-type: text/html\n\nUne erreur inconnue est survenue. Détails: PreClient - App";
				try { postClient(); } catch(...) {}
				continue;
			}
			
			ReponseHttp& reponse = ReponseHttp::Instance();

			// Lancement de l'application.
			try {
				Cible* cible = new Cible();
				cible->executer();
				delete cible;

				reponse.executer();
			}
			catch (const std::exception& except) {
				Environnement::Sortie() << "Content-type: text/html; charset=utf-8\n\n";
				Environnement::Sortie() << except.what();
			}
			catch (...) { Environnement::Sortie() << "Content-type: text/html\n\nUne erreur inconnue est survenue. Détails: Exécution"; }

			// Cloture du client - App
			try { postClient(); }
			catch (const std::exception& except) {
				Environnement::Sortie() << "Content-type: text/html; charset=utf-8\n\n";
				Environnement::Sortie() << except.what();
			}
			catch (...) { Environnement::Sortie() << "Content-type: text/html\n\nUne erreur inconnue est survenue. Détails: PostClient - App"; }

			try { environnement->postClient(); }
			catch (const std::exception& except) {
				Environnement::Sortie() << "Content-type: text/html; charset=utf-8\n\n";
				Environnement::Sortie() << except.what();
			}
			catch (...) { Environnement::Sortie() << "Content-type: text/html\n\nUne erreur inconnue est survenue. Détails: PostClient - Environnement"; }
			
			ReponseHttp::libererInstance();
		}

		try { environnement->postExecution(); }
		catch(const std::exception& except) {
			Environnement::Sortie() << "Content-type: text/html; charset=utf-8\n\n";
			Environnement::Sortie() << except.what();
			return;
		}
		catch(...) {
			Environnement::Sortie() << "Content-type: text/html\n\nUne erreur inconnue est survenue lors de la postexécution.";
			return;
		}
	}

protected:
	virtual void preClient() = 0;
	virtual void postClient() = 0;

private:

	void configurerLocalisation()
	{
		locale_client = std::locale(""); // Par défaut: Locale du serveur.

		ListeClassee liste;

		try { liste = Client::Langages(); }
		catch (...) {}

		for (ListeClassee::iterator it = liste.begin(); it != liste.end(); ++it)
		{
			try {
				locale_client = Localisation::generer(it->first);
				break;
			}
			catch(...) {}
		}

		std::locale::global(locale_client);
		Environnement::Sortie().imbue(locale_client);
		Environnement::Debug().imbue(locale_client);
	}

	void attendreDebug()
	{
		if (attendre_debugueur)
		{
			#if defined (__WINDOWS__) && defined (__DEBUGAGE__)
				while(!IsDebuggerPresent())
					Sleep(100);
				Sleep(100);
			#endif
		}
		
	}

	bool attendre_debugueur;
	Environnement* environnement;
	std::locale locale_client;
};

} // namespace CppCgi

#endif
