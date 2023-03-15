#include "Interact.h"
#include <iostream>

Interact::Interact(std::string name, std::string desc) :
	m_Name(name), m_Desc(desc)
{}

std::string Interact::GetName() const
{
	return m_Name;
}
