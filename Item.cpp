#include "Item.h"

Item::Item(int heal, int healS, int damage, int negate, bool killU, std::string name, std::string floorDesc) :
	m_HealAmt(heal), m_HealSanityAmt(healS), m_Damage(damage), m_NegateAmt(negate), m_KillUndead(killU), m_Name(name), m_FloorDesc(floorDesc)
{}

int Item::GetHeal()
{
	return m_HealAmt;
}

int Item::GethealSanity()
{
	return m_HealSanityAmt;
}

int Item::GetDamage()
{
	return m_Damage;
}

int Item::GetNegate()
{
	return m_NegateAmt;
}

bool Item::GetUndead()
{
	return m_KillUndead;
}

std::string Item::GetName()
{
	return m_Name;
}

std::string Item::GetDesc()
{
	return m_FloorDesc;
}
