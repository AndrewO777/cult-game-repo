#include "Trap.h"

Trap::Trap(std::string name, std::string desc, int sanity):
	m_Name(name), m_Desc(desc), m_Sanity(sanity)
{}

std::string Trap::GetDesc()
{
	return m_Desc;
}

std::string Trap::GetName()
{
	return m_Name;
}

int Trap::GetSanity()
{
	return m_Sanity;
}