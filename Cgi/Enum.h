#ifdef IMPOSSIBLE

#ifndef _ENUM_H_
#define _ENUM_H_

#include <string>

struct Enum
{
	Enum() : id(derniere_id++) {}
	explicit Enum(const std::string& nom_enum, unsigned short id_manuel)
							: id(id_manuel), nom(nom_enum) { derniere_id = id_manuel + 1; }
	explicit Enum(const std::string& nom_enum) : id(derniere_id++), nom(nom_enum) {}
	Enum(unsigned short id_manuel) : id(id_manuel) { derniere_id = id_manuel + 1; }

	virtual ~Enum() {}

	bool operator == (const Enum& droite) const
	{
		return id == droite.id;
	}

    bool operator != (const Enum& droite) const
	{
		return id != droite.id;
	}

    bool operator < (const Enum& droite) const
	{
		return id < droite.id;
	}

	bool operator > (const Enum& droite) const
	{
		return id > droite.id;
	}

	bool operator <= (const Enum& droite) const
	{
		return id <= droite.id;
	}

	bool operator >= (const Enum& droite) const
	{
		return id >= droite.id;
	}

	operator const char*() const
	{
		return nom.c_str();
	}

private:
	std::string nom;
	unsigned short id;
	static unsigned short derniere_id;
};

unsigned short Enum::derniere_id = 0;

#endif

#endif