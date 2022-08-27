#include "Puzzle.h"

Puzzle::Puzzle(std::string name, std::string desc, std::string cdesc) :
	m_Name(name), m_Desc(desc), m_CheckDesc(cdesc)
{}

std::string Puzzle::GetName() const
{
	return m_Name;
}

void Puzzle::TestLock()
{
	if (m_Name == "Trapdoor" || m_Name == "Safe" || m_Name == "Front Door")
	{
		if (m_Lock == 1 && m_Name == "Trapdoor")
		{
			m_CheckDesc = "The trapdoor is unlocked, (type 'down' to go down or 'up' to go up).";
		}
		else if (m_Lock == 1 && m_Name == "Safe")
		{
			m_CheckDesc = "The safe is unlocked, inside you see a " + m_Contains.GetName() + ".";
		}
		else if (m_Lock == 1 && m_Name == "Front Door")
		{
			m_CheckDesc = "Front door, (type exit to escape!).";
		}
	}
	else if (m_Name == "Sloted Chest")
	{
		if (m_Lock == 1)
		{
			m_CheckDesc = "One of the slots has a gem sealed in it.";
		}
		else if (m_Lock == 2)
		{
			m_CheckDesc = "Two of the slots have gems sealed in them.";
		}
		else if (m_Lock == 3)
		{
			m_CheckDesc = "All three slots have gems sealed in them, you open the chest and find a " + m_Contains.GetName() + ".";
		}
	}
}
