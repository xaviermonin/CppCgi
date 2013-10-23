#ifndef _INCOPIABLE_H_
#define _INCOPIABLE_H_

class Incopiable
{
private:
	Incopiable(const Incopiable&);
	Incopiable& operator = (const Incopiable&);
public:
	Incopiable() {}
	virtual ~Incopiable() {}
};

#endif
