#ifndef ITEM_H
#define ITEM_H
#include <string>

class Item
{
public:
	Item(int heal, int healS, int damage, int negate, bool killU, std::string name, std::string floorDesc);
	int GetHeal();
	int GethealSanity();
	int GetDamage();
	int GetNegate();
	bool GetUndead();
	std::string GetName();
	std::string GetDesc();
	int m_AttackUses = 3;
private:
	std::string m_Name;
	std::string m_FloorDesc;
	int m_HealAmt = 0;
	int m_HealSanityAmt = 0;
	int m_Damage = 0;
	int m_NegateAmt = 0;
	bool m_KillUndead = false;
};
#endif
