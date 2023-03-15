#ifndef TRAP_H
#define TRAP_H
#include <string>

class Trap
{
public:
	Trap(std::string name, std::string desc, int sanity);
	std::string GetName();
	std::string GetDesc();
	int GetSanity();
private:
	std::string m_Name;
	std::string m_Desc;
	int m_Sanity;
};
#endif