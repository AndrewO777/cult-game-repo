#ifndef INTERACT_H
#define INTERACT_H
#include "Item.h"
#include "Trap.h"
#include <vector>

class Interact
{
public:
	Interact(std::string name, std::string desc);
	std::string GetName() const;
	std::string m_Desc;
	Item m_Contains = Item(0,0,0,0,false,"nothing","");
	bool m_HasTrap = false;
	Trap m_Trap = Trap("", "", 0);
private:
	std::string m_Name;
};
#endif
